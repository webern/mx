# data/

MusicXML test inputs and expected-output fixtures.

## `.invalid` marker convention

A file `foo.xml` (or `foo.musicxml`) that is **not** valid MusicXML must be accompanied by a sibling
marker file `foo.xml.invalid` whose contents describe, in prose, why the file is invalid. The
purpose is to use the file is to mark for test suites which files are not expected to be parseable
by `mx`.

## `.fixup.xml` sidecar convention

A file `foo.xml` that is schema-invalid parseable due to `mx` leniency will have a sidecar file,
`foo.fixup.xml`, which describes how the file will be parsed. The core roundtrip suite (`corert`)
loads it via `Fixer` (`src/private/mxtest/corert/Fixer.h`) and uses it to prepare the "expected"
test by altering values it finds after loading the test file.

```xml
<fixups>
  <replace>
    <type>element</type>
    <name>midi-channel</name>
    <value>0</value>
    <replacement-value>1</replacement-value>
  </replace>
  <replace>
    <type>attribute</type>
    <name>dynamics</name>
    <value>-1.11</value>
    <replacement-value>0</replacement-value>
  </replace>
</fixups>
```

## `synthetic/` version suffixes and 3.0-only attributes

Files under `synthetic/` are named `<element>.<musicxml-version>.xml`: the version is the schema
the file's constructs first appear in (the root `version` attribute is pinned to `3.0` like the
rest of the corpus; harnesses gate on it only when it declares something newer than their schema).

Two synthetic files exercise types whose attribute sets MusicXML itself narrowed after 3.0 --
a place where the spec broke its own backward compatibility:

- `elision.3.0.xml`: in 3.0, `elision` has type `text-font-color`, which carries `underline`,
  `overline`, `line-through`, `rotation`, `letter-spacing`, `xml:lang`, and `dir`. In 3.1 and 4.0
  the element was retyped to the new `elision` type (font + color + `smufl` only), so those
  attributes are invalid there and the file does not use them: no generated model 3.1+ can
  represent them, and a 4.0 copy keeping them would simply be invalid MusicXML.
- `extend.3.0.xml`: in 3.0, `extend` carries the full `print-style` group (position + font +
  color). 3.1 and 4.0 narrowed it to position + color, dropping the font attributes, with the
  same consequence.

## `.fixup.xml` leniency policy

The sidecars encode one uniform leniency policy, shared by every generated target:

- an unknown enum literal falls back to the enum's first variant (`display-step` `=` -> `A`);
- an unparseable number becomes 0, with decimal-looking integers truncating toward zero;
- every number then clamps into its declared range, including the primitive-implied lower bounds
  (`xs:positiveInteger` >= 1, `xs:nonNegativeInteger` >= 0), so `midi-channel` `0` -> `1` and
  `accordion-middle` `` -> `1`;
- an exclusive decimal bound clamps to the bound +/- 1e-6 (`duration` `0` -> `0.000001`);
- string facets (pattern, length) are enforced by the strict parse only (`TryParseX` reports
  false): the lenient parse the deserializer uses keeps the value verbatim, because unlike a
  numeric bound there is no canonical replacement for a failed pattern, and round-trip fidelity
  wins. No fixup sidecar therefore ever encodes a string substitution.

## Normalization pipeline

Before `corert` compares an expected document against the one `mx` re-serialized, it normalizes
both so the comparison is canonical-against-canonical. The pipeline lives in
`src/private/mxtest/corert/Compare.cpp` and `src/private/mxtest/import/Normalize.cpp`:

1. Pin the root `version` attribute to the harness baseline (`3.0`).
2. Strip whitespace-only text nodes from every element. Pretty-printing indentation is not content
   (MusicXML has no mixed content), and the rule is applied to both sides, so it stays symmetric.
3. Strip trailing zeros from decimal fields: `mx` serializes the shortest round-trip form, so a
   trailing-zero decimal like `40.00000` in a source file would otherwise mismatch. The field list
   lives in `DecimalFields.h`.
4. Sort each element's attributes alphabetically by qualified name (`xlink:href`, not `href`); it
   runs last.

Comparison rules that took debugging to get right:

- Compare each element's direct text only, never the subtree concatenation: a
  numerically-equivalent leaf reformat would otherwise fail at every ancestor, not only the leaf.
- Compare attributes by qualified name, so a defect that drops a prefix (`xlink:href` -> `href`)
  fails instead of sliding by on the local name.
- A document whose root declares a MusicXML version newer than the model's
  `SupportedMusicXMLVersion` is skipped, not failed: it may use elements the generated model has no
  types for.
