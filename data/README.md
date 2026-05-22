# data/

MusicXML test inputs and expected-output fixtures.

## `.invalid` marker convention

A file `foo.xml` (or `foo.musicxml`) that is **not** valid MusicXML must be
accompanied by a sibling marker file `foo.xml.invalid` whose contents
describe, in prose, why the file is invalid.

The marker tells schema-strict consumers (notably the **core roundtrip**
suite at `src/private/mxtest/corert/`) to skip the file. `mx::core` is a
strongly-typed DOM generated from the MusicXML XSD and cannot round-trip
elements or attributes that are not in the schema; running such a file
through `fromXDoc` → `toXDoc` is guaranteed to lose content.

The marker is descriptive only — its body is for humans. Presence of the
file is the signal.

Other suites (e.g. **api import** under `src/private/mxtest/import/`) may
still include invalid files when they exercise reader leniency on
malformed input. Those suites ignore the marker.
