package corert

import (
	"math"
	"sort"
	"strconv"
	"strings"

	"github.com/beevik/etree"
)

const musicXMLVersion = "3.0"

var decimalFields = map[string]bool{
	"top-system-distance": true,
	"dynamics":            true,
	"left-margin":         true,
	"right-margin":        true,
	"staff-distance":      true,
	"system-distance":     true,
	"default-y":           true,
	"default-x":           true,
	"tenths":              true,
	"width":               true,
}

func Normalize(doc *etree.Document) {
	setXMLDeclaration(doc)
	setDoctypeFromRoot(doc)
	setRootMusicXMLVersion(doc)
	stripInterElementWhitespace(doc.Root())
	stripZerosFromDecimalFields(doc.Root())
	sortAttributes(doc.Root())
}

// stripInterElementWhitespace removes whitespace-only character data from
// every element: pretty-printing indentation in containers, including
// containers whose optional children are all absent (an empty <measure>
// holds only its own indentation). MusicXML has no mixed content, and the
// rule applies to expected and actual alike, so leaf values with real
// content are never touched and the comparison stays symmetric.
func stripInterElementWhitespace(el *etree.Element) {
	if el == nil {
		return
	}
	var remove []etree.Token
	for _, tok := range el.Child {
		if cd, ok := tok.(*etree.CharData); ok && strings.TrimSpace(cd.Data) == "" {
			remove = append(remove, tok)
		}
	}
	for _, tok := range remove {
		el.RemoveChild(tok)
	}
	for _, child := range el.ChildElements() {
		stripInterElementWhitespace(child)
	}
}

func setXMLDeclaration(doc *etree.Document) {
	doc.CreateProcInst("xml", `version="1.0" encoding="UTF-8" standalone="no"`)
}

func setDoctypeFromRoot(doc *etree.Document) {
	root := doc.Root()
	if root == nil {
		return
	}
	// Remove any existing directives that look like DOCTYPE.
	var toRemove []etree.Token
	for _, tok := range doc.Child {
		if dir, ok := tok.(*etree.Directive); ok {
			if strings.Contains(dir.Data, "DOCTYPE") {
				toRemove = append(toRemove, tok)
			}
		}
	}
	for _, tok := range toRemove {
		doc.RemoveChild(tok)
	}

	var doctype string
	if root.Tag == "score-timewise" {
		doctype = `DOCTYPE score-timewise PUBLIC "-//Recordare//DTD MusicXML 3.0 Timewise//EN" "http://www.musicxml.org/dtds/timewise.dtd"`
	} else {
		doctype = `DOCTYPE score-partwise PUBLIC "-//Recordare//DTD MusicXML 3.0 Partwise//EN" "http://www.musicxml.org/dtds/partwise.dtd"`
	}
	doc.CreateDirective(doctype)
}

func setRootMusicXMLVersion(doc *etree.Document) {
	root := doc.Root()
	if root == nil {
		return
	}
	attr := root.SelectAttr("version")
	if attr != nil {
		attr.Value = musicXMLVersion
	} else {
		root.CreateAttr("version", musicXMLVersion)
	}
}

func sortAttributes(el *etree.Element) {
	if el == nil {
		return
	}
	if len(el.Attr) > 1 {
		// Qualified names: xlink:href must sort (and compare) as itself, not
		// as a second "href".
		sort.Slice(el.Attr, func(i, j int) bool {
			return el.Attr[i].FullKey() < el.Attr[j].FullKey()
		})
	}
	for _, child := range el.ChildElements() {
		sortAttributes(child)
	}
}

func stripZerosFromDecimalFields(el *etree.Element) {
	if el == nil {
		return
	}
	hasChildren := len(el.ChildElements()) > 0
	if !hasChildren && decimalFields[el.Tag] {
		el.SetText(stripTrailingZeros(el.Text()))
	}
	for i := range el.Attr {
		if decimalFields[el.Attr[i].Key] {
			el.Attr[i].Value = stripTrailingZeros(el.Attr[i].Value)
		}
	}
	for _, child := range el.ChildElements() {
		stripZerosFromDecimalFields(child)
	}
}

func stripTrailingZeros(s string) string {
	if s == "" {
		return s
	}
	dotIdx := strings.LastIndex(s, ".")
	if dotIdx < 0 {
		return s
	}
	trimmed := strings.TrimRight(s, "0")
	if strings.HasSuffix(trimmed, ".") {
		trimmed = trimmed[:len(trimmed)-1]
	}
	if trimmed == "-0" {
		return "0"
	}
	return trimmed
}

func IsEquivalent(a, b string) bool {
	if a == b {
		return true
	}
	if equivInt(a, b) {
		return true
	}
	if equivFloat(a, b) {
		return true
	}
	return false
}

func equivInt(a, b string) bool {
	ai, errA := strconv.ParseInt(a, 10, 64)
	bi, errB := strconv.ParseInt(b, 10, 64)
	return errA == nil && errB == nil && ai == bi
}

func equivFloat(a, b string) bool {
	af, errA := strconv.ParseFloat(a, 64)
	bf, errB := strconv.ParseFloat(b, 64)
	if errA != nil || errB != nil {
		return false
	}
	if math.IsNaN(af) || math.IsNaN(bf) || math.IsInf(af, 0) || math.IsInf(bf, 0) {
		return false
	}
	return math.Abs(af-bf) < 0.00000001
}
