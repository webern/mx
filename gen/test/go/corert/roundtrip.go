package corert

import (
	"fmt"
	"os"
	"path/filepath"
	"strings"

	"github.com/beevik/etree"
	"github.com/webern/mx/gen/test/go/stub"
)

type Result struct {
	OK          bool
	Message     string
	ExpectedXML string
	ActualXML   string
}

func RunCoreRoundtrip(absInputPath string) Result {
	inputDoc := etree.NewDocument()
	if err := inputDoc.ReadFromFile(absInputPath); err != nil {
		return Result{Message: fmt.Sprintf("load input: %v", err)}
	}

	setRootMusicXMLVersion(inputDoc)

	model, err := stub.FromXDoc(inputDoc)
	if err != nil {
		return Result{Message: fmt.Sprintf("FromXDoc: %v", err)}
	}

	actualDoc, err := stub.ToXDoc(model)
	if err != nil {
		return Result{Message: fmt.Sprintf("ToXDoc: %v", err)}
	}

	Normalize(actualDoc)

	expectedDoc := etree.NewDocument()
	if err := expectedDoc.ReadFromFile(absInputPath); err != nil {
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
		if eAttrs[i].Key != aAttrs[i].Key || !IsEquivalent(eAttrs[i].Value, aAttrs[i].Value) {
			return fmt.Sprintf("attribute mismatch at %s[@%s]: expected '%s=%s', actual '%s=%s'",
				nodePath(path), eAttrs[i].Key,
				eAttrs[i].Key, eAttrs[i].Value,
				aAttrs[i].Key, aAttrs[i].Value)
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
