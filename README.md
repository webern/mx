# MusicXML Class Library

- Author: Matthew James Briggs
- License: MIT
- Supported MusicXML Version: 4.0
- Language: C++20

* * *

## Introduction

This project is a C++ library for working with MusicXML.

## Status

The `mx::core` typed model is generated: a language-agnostic code generator (`gen/`) reads the
MusicXML 4.0 XSD and emits `mx::core` (see the [Code Generation](#code-generation) section). The
simplified `mx::api` layer and its `mx::impl` implementation are fully ported to the generated core
and build by default.

# Build

A top-level `Makefile` wraps CMake and encodes the build/test configurations this project uses. It
is a convenience layer, not a replacement for CMake. It needs `cmake` (>= 3.13) and a POSIX shell.

Building and running the core roundtrip suite should be as simple as:

```
git clone https://github.com/webern/mx.git mx
cd mx
make test-core-dev
```

Run `make` (or `make help`) to list every target.

Reproducibility matters for the gates (pinned compiler, formatter, analyzers), so the Makefile
targets run inside an `mx-sdk` Docker image by default; the image is built automatically on first
use. Set `MX_RUNNING_IN_DOCKER=1` to run natively with your host toolchain instead. See
`Dockerfile` and `Makefile` for details.

### Build Targets

The CMake options are `MX_CORE_DEV` (test binaries), `MX_API` (the product library, default `ON`),
and `MX_COVERAGE` (gcov instrumentation). The useful workflows are exposed as Makefile targets:

| Target                      | What it does                                                       |
|-----------------------------|--------------------------------------------------------------------|
| `make lib`                  | build the `mx` static library (`mx::api` + `mx::impl`)            |
| `make dev`                  | build `mx` + all test and example binaries                        |
| `make test`                 | run the `mxtest` suite (api/impl/file/control)                    |
| `make test-api-roundtrip`   | corpus api roundtrip in regression mode (CI gate)                 |
| `make examples-run`         | build and run all three api example programs                      |
| `make core-dev`             | build `mx_core` and the corert/unit/validate test binaries        |
| `make test-core-dev`        | run the core roundtrip suite over the `data/` corpus              |
| `make test-cpp-unit`        | run the `mx::core` unit tests (values, shapes, rejection suite)   |
| `make validate-cpp`         | serialize every corpus file and `xmllint`-validate the output     |
| `make probe-cpp`            | compile-time probes: invalid constructions must NOT compile       |

`make clean` removes the build tree. Knobs: `JOBS` (parallelism, auto-detected), `BUILD_TYPE`
(default `Debug`), and `ARGS` (forwarded to test binaries, e.g.
`make test-core-dev ARGS='lysuite/*'`).

### Build Tenets

* `mx` should not depend on any outside libraries (no deps).
* `mx` third-party code should be kept to a minimum.
* `mx` third-party code should be checked into the `mx` repo and compiled as part of the `mx`
  library.
* `mx` should not depend on any package manager, though it may be published into any package
  manager.

### Invalid File Tenets

`mx` is designed such that invalid scalar values are typically supported by clamping the invalid
value to a valid one. For example, in test files I have discovered many cases where MuseScore wrote
-1.11 as a dynamics value. This is invalid to the spec which says

```xml
<xs:attribute name="dynamics" type="non-negative-decimal"/>
```

So in this case, `mx` will load the file and replace -1.11 with 0. Unfortunately this is silent for
now, but we may surface a message system to let the caller know that this has happened.

The `make validate-cpp` gate is the permanent mechanical proof of this tenet: every parsed corpus
document is serialized and the *output* is validated against the MusicXML 4.0 XSD, so clamp
leniency on import can only ever emit schema-valid XML.

### Using `mx` in a Cmake Project

The following script demonstrates how you can start a new cmake project that uses `mx` by committing
its sourcecode into your project.

```sh
#!/bin/bash
set -eou pipefail

# this script demonstrates how to depend on mx by including it in your
# sourcecode tree.

# if given, the first argument is a path to directory where the new
# project will be created.
REPO="${1:-/tmp/$(uuidgen)}"
echo "Creating a new project in: ${REPO}"

# create a new git repository for your project
mkdir -p "${REPO}"
cd "${REPO}"
git init
# bring the mx sourcecode into your project into a temporary location
git clone https://github.com/webern/mx.git mxtemp

# copy only what we need.  all we need is the source code directory, the
# cmake file, the license, and the .gitignore file (helpful since there
# is one generated file.)
mkdir mx
mv mxtemp/src mx/src
mv mxtemp/.gitignore mx/.gitignore
mv mxtemp/LICENSE mx/LICENSE
mv mxtemp/CMakeLists.txt mx/CMakeLists.txt
# we don't need the test code, either
rm -rf mx/src/private/mxtest
rm -rf mxtemp

# commit the mx sourcecode to our project repo
git add --all && git commit -m'mx sourcecode'

# create a main.cpp file
cat <<- "EOF" > main.cpp
#include <iostream>
#include "mx/api/ScoreData.h"
#include "mx/api/DocumentManager.h"

int main () {
    using namespace mx::api;
    ScoreData score{};
    score.workTitle = "Hello World";
    NoteData note{};
    note.durationData.durationName = DurationName::quarter;
    note.pitchData.step = Step::d;
    VoiceData voiceData{};
    voiceData.notes.push_back(note);
    StaffData staff{};
    staff.voices[0] = voiceData;
    MeasureData measure{};
    measure.staves.push_back(staff);
    PartData part{};
    part.measures.push_back(measure);
    score.parts.push_back(part);
    auto& mgr = DocumentManager::getInstance();
    const auto id = mgr.createFromScore(score);
    mgr.writeToStream(id, std::cout);
    mgr.destroyDocument(id);
}
EOF

# create a cmake file
cat <<- "EOF" > CMakeLists.txt
cmake_minimum_required(VERSION 3.17)
project(my-musicxml-proj)
set(CMAKE_CXX_STANDARD 20)

add_executable(my-musicxml-proj main.cpp)
add_subdirectory(mx)
target_link_libraries(my-musicxml-proj mx)
target_include_directories(my-musicxml-proj PRIVATE mx/src/include)
EOF

# create a .gitignore file to ignore a build directory
cat <<- "EOF" > .gitignore
build/
EOF

git add --all && git commit -m'musicxml hello world'

# create a build directory
mkdir build

# build your project
cd build
cmake .. && make -j10
# run your executable
./my-musicxml-proj
```

# Code Generation

`src/private/mx/core/generated/` is generated by the `gen/` code generator; do not edit it
directly. Regenerate with `make gen-cpp`. See `gen/README.md` for an overview and
`gen/AGENTS.md` for commands and gates.

# Using `mx`

## API

The `mx::api` namespace is intended to be a simplified structural representation of MusicXML. It
should be more intuitive than manipulating the DOM representation directly. In particular, voices
and time positions are more explicitly managed. Some complexities, on the other hand, are retained
in `mx::api`, such as the need to manage beam starts and stops explicitly.

#### Writing MusicXML with `mx::api`

```C++
#include <string>
#include <iostream>
#include <cstdint>
#include <sstream>

#include "mx/api/DocumentManager.h"
#include "mx/api/ScoreData.h"

// set this to 1 if you want to see the xml in your console
#define MX_WRITE_THIS_TO_THE_CONSOLE 0

int main(int argc, const char * argv[])
{
    using namespace mx::api;
    const auto qticks = 4;

    // create a score
    auto score = ScoreData{};
    score.workTitle = "Mx Example";
    score.composer = "Matthew James Briggs";
    score.copyright = "Copyright (c) 2019";
    score.ticksPerQuarter = qticks;

    // create a part
    score.parts.emplace_back( PartData{} );
    auto& part = score.parts.back();

    // give the part a name
    part.name = "Flute";
    part.abbreviation = "Fl.";
    part.displayName = "Flute";
    part.displayAbbreviation = "Fl.";

    // give the part an instrument
    part.instrumentData.soundID = SoundID::windFlutesFlute;
    part.instrumentData.midiData.channel = 1;
    part.instrumentData.midiData.program = 74;

    // add a measure
    part.measures.emplace_back( MeasureData{} );
    auto& measure = part.measures.back();
    measure.timeSignature.beats = "4";
    measure.timeSignature.beatType = "4";
    measure.timeSignature.isImplicit = false;

    // add a staff
    measure.staves.emplace_back( StaffData{} );
    auto& staff = measure.staves.back();

    // set the clef
    auto clef = ClefData{};
    clef.setTreble();
    staff.clefs.emplace_back( clef );

    // add a voice
    staff.voices[0] = VoiceData{};
    auto& voice = staff.voices.at( 0 );

    const auto quarter = qticks;
    const auto half = qticks * 2;
    const auto eighth = qticks / 2;

    // add a few notes
    auto currentTime = 0;
    auto note = NoteData{};
    note.pitchData.step = Step::d;
    note.pitchData.alter = 1;
    note.pitchData.octave = 5;
    note.pitchData.accidental = Accidental::sharp;
    note.durationData.durationName = DurationName::half;
    note.durationData.durationTimeTicks = half;
    note.tickTimePosition = currentTime;
    voice.notes.push_back( note );

    // advance our time
    currentTime += half;

    note.pitchData.step = Step::e;
    note.pitchData.alter = 0;
    note.pitchData.octave = 5;
    note.pitchData.accidental = Accidental::none;
    note.durationData.durationName = DurationName::eighth;
    note.durationData.durationTimeTicks = eighth;
    note.tickTimePosition = currentTime;
    // beams are handled explicitly in musicxml
    note.beams.push_back( Beam::begin ); // start an eighth-note beam
    voice.notes.push_back( note );
    currentTime += eighth;

    note.pitchData.step = Step::f;
    note.pitchData.alter = 0;
    note.pitchData.octave = 5;
    note.pitchData.accidental = Accidental::none;
    note.durationData.durationName = DurationName::eighth;
    note.tickTimePosition = currentTime;
    note.durationData.durationTimeTicks = eighth;
    note.beams.clear();
    note.beams.push_back( Beam::end ); // end the eighth-note beam
    voice.notes.push_back( note );
    currentTime += eighth;

    note.pitchData.step = Step::e;
    note.pitchData.alter = 0;
    note.pitchData.octave = 5;
    note.pitchData.accidental = Accidental::none;
    note.durationData.durationName = DurationName::quarter;
    note.durationData.durationTimeTicks = quarter;
    note.tickTimePosition = currentTime;
    note.beams.clear();
    voice.notes.push_back( note );

    // the document manager is the liaison between our score data and the MusicXML DOM.
    // it completely hides the MusicXML DOM from us when using mx::api
    auto& mgr = DocumentManager::getInstance();
    const auto idResult = mgr.createFromScore( score );
    if( !idResult.ok() ) { return 1; }
    const auto documentID = idResult.value();

    // write to the console
    #if MX_WRITE_THIS_TO_THE_CONSOLE
    mgr.writeToStream( documentID, std::cout );
    std::cout << std::endl;
    #endif

    // write to a file. argv[1] overrides the default output path so the build
    // system can send the file to a gitignored location (see issue #150).
    const std::string outputPath = ( argc > 1 ) ? argv[1] : "./example.musicxml";
    const auto writeResult = mgr.writeToFile( documentID, outputPath );

    // we need to explicitly delete the object held by the manager
    mgr.destroyDocument( documentID );

    return writeResult.ok() ? 0 : 1;
}
```

#### Reading MusicXML with `mx::api`

```C++
#include "mx/api/DocumentManager.h"
#include "mx/api/ScoreData.h"

#include <string>
#include <iostream>
#include <cstdint>
#include <sstream>

#define MX_IS_A_SUCCESS 0
#define MX_IS_A_FAILURE 1

constexpr const char* const xml = R"(
<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<!DOCTYPE score-partwise PUBLIC
    "-//Recordare//DTD MusicXML 3.1 Partwise//EN"
    "http://www.musicxml.org/dtds/partwise.dtd">
<score-partwise version="3.1">
  <part-list>
    <score-part id="P1">
      <part-name>Music</part-name>
    </score-part>
  </part-list>
  <part id="P1">
    <measure number="1">
      <attributes>
        <divisions>1</divisions>
        <key>
          <fifths>0</fifths>
        </key>
        <time>
          <beats>4</beats>
          <beat-type>4</beat-type>
        </time>
        <clef>
          <sign>G</sign>
          <line>2</line>
        </clef>
      </attributes>
      <note>
        <pitch>
          <step>C</step>
          <octave>4</octave>
        </pitch>
        <duration>4</duration>
        <type>whole</type>
      </note>
    </measure>
  </part>
</score-partwise>
)";

int main(int argc, const char * argv[])
{
    using namespace mx::api;

    // create a reference to the singleton which holds documents in memory for us
    auto& mgr = DocumentManager::getInstance();

    // place the xml from above into a stream object
    std::istringstream istr{ xml };

    // ask the document manager to parse the xml into memory for us, returns a document ID.
    const auto idResult = mgr.createFromStream( istr );
    if( !idResult.ok() ) { return MX_IS_A_FAILURE; }
    const auto documentID = idResult.value();

    // get the structural representation of the score from the document manager
    const auto scoreResult = mgr.getData( documentID );

    // we need to explicitly destroy the document from memory
    mgr.destroyDocument( documentID );

    if( !scoreResult.ok() ) { return MX_IS_A_FAILURE; }
    const auto& score = scoreResult.value();

    // make sure we have exactly one part
    if( score.parts.size() != 1 )
    {
        return MX_IS_A_FAILURE;
    }

    // drill down into the data structure to retrieve the note
    const auto& part = score.parts.at( 0 );
    const auto& measure = part.measures.at( 0 );
    const auto& staff = measure.staves.at( 0 );
    const auto& voice = staff.voices.at( 0 );
    const auto& note = voice.notes.at( 0 );

    if( note.durationData.durationName != DurationName::whole )
    {
        return MX_IS_A_FAILURE;
    }

    if( note.pitchData.step != Step::c )
    {
        return MX_IS_A_FAILURE;
    }

    return MX_IS_A_SUCCESS;
}
```

# Implementation Details

The MusicXML classes in `mx::core` are tightly bound to the `musicxml.xsd` specification. MusicXML
can be challenging to use and the `mx::core` class structure mirrors the complexity of the MusicXML
specification. A simplified representation is available in `mx::api`. It is possible to work with a
subset of MusicXML using only `mx::api`, without delving into `mx::core`.

##### Namespaces

```C++
using namespace mx::api;    // an easier interface for reading and writing MusicXML
using namespace mx::core;   // a direct representation of a musicxml document in C++ classes
using namespace mx::impl    // the logic that translates between mx::api and mx::core
using namespace mx::utility // a typical catch-all for generic stuff like logging macros
```

##### `mx::api`

The `mx::api` namespace is a set of 'plain old data' structs that represent a simplified model of
MusicXML. For example, here is the `ScoreData.h`, which represents the top level of the object
hierarchy:

```C++
class ScoreData
{
public:
    MusicXmlVersion musicXmlVersion;
    std::string musicXmlType;
    std::string workTitle;
    std::string workNumber;
    std::string movementTitle;
    std::string movementNumber;
    std::string composer;
    std::string lyricist;
    std::string arranger;
    std::string publisher;
    std::string copyright;
    EncodingData encoding;
    std::vector<PageTextData> pageTextItems;
    DefaultsData defaults;
    std::vector<PartData> parts;
    std::vector<PartGroupData> partGroups;
    int ticksPerQuarter;
    std::map<MeasureIndex, LayoutData> layout;
};
```

`mx::api` and `mx::core` are kept completely separate.\
That is, `mx::api` data is serialized into `mx::core` data, which is then serialized into MusicXML.
The `mx::api` structs allow us to simplify things like specifying a note's tick time position, and
allowing the serialization process to take care of details such as `<forward>` `<backup>` elements.

##### `mx::core`

The `mx::core` namespace contains the MusicXML representation objects such as elements and
attributes. `mx::core` is generated from the MusicXML 4.0 XSD by the generator in `gen/` (the
hand-written runtime lives in `src/private/mx/core/`, the generated model in
`src/private/mx/core/generated/` -- the directory is the generated/hand-written boundary).

The generated model is valid-by-construction: enum wrappers whose named factories are the only
constructors, clamp-on-construct number wrappers, composites with named fields in schema order
(the serializer walks declaration order, so wrong element order is unrepresentable),
`std::variant`-based choice classes, `OneOrMore<T>` for required repeats, and `Result`-returning
bounded appends (e.g. beam <= 8). Value semantics throughout -- no shared pointers. Errors exist
in exactly two places: `mx::core::parse(const pugi::xml_document&) -> Result<Document>` (strict on
names and structure, lenient on values) and the bounded `add...` methods. Serialization is
`mx::core::serialize(const Document&, pugi::xml_document&)`. The design and its rationale are
recorded in `docs/ai/design/mx-core-plan.md`.

`mx::core` consumes the vendored [pugixml](http://pugixml.org/) (`src/private/pugixml/`) directly,
in keeping with the build tenets [above](#build-tenets). The earlier `ezxml` abstraction layer is
retired.

##### `mx::impl`

`mx::impl` is the translation layer between `mx::api` and `mx::core`.

##### `mx::utility`

This namespace is small. It mostly contains macros and small, generic functions.

##### Partwise vs. Timewise

There are two types of MusicXML documents, `partwise` and `timewise`. A partwise document consists
of a set of parts which contain measures. A timewise document consists of a set of measures which
contain parts. Partwise is used more often by MusicXML applications while Timewise documents seem to
be rare or even nonexistent. Nonetheless *MusicXML Class Library* implements both Timewise and
Partwise. The class `mx::core::Document` holds *either* a Partwise *or* a Timewise score: its root
is a `std::variant<ScorePartwise, ScoreTimewise>` -- exactly one root by construction. You can
check the inner document type with `isScorePartwise()` / `isScoreTimewise()`.

### Unit Test Framework

The C++ test binaries (`mxtest-core`, `mxtest-core-dev`) use the Catch test framework via the
vendored runner in `src/private/cpul/`. The primary correctness gate is the core roundtrip
(corert) suite, which round-trips every eligible file in `data/` through the typed model and
compares the result against a normalized form of the input; the same suite is implemented for the
Go and C test targets. See `AGENTS.md` for the full gate list.
