# `data/api.features.xml` format

The api audit output is a superset of the per-file `<feature-audit>` shape (`audit/featuresfile.py`),
so it lines up element-for-element with `data/corpus.xml`. It adds support markers and an `<enums>`
section that the per-file corpus sidecars do not have.

Write it to `data/api.features.xml`. That name ends in `.features.xml`, so the round-trip test
suites skip it automatically (same rule as the corpus sidecars).

```xml
<?xml version="1.0" encoding="UTF-8"?>
<!-- Audit of mx::api feature support. Produced by the api-feature-audit skill. -->
<feature-audit>
  <audited>mx::api</audited>
  <audited-version>4.0</audited-version>   <!-- the max MusicXML version mx::core supports -->
  <features>
    <feature name="dynamics" support="partial">
      <attributes>
        <attribute name="placement" support="yes"/>
        <attribute name="default-x" support="yes"/>
        <attribute name="font-size" support="no"/>
      </attributes>
      <enums>
        <!-- A BUG: core has members the api enum cannot represent. -->
        <enum core-type="DynamicsChoice::Kind" api-type="MarkType" status="missing-members">
          <missing>n</missing>
          <missing>pf</missing>
          <missing>sfzp</missing>
        </enum>
      </enums>
      <notes>Exposed as MarkData/MarkType via DynamicsReader/DynamicsWriter.</notes>
    </feature>

    <feature name="credit-words" support="none">
      <notes>No api representation; credits are not modeled.</notes>
    </feature>
  </features>
</feature-audit>
```

## Vocabulary

- `<feature support="full|partial|none">` -- `full`: element and its commonly-used attributes are
  exposed and round-trip; `partial`: element is exposed but some attributes/enum members are not;
  `none`: not modeled at all.
- `<attribute support="yes|no">` -- whether that attribute is carried through `mx::api`. List the
  attributes that appear in `data/corpus.xml` for the element (at least the ones with non-zero
  `wild-files`); you do not have to list spec attributes no corpus file uses.
- `<enum core-type="..." api-type="..." status="ok|missing-members">` -- the result of the Step 2
  diff for an enum that belongs to this element. `status="ok"` means the api enum represents every
  core member; `missing-members` lists each lost core value in a `<missing>` child. `core-type` is
  the `mx::core` enum (qualify choice kinds, e.g. `DynamicsChoice::Kind`); `api-type` is the
  `mx::api` enum.
- `<notes>` -- free text: where/how it is exposed, or why it is not.

## Conventions

- Use the same element `name` spelling as `data/corpus.xml` (the MusicXML element name) so the two
  files can be compared directly.
- Sort `<feature>` entries alphabetically by `name` for stable diffs.
- It is fine to omit `<attributes>`/`<enums>`/`<notes>` when they do not apply (e.g. a
  `support="none"` feature needs only `<notes>`).
