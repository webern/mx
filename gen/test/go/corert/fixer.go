package corert

import (
	"os"
	"path/filepath"
	"strings"

	"github.com/beevik/etree"
)

type fixupType int

const (
	fixupElement   fixupType = iota
	fixupAttribute fixupType = iota
)

type fixup struct {
	typ              fixupType
	name             string
	value            string
	replacementValue string
}

type Fixer struct {
	fixups []fixup
}

func NewFixer(inputFilePath string) *Fixer {
	f := &Fixer{}
	sidecarPath := fixupSidecarPath(inputFilePath)
	if _, err := os.Stat(sidecarPath); err != nil {
		return f
	}
	doc := etree.NewDocument()
	if err := doc.ReadFromFile(sidecarPath); err != nil {
		return f
	}
	root := doc.Root()
	if root == nil || root.Tag != "fixups" {
		return f
	}
	for _, replace := range root.SelectElements("replace") {
		fx, ok := parseReplace(replace)
		if ok {
			f.fixups = append(f.fixups, fx)
		}
	}
	return f
}

func (f *Fixer) HasFixups() bool {
	return len(f.fixups) > 0
}

func (f *Fixer) ApplyToExpected(doc *etree.Document) {
	if len(f.fixups) == 0 || doc.Root() == nil {
		return
	}
	applyToElement(doc.Root(), f.fixups)
}

func fixupSidecarPath(inputPath string) string {
	ext := filepath.Ext(inputPath)
	return strings.TrimSuffix(inputPath, ext) + ".fixup.xml"
}

func parseReplace(el *etree.Element) (fixup, bool) {
	var fx fixup
	var gotType, gotName, gotValue, gotReplacement bool
	for _, child := range el.ChildElements() {
		switch child.Tag {
		case "type":
			switch child.Text() {
			case "element":
				fx.typ = fixupElement
				gotType = true
			case "attribute":
				fx.typ = fixupAttribute
				gotType = true
			}
		case "name":
			fx.name = child.Text()
			gotName = true
		case "value":
			fx.value = child.Text()
			gotValue = true
		case "replacement-value":
			fx.replacementValue = child.Text()
			gotReplacement = true
		}
	}
	return fx, gotType && gotName && gotValue && gotReplacement
}

func applyToElement(el *etree.Element, fixups []fixup) {
	hasChildren := len(el.ChildElements()) > 0
	text := el.Text()

	if !hasChildren {
		for _, fx := range fixups {
			if fx.typ != fixupElement || fx.name != el.Tag {
				continue
			}
			if text == "" && fx.value == "" {
				el.SetText(fx.replacementValue)
				break
			}
			if text == fx.value {
				el.SetText(fx.replacementValue)
				break
			}
		}
	}

	for _, attr := range el.Attr {
		for _, fx := range fixups {
			if fx.typ != fixupAttribute || fx.name != attr.Key || fx.value != attr.Value {
				continue
			}
			el.RemoveAttr(attr.Key)
			el.CreateAttr(attr.Key, fx.replacementValue)
			break
		}
	}

	for _, child := range el.ChildElements() {
		applyToElement(child, fixups)
	}
}
