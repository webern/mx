# Changelog
All notable changes to this project will be documented in this file.
On 2020-05-31, two tags were deleted from origin, `v1.0.0` and `v1.0.1` and replaced with `v0.4.0` and `v0.4.1` respectively.
If you cloned/forked the repository before 2020-05-31, you should delete `v1.0.0` and `v1.0.1` and pull tags.

## [Unreleased]
- Change the default-constructed value of `DynamicsEnum` to `mf` instead of `otherDynamics`. [#106]/[8a5cd6b]
- Change the spelling of `KindValue` enums from, e.g. `dominant11Th` to `dominant11th`. [#105]
- Break up `Strings.h` into multiple `.h` files, one for each type [#107]. (Related to codegen, [#58]).
- Rewrite `Integers.h` and `Integers.cpp`. Changed `NonNegativeInteger` to default to 0, and changed the tests that had locked-in that behavior. [#109]. (Related to codegen, [#58]).
- Rewrite `Decimals.h` and `Decimals.cpp`. [#111]. (Related to codegen, [#58]).

[#105]: https://github.com/webern/mx/pull/105
[#106]: https://github.com/webern/mx/pull/106
[#107]: https://github.com/webern/mx/pull/107
[#109]: https://github.com/webern/mx/pull/109
[#111]: https://github.com/webern/mx/pull/111
[#58]: https://github.com/webern/mx/issues/58
[8a5cd6b]: https://github.com/webern/mx/commit/8a5cd6b

## [v0.5.1] - 2020-06-13
- Fix xcode build, new headers were missing from frameworks.

## [v0.5.0] - 2020-06-13
### Breaking Changes
- Add support for `new-page` attributes and page layout [#94]
- The `LayoutData layout` field of `ScoreData` has been renamed to `DefaultsData defaults` [#94]
  - What used to be the `LayoutData` class has been renamed to `DefaultsData`.
  - This name change freed up the name `LayoutsData` to be used for a new class that better fits the name.
- The `std::set<SystemData> systems` field of `ScoreData` has been replaced by `std::map<MeasureIndex, LayoutData>`. [#94]
  - `LayoutData` is a new class (not to be confused with `DefaultsData` which previously held the name).
  - `LayoutData` holds a `SystemData` and a `PageData` (new) to specify system and page layout.
- Some recurring data patterns were factored out into new classes. [#94]
  - The existing classes affected by these changes are:
    - `DefaultsData` (which used to be called `LayoutData`).
    - `SystemData`
  - The new classes are:
    - `LeftRight`
    - `MarginsData`
    - `PageLayoutData`
    - `PageMarginsData`
    - `SizeData`
    - `SystemLayoutData`

### Other Changes
- Bump C++ standard to 17 [#93]
- Support non-traditional key signatures in `mx::api`. [#81]
- Add a constructor for `PitchData`. [#90]
- Provide a hint in the exception message when a zip file is attempted. [#67]
- Update the readme fairly substantially. [#91]
- Add a changelog. [#97]
- Hide the independence of the XML parser to reduce confusion about dependency management. [#79]
- The introduction of `std::optional` as a pattern for specifying optional data. [#94]

[#67]: https://github.com/webern/mx/pull/67
[#79]: https://github.com/webern/mx/pull/79
[#81]: https://github.com/webern/mx/pull/81
[#90]: https://github.com/webern/mx/pull/90
[#91]: https://github.com/webern/mx/pull/91
[#93]: https://github.com/webern/mx/pull/93
[#94]: https://github.com/webern/mx/pull/94
[#97]: https://github.com/webern/mx/pull/97

## [v0.4.1] - 2019-11-23
- Mostly internal changes related to the xml parser.

## [v0.4.0] - 2019-04-08
- A simplified data model, called `mx::api`.

## [v0.3.0] - 2017-11-20
- MusicXML files can be parsed into the C++ classes.

## [v0.2.0] - 2016-08-21
- MusicXML 3.0 represented in C++ classes, mostly generated from XSD using custom Ruby scripts.
- MusicXML DOM classes can serialize to XML.

## [v0.1.0] - 2016-06-20
- Initial Commit

[v0.1.0]: https://github.com/webern/mx/commit/5d46609
[v0.2.0]: https://github.com/webern/mx/compare/5d46609..v0.2.0
[v0.3.0]: https://github.com/webern/mx/compare/v0.2.0..v0.3.0
[v0.4.0]: https://github.com/webern/mx/compare/v0.3.0..v0.4.0
[v0.4.1]: https://github.com/webern/mx/compare/v0.4.0..v0.4.1
[v0.5.0]: https://github.com/webern/mx/compare/v0.4.1..v0.5.0
[v0.5.1]: https://github.com/webern/mx/compare/v0.5.0..v0.5.1
[Unreleased]: https://github.com/webern/mx/compare/v0.5.1...HEAD

#### Historical Notes

**Historical Note: April 7, 2019** Trying to button up a '1.0' 'release' by tying up some of the loose ends with the build, continuous integration, testing, and header leakage.
The use of the semver `1.0` implies that we hope there won't be many breaking changes.
I suppose the way I will use semver is as follows: a patch release may cause slight compilation annoyances, but nothing that would cause any rewriting of code.
A minor release may similarly require some tweaks but hopefully nothing too major.
A major release could be anything up-to a complete re-write.

**Historical Note: October 6, 2016:** Significant progress has been made on the `api` namespace, which is a simplified set of data structures to represent a MusicXML document.
These are being implemented as mostly-POD structs.
Currently the importing of data into these structures is well-underway, but the exporting from these data structures has not been implemented.

**Historical Note: August 16, 2016:** All tests are passing (core, xml and import).
The remaining items to do on the ximport feature are
- search for all `\\TODO's` and fix those that can be fixed
- Standardize the code file copyrights and bump to version 0.2
- Rename XElement functions to better names
- Update the readme including information about the XDoc XElement classes and how to implement those interfaces.

**Historical Note: August 11, 2016:** The feature to import MusicXML files is essentially complete with most "round-trip" integration tests passing.

**Historical Note: June 27, 2016:** An "integration" tester has been added to the MxTest executable.
This exists in ImportTest.cpp, and ImportTestImpl.h/cpp.
A top level directory "Resources" has been added to hold test input (i.e. "golden") files.
The input files are gathered from Recordare, Lilypond and MuseScore and to this I have added a few of my own scores as exported by Finale Dolet.

Each of these test input files has been "scrubbed" using the XDoc classes (i.e. it has been round-tripped through pugixml and has been updated to a normalized MusicXML 3.0 header format.
The resultant scrubbed files are in Resources/expected.
During the test run, a csv file is written in Resources/testOutput recording a row for each test (Pass/Fail, duration of test, messages, etc).
Each time a test failure is encountered the expected file and the error file will be saved to the Resources/testOutput directory to allow for visual inspection.

Currently this tester is a "wire-up".
All 263 of these round-trip import/export tests fail because the implementation does not yet exist in mx::core.
The next body of work will be the mx::core implementation.

**Historical Note: June 20, 2016:** A simple interface to for XML DOM has been added in the ::ezxml:: namespace.
The key classes (pure virtual) are XDoc, XElement, XAttribute, XElementIterator, XAttributeIterator.
These are implemented by concrete classes PugiDoc, PugiElement, etc. which serve as a wrapper for the pugixml library (http://pugixml.org/).
Although this is a static library, a class XFactory can be used to create a Pugi instance of the XDoc interface.

The idea behind using a pure virtual interface is that the client of the Music XML Class Library can, in theory choose a different XML DOM library (Xerces, TinyXML, etc) and wrap with instances of the XDoc interfaces and the Music XML core classes will not know the difference.

**Historical Note: June 20, 2016:** The GitHub repo was bloated and has been entirely wiped-out and restarted.
