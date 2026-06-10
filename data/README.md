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

The sidecars encode one uniform leniency policy, shared by every generated target:

- an unknown enum literal falls back to the enum's first variant (`display-step` `=` -> `A`);
- an unparseable number becomes 0, with decimal-looking integers truncating toward zero;
- every number then clamps into its declared range, including the primitive-implied lower bounds
  (`xs:positiveInteger` >= 1, `xs:nonNegativeInteger` >= 0), so `midi-channel` `0` -> `1` and
  `accordion-middle` `` -> `1`;
- an exclusive decimal bound clamps to the bound +/- 1e-6 (`duration` `0` -> `0.000001`).
