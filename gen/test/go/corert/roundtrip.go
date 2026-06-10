package corert

import (
	"bytes"
	"fmt"
	"io"
	"os"
	"path/filepath"
	"strconv"
	"strings"
	"unicode/utf16"
	"unicode/utf8"

	"github.com/beevik/etree"
	"github.com/webern/mx/gen/test/go/mx"
)

// The MusicXML version the generated model supports comes from the model
// itself (the generator emits it from the schema the config pins), so
// retargeting the schema cannot leave the gate stale. Documents declaring a
// NEWER version may use elements the model has no types for; MusicXML is
// backward compatible, so older documents are fine.
const maxSupportedVersion = mx.SupportedMusicXMLVersion

type Result struct {
	OK          bool
	Skipped     bool
	Message     string
	ExpectedXML string
	ActualXML   string
}

// declaredVersionExceeds reports whether the document's root version
// attribute declares a version newer than max. An absent attribute means
// MusicXML 1.0.
func declaredVersionExceeds(doc *etree.Document, max string) bool {
	root := doc.Root()
	if root == nil {
		return false
	}
	version := "1.0"
	if a := root.SelectAttr("version"); a != nil && a.Value != "" {
		version = a.Value
	}
	parse := func(v string) (int, int) {
		parts := strings.SplitN(v, ".", 3)
		major, _ := strconv.Atoi(parts[0])
		minor := 0
		if len(parts) > 1 {
			minor, _ = strconv.Atoi(parts[1])
		}
		return major, minor
	}
	maj, min := parse(version)
	maxMaj, maxMin := parse(max)
	return maj > maxMaj || (maj == maxMaj && min > maxMin)
}

// charsetReader handles the non-UTF-8 encodings the corpus contains.
// ISO-8859-1 is a one-byte-per-rune encoding, so its conversion is total.
// UTF-16 documents were already transcoded whole (see loadDocument), so a
// declared utf-16 passes through as the UTF-8 it now is. Anything else
// passes through to the decoder's UTF-8 validation. (pugixml and libxml2
// auto-detect these encodings natively; Go's encoding/xml does not.)
func charsetReader(charset string, input io.Reader) (io.Reader, error) {
	switch strings.ToLower(charset) {
	case "iso-8859-1", "latin1":
		raw, err := io.ReadAll(input)
		if err != nil {
			return nil, err
		}
		var out []byte
		for _, b := range raw {
			out = utf8.AppendRune(out, rune(b))
		}
		return bytes.NewReader(out), nil
	}
	return input, nil
}

// transcodeUTF16 converts a byte-order-marked UTF-16 document to UTF-8;
// anything without a BOM is returned untouched.
func transcodeUTF16(raw []byte) []byte {
	if len(raw) < 2 {
		return raw
	}
	little := raw[0] == 0xFF && raw[1] == 0xFE
	big := raw[0] == 0xFE && raw[1] == 0xFF
	if !little && !big {
		return raw
	}
	raw = raw[2:]
	units := make([]uint16, 0, len(raw)/2)
	for i := 0; i+1 < len(raw); i += 2 {
		if little {
			units = append(units, uint16(raw[i])|uint16(raw[i+1])<<8)
		} else {
			units = append(units, uint16(raw[i])<<8|uint16(raw[i+1]))
		}
	}
	return []byte(string(utf16.Decode(units)))
}

func loadDocument(absPath string) (*etree.Document, error) {
	raw, err := os.ReadFile(absPath)
	if err != nil {
		return nil, err
	}
	doc := etree.NewDocument()
	doc.ReadSettings.CharsetReader = charsetReader
	if err := doc.ReadFromBytes(transcodeUTF16(raw)); err != nil {
		return nil, err
	}
	return doc, nil
}

