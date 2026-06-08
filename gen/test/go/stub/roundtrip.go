package stub

import (
	"errors"

	"github.com/beevik/etree"
)

var ErrNotImplemented = errors.New("generated parser not implemented")

// FromXDoc parses an etree document into the typed MusicXML model.
// This is a stub that always returns an error until the generator
// emits the Go typed model.
func FromXDoc(doc *etree.Document) (any, error) {
	return nil, ErrNotImplemented
}

// ToXDoc serializes the typed MusicXML model back to an etree document.
// This is a stub that always returns an error until the generator
// emits the Go typed model.
func ToXDoc(model any) (*etree.Document, error) {
	return nil, ErrNotImplemented
}