func RunCoreRoundtrip(absInputPath string) Result {
	inputDoc, err := loadDocument(absInputPath)
	if err != nil {
		return Result{Message: fmt.Sprintf("load input: %v", err)}
	}

	if declaredVersionExceeds(inputDoc, maxSupportedVersion) {
		return Result{Skipped: true, Message: fmt.Sprintf(
			"declares MusicXML > %s; this target generates from the %s schema",
			maxSupportedVersion, maxSupportedVersion)}
	}

	setRootMusicXMLVersion(inputDoc)

	model, err := mx.FromXDoc(inputDoc)
	if err != nil {
		return Result{Message: fmt.Sprintf("FromXDoc: %v", err)}
	}

	actualDoc, err := mx.ToXDoc(model)
	if err != nil {
		return Result{Message: fmt.Sprintf("ToXDoc: %v", err)}
	}

	Normalize(actualDoc)

	expectedDoc, err := loadDocument(absInputPath)
	if err != nil {
		return Result{Message: fmt.Sprintf("load expected: %v", err)}
	}
	setRootMusicXMLVersion(expectedDoc)
	Normalize(expectedDoc)

	fixer := NewFixer(absInputPath)
	fixer.ApplyToExpected(expectedDoc)

	failure := compareElements(expectedDoc.Root(), actualDoc.Root(), nil)
	if failure != "" {
		expectedStr, _ := expectedDoc.WriteToString()
		actualStr, _ := actualDoc.WriteToString()
		return Result{
			Message:     failure,
			ExpectedXML: expectedStr,
			ActualXML:   actualStr,
		}
	}

	return Result{OK: true}
}

func WriteFailureFiles(repoRoot, testName, expectedXML, actualXML string) {
	outDir := filepath.Join(repoRoot, "data", "testOutput", "corert")
	os.MkdirAll(outDir, 0o755)

	flat := strings.ReplaceAll(testName, "/", "_")
	flat = strings.ReplaceAll(flat, "\\", "_")

	os.WriteFile(filepath.Join(outDir, flat+".expected.xml"), []byte(expectedXML), 0o644)
	os.WriteFile(filepath.Join(outDir, flat+".actual.xml"), []byte(actualXML), 0o644)
}

func compareElements(expected, actual *etree.Element, path []string) string {
	if expected == nil || actual == nil {
		return "nil element in comparison"
	}

	if expected.Tag != actual.Tag {
		return fmt.Sprintf("element name mismatch at %s: expected '%s', actual '%s'",
			nodePath(path), expected.Tag, actual.Tag)
	}

	if !IsEquivalent(expected.Text(), actual.Text()) {
		return fmt.Sprintf("mismatch at %s: expected '%s', actual '%s'",
			nodePath(path), expected.Text(), actual.Text())
	}

	eAttrs := expected.Attr
	aAttrs := actual.Attr
	if len(eAttrs) != len(aAttrs) {
		return fmt.Sprintf("attribute count mismatch at %s", nodePath(path))
	}
	for i := range eAttrs {
		// Compare qualified names: a defect that drops a prefix
		// (xlink:href -> href) must fail, not slide by on the local name.
		if eAttrs[i].FullKey() != aAttrs[i].FullKey() || !IsEquivalent(eAttrs[i].Value, aAttrs[i].Value) {
			return fmt.Sprintf("attribute mismatch at %s[@%s]: expected '%s=%s', actual '%s=%s'",
				nodePath(path), eAttrs[i].FullKey(),
				eAttrs[i].FullKey(), eAttrs[i].Value,
				aAttrs[i].FullKey(), aAttrs[i].Value)
		}
	}

	eChildren := expected.ChildElements()
	aChildren := actual.ChildElements()
	if len(eChildren) != len(aChildren) {
		return fmt.Sprintf("child count mismatch at %s", nodePath(path))
	}
	for i := range eChildren {
		childPath := append(path, fmt.Sprintf("%s[%d]", eChildren[i].Tag, i))
		if fail := compareElements(eChildren[i], aChildren[i], childPath); fail != "" {
			return fail
		}
	}

	return ""
}

func nodePath(segments []string) string {
	if len(segments) == 0 {
		return "/"
	}
	return "/" + strings.Join(segments, "/")
}
