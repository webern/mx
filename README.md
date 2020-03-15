MusicXML Class Library
======================

- Author: Matthew James Briggs
- License: MIT
- Version: 1.0
- Supported MusicXML Version: 3.0
- Language: C++14

-----------------------------------------

[![CircleCI](https://circleci.com/gh/webern/mx.svg?style=svg)](https://circleci.com/gh/webern/mx)

## Introduction

This project is a C++ library for working with MusicXML files.  MusicXML files are represented as statically typed objects.  This structure can be serialized and deserialize to/from MusicXML.

## Compiling

The project is tested with the following:
* Clang (Xcode 10 and higher)
* g++ (Debian 6.3.0-18+deb9u1) 6.3.0 20170516
* cmake version 3.7.2
* MSVC2019

There are three cmake options:

```
    -DMX_BUILD_TESTS=on
    -DMX_BUILD_CORE_TESTS=off
    -DMX_BUILD_EXAMPLES=on
```

The configuration shown above is the recommended configuration for development. If you just need the lib then turn off all three of the cmake options.

Here's an example of a clean build of the project followed by a test run.

```
git clone https://github.com/webern/mx.git mx
mkdir build
cd build
cmake ../mx -DMX_BUILD_TESTS=on -DMX_BUILD_CORE_TESTS=off -DMX_BUILD_EXAMPLES=on
make -j6
./MxTest
```

The Xcode project (checked-in to the repo) has targets for iOS and macOS frameworks and dylibs.  These are not specified in the cmake file.

## API

The `mx::api` namespace is intended to be a simplified structural representation of MusicXML. It should be slightly more intuitive than manipulating the DOM representation directly. In particular, voices and time positions are more explicitly managed.

#### Writing MusicXML with `mx::api`

```C++
#include <string>
#include <iostream>
#include <cstdint>
#include <sstream>

#include "mx/api/DocumentManager.h"
#include "mx/api/ScoreData.h"

// set this to 1 if you want to see the xml in your console
#define MX_WRITE_THIS_TO_THE_CONSOLE 1

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
    measure.timeSignature.beats = 4;
    measure.timeSignature.beatType = 4;
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
    const auto documentID = mgr.createFromScore( score );

    // write to the console
    #if MX_WRITE_THIS_TO_THE_CONSOLE
    mgr.writeToStream( documentID, std::cout );
    std::cout << std::endl;
    #endif

    // write to a file
    mgr.writeToFile( documentID, "./example.musicxml" );

    return 0;
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
    const auto documentID = mgr.createFromStream( istr );

    // get the structural representation of the score from the document manager
    const auto score = mgr.getData( documentID );

    // we need to explicitly destroy the document from memory
    mgr.destroyDocument(documentID);

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

## Core Implementation Details

The MusicXML classes in `mx::core` are tightly bound to the `musicxml.xsd` specification.  MusicXML can be challenging to use and the `mx::core` class structure mirrors the complexity of the MusicXML specification.  A slightly simplified representation is available in `mx::api`. It is possible to work with a subset of MusicXML using only `mx::api`, without delving into `mx::core`.

##### Namespaces

```
using namespace ezxml;
using namespace mx::api;
using namespace mx::core;
```

The `mx::core` namespace contains the MusicXML representation objects such as elements and attributes.  In the musicxml.xsd there are many cases of 'xs:choice' or 'xs:group' being used.  These constructs are typically represented in the mx::core class structure the same way that they are found in the musicxml.xsd specification.  The interfaces in this namespace are relatively stable, however they are tightly bound to MusicXML's specification and thus they will change when it comes time to support a future version of MusicXML.

The `::ezxml::` namespace contains generic XML DOM functionality.  Under the hood [pugixml](http://pugixml.org/) is being used.  See the XML DOM section for more information.

##### Partwise vs. Timewise
There are two types of MusicXML documents, 'partwise' and 'timewise'.  A partwise document consists of a set of parts which contain measures.  A timewise document consists of a set of measures which contain parts.  Partwise is used more often by MusicXML applications while Timewise documents seem to be rare or even nonresistant.  Nonetheless *MusicXML Class Library* implements both Timewise and Partwise.  The class `mx::core::Document` can hold *either* a Partwise *or* a Timewise score.  Note that is actually holds both, but only one or the other is 'active'.  You can check the inner document type with the getChoice function.  You can convert between Partwise and Timewise with the convertContents function.

##### Elements
Each XML element is represented by a class which derives from ElementInterface.  Elements are created and used by way of shared pointers.  Each element comes with a set of using/typedef statements as well as a convenience function for making the shared pointers.

##### Shared Pointers
Many elements contain other elements.  When they do, these data members will also be shared pointers.  Get/set functions will allow access to the data members by accepting and returning shared pointers.  If you attempt to set a data member to a nullptr, the setter function will silently do nothing.  Thus we can be reasonably assured our objects will never return nullptr.

For example

```
std::shared_ptr<Foo> foo; /* nullptr! */
bar->setFoo( foo ); /* no-op because you passed a nullptr */
auto x = bar->getFoo(); /* guaranteed not to be null */
x->somefuntion(); /* OK to dereference without checking for nullptr */
```

##### Optional Member Data
Many of the elements in MusicXML are optional.  In these cases there is a bool which indicates whether or not the element is present.  The bool serves as a flag indicating whether or not the optional element will be output when you stream out your MusicXML document.  The bool has no side-effect on the element who's presence/absence it represents.  So for example we may set some data:

```
foo->setValue( "hello" );
bar->setFoo( foo );
```

But in this example, if Foo is an optional member of Bar, then we must also set hasFoo to *true* or else foo will not be in the XML output.

```
bar->toStream(...); /* Foo is not in the output! */
bar->setHasFoo( true );
bar->toStream(...); /* Now we see <foo>hello</foo> in the output. */
```

Also note that setting HasFoo to *false* does not mean that Foo's value is gone.

```
bar->setHasFoo( false ); /* The XML document no longer has a Foo */
bar->getFoo()->getValue() == "hello"; /* True! The value still exists but is not present in the XML. */
```

##### Optional Member Data with Unbounded Occurrences
Sometimes an element may contain zero, one, or many occurrences of another element.  For example

```
<xs:element name="key" type="key" minOccurs="0" maxOccurs="unbounded">
```

In this case there will be a collection of Key objects and the getter/setters will look like this, where `KeySet` is a typedef of `std::vector<Key>`.

```
const KeySet& getKeySet() const;
void addKey( const KeyPtr& value );
void removeKey( const KeySetIterConst& value );
void clearKeySet();
KeyPtr getKey( const KeySetIterConst& setIterator ) const;
```

##### Required Member Data with Unbounded Occurrences
Sometimes an element is required, but you may optionally have more than one.  For example

```
<xs:element name="direction-type" type="direction-type" maxOccurs="unbounded"/>
```

In this case, minOccurs="1" (by default per XSD language rules).  The functions will look just like the previous example, but they will behave differently

```
const DirectionTypeSet& getDirectionTypeSet() const;
void addDirectionType( const DirectionTypePtr& value );
void removeDirectionType( const DirectionTypeSetIterConst& value );
void clearDirectionTypeSet();
DirectionTypePtr getDirectionType( const DirectionTypeSetIterConst& setIterator ) const;
```

When the containing element is constructed, a single DirectionType will be default constructed and pushed onto the vector.  Thus you will have one default constructed DirectionType in the set upon construction.

If you try to call removeDirectionType with only one DirectionType in the set (size==1) nothing will happen.  You will still have a single DirectionType in the collection.

When you call clearDirectionTypeSet vector.clear() will be called but it will follow up by pushing a default constructed DirectionType onto the vector so you will still have size==1.

As it turns out, this design choice tends to be annoying in practice.  On the upside, it does guarantee that your MusicXML document will be valid, even if you forget to add a required element.  The downside is that it means you have to deal with the fact that a default constructed element always exists in the set, so you must replace or remove the first element.  Furthermore, you cannot remove the existing element until another one has been added.  Here are the two patterns I have used for this (pseudocode).

**Pattern 1:** Replace the first element by dereferencing the begin() iterator:

```
bool isFirstAdded = false;
for( auto stuffElement : stuffElementsIWantToAdd )
{
    if( !isFirstAdded )
    {
        *( myElementIWantToAddThemTo->getStuffSet().begin() ) = stuffElement;
        isFirstAdded = true;
    }
    else
    {
        myElementIWantToAddThemTo->addStuff( stuffElement );
    }
}
```

**Pattern 2:** Remove the default element *After* adding a replacement:

```
bool isFirstAdded = false;
for( auto stuffElement : stuffIWantToAdd )
{
    myElementIWantToAddThemTo->addStuff( stuffElement );
    if( !isFirstAdded )
    {
        myElementIWantToAddThemTo->removeStuff( myElementIWantToAddThemTo->getStuffSet().cbegin() )
        isFirstAdded = true;
    }
}
```

Pattern 1 always works, even if you're not sure whether or not the `minOccurs="1"` or `"0"`.  Pattern 2 only works when `minOccurs="1"`.  There are no cases where `minOccurs` is greater than 1.

##### Member Data with Bounded maxOccurs
```
<xs:element name="beam" type="beam" minOccurs="0" maxOccurs="8"/>
```
In this case if you call addBeam when there are already 8 beams in the vector, nothing will happen.

##### xs:groups
For an xs:group there is usually a single 'element' class which represents the group of elements.  For example this XSD snippet:

```
<xs:group name="editorial">
	<xs:sequence>
		<xs:group ref="footnote" minOccurs="0"/>
		<xs:group ref="level" minOccurs="0"/>
	</xs:sequence>
</xs:group>
```

is represented by this class:

```
class EditorialGroup : public ElementInterface
{
public:
    EditorialGroup();

    /* ... other stuff ... */

    /* _________ Footnote minOccurs = 0, maxOccurs = 1 _________ */
    FootnotePtr getFootnote() const;
    void setFootnote( const FootnotePtr& value );
    bool getHasFootnote() const;
    void setHasFootnote( const bool value );

    /* _________ Level minOccurs = 0, maxOccurs = 1 _________ */
    LevelPtr getLevel() const;
    void setLevel( const LevelPtr& value );
    bool getHasLevel() const;
    void setHasLevel( const bool value );
    
    bool fromXElement( std::ostream& message, xml::XElement& xelement );

private:
    FootnotePtr myFootnote;
    bool myHasFootnote;
    LevelPtr myLevel;
    bool myHasLevel;
};
```

##### xs:choices

There are a few exceptions (mistakes) but for the most part, `xs:choice` constructs are represented by a class with a name ending in 'Choice'.  The element will have an enum named 'Choice' in the public scope of the class.  Each of the possible 'choices' will exist as data members of the class, but only one of them will be 'active' (was present in, or will be written to, XML).  For example, this xsd construct:

```
<xs:choice minOccurs="0">
    <xs:element name="pre-bend" type="empty">
        <xs:annotation>
            <xs:documentation>The pre-bend element indicates that this is a pre-bend rather than a normal bend or a release.</xs:documentation>
        </xs:annotation>
    </xs:element>
    <xs:element name="release" type="empty">
        <xs:annotation>
            <xs:documentation>The release element indicates that this is a release rather than a normal bend or pre-bend.</xs:documentation>
        </xs:annotation>
    </xs:element>
</xs:choice>
```

Is represented by this class:

```
class BendChoice : public ElementInterface
{
public:
    enum class Choice
    {
        preBend = 1,
        release = 2
    };
    BendChoice();

	/* ... other stuff ... */
    
    BendChoice::Choice getChoice() const;
    void setChoice( BendChoice::Choice value );

    /* _________ PreBend minOccurs = 1, maxOccurs = 1 _________ */
    PreBendPtr getPreBend() const;
    void setPreBend( const PreBendPtr& value );

    /* _________ Release minOccurs = 1, maxOccurs = 1 _________ */
    ReleasePtr getRelease() const;
    void setRelease( const ReleasePtr& value );

    bool fromXElement( std::ostream& message, xml::XElement& xelement );

private:
    Choice myChoice;
    PreBendPtr myPreBend;
    ReleasePtr myRelease;
};
```

When `getChoice() == BendChoice::Choice::preBend` then we will see `<pre-bend/>` in the XML, but when `getChoice() == BendChoice::Choice::postBend` then we will see `<post-bend/>` in the XML.

### XML DOM (::ezxml::)

Any XML document can be read and manipulated with the classes in the `::ezxml::` namespace.  Most notably, look at the following pure virtual interfaces XDoc, XElement, XAttribute.  Also look at the STL-compliant iterators XElementIterator and XAttributeIterator.

These interfaces are designed to wrap any underlying XML DOM software so that `mx::core` does not care or know about the XML DOM code.  A set of implementation classes wrapping pugixml are provided, but if you need to use, say Xerces or RapidXML, you can look at the PugiElement, PugiDoc, etc classes and wrap whatever library you need.

Here's how you can read a MusicXML document into `mx::core` classes by way of `::ezxml::`.

```
#include "mx/core/Document.h"
#include "mx/utility/Utility.h"
#include "functions.h"
#include "ezxml/XFactory.h"
#include "ezxml/XDoc.h"

#include <iostream>
#include <string>
#include <sstream>

int main(int argc, const char *argv[])
{
    // allocate the objects
    mx::core::DocumentPtr mxDoc = makeDocument();
    ::ezxml::::XDocPtr xmlDoc = ::ezxml::::XFactory::makeXDoc();
    
    // read a MusicXML file into the XML DOM structure
    xmlDoc->loadFile( "music.xml" );

    // create an ostream to receive any parsing messages
    std::stringstream parseMessages;
    
    // convert the XML DOM into MusicXML Classes
    bool isSuccess = mxDoc->fromXDoc( parseMessages, *xmlDoc );
    
    if( !isSuccess )
    {
        std::cout << "Parsing of the MusicXML document failed with the following message(s):" << std::endl;
        std::cout << parseMessages.str() << std::endl;
        return -1;
    }
    
    // maybe the document was timewise document. if so, convert it to partwise
    if( mxDoc->getChoice() == mx::core::DocumentChoice::timewise )
    {
        mxDoc->convertContents();
    }
    
    // get the root
    auto scorePartwise = mxDoc->getScorePartwise();
    
    // change the title
    scorePartwise->getScoreHeaderGroup()->setHasWork( true );
    scorePartwise->getScoreHeaderGroup()->getWork()->setHasWorkTitle( true );
    scorePartwise->getScoreHeaderGroup()->getWork()->getWorkTitle()->setValue( mx::core::XsString( "New Title" ) );
    
    // write it back out to disk
    mxDoc->toXDoc( *xmlDoc );
    xmlDoc->write( "newtitle.xml" );
    
    return 0;
}
```

### Hello World using mx::core
On the MusicXML home page there is an example of a "Hello World" simple MusicXML file.  Here is a main function that would output this "Hello World" MusicXML example to std::cout.

```
#include <iostream>
#include "DocumentPartwise.h"
#include "Elements.h"

using namespace mx::core;
using namespace std;

int main(int argc, const char * argv[])
{
    auto doc = makeDocumentPartwise();
    auto s = doc->getScorePartwise();
    s->getAttributes()->hasVersion = true;
    s->getAttributes()->version = XsToken( "3.0" );
    auto header = s->getScoreHeaderGroup();
    header->getPartList()->getScorePart()->getAttributes()->id = XsID( "P1" );
    header->getPartList()->getScorePart()->getPartName()->setValue( XsString( "Music" ) );
    auto part = *( s->getPartwisePartSet().cbegin() );
    part->getAttributes()->id = XsIDREF( "P1" );
    auto measure = *( part->getPartwiseMeasureSet().cbegin() );
    measure->getAttributes()->number = XsToken( "1" );
    auto propertiesChoice = makeMusicDataChoice();
    propertiesChoice->setChoice( MusicDataChoice::Choice::properties );
    auto properties = propertiesChoice->getProperties();
    properties->setHasDivisions( true );
    properties->getDivisions()->setValue( PositiveDivisionsValue( 1 ) );
    properties->addKey( makeKey() );
    auto time = makeTime();
    time->getTimeChoice()->setChoice( TimeChoice::Choice::timeSignature );
    time->getTimeChoice()->getTimeSignature()->getBeats()->setValue( XsString( "4" ) );
    time->getTimeChoice()->getTimeSignature()->getBeatType()->setValue( XsString( "4" ) );
    properties->addTime( time );
    auto clef = makeClef();
    clef->getSign()->setValue( ClefSign::g );
    clef->setHasLine( true );
    clef->getLine()->setValue( StaffLine( 2 ) );
    properties->addClef( clef );
    measure->getMusicDataGroup()->addMusicDataChoice( propertiesChoice );
    auto noteData = makeMusicDataChoice();
    noteData->setChoice( MusicDataChoice::Choice::note );
    noteData->getNote()->getNoteChoice()->setChoice( NoteChoice::Choice::normal );
    noteData->getNote()->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->getFullNoteTypeChoice()->setChoice( FullNoteTypeChoice::Choice::pitch );
    noteData->getNote()->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->getFullNoteTypeChoice()->getPitch()->getStep()->setValue( StepEnum::c );
    noteData->getNote()->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->getFullNoteTypeChoice()->getPitch()->getOctave()->setValue( OctaveValue( 4 ) );
    noteData->getNote()->getNoteChoice()->getNormalNoteGroup()->getDuration()->setValue( PositiveDivisionsValue( 4 ) );
    noteData->getNote()->getType()->setValue( NoteTypeValue::whole );
    measure->getMusicDataGroup()->addMusicDataChoice( noteData );
    
    doc->toStream( cout ); /* print Hello World MusicXML document to console */
    return 0;
}
```

### Unit Test Framework

An executable program named MxTest is also included in the project.  MxTest utilizes the CppUnitLite macro library by Michael Feathers.  Licensing of this library is not clear, [here is a link](http://c2.com/cgi/wiki?CppUnitLite) to the source of this library.
CppUnitLite appears to be abandonware, but it is very useful.
Here are some additional CppUnitLite links
  * [http://www.objectmentor.com/resources/downloads.html](http://www.objectmentor.com/resources/downloads.html)
  * [https://github.com/smikes/CppUnitLite](https://github.com/smikes/CppUnitLite)
  * [https://github.com/webern/CppUnitLite](https://github.com/webern/CppUnitLite)

The tests are slow to compile, see the *Compiling* section for more info on how to skip compilation of the tests.

#### Release Notes

  * **Version 1.0** April 2019: A more-or-less stable version of `mx::api`
  * **Version 0.3** Early 2019
  * **Version 0.2** August 21, 2016 Adds the ability to import MusicXML documents into MusicXML Classes.

#### Historical Notes

**Historical Note: April 7, 2019** Trying to button up a '1.0' 'release' by tying up some of the loose ends with the build, continuous integration, testing, and header leakage. The use of the semver `1.0` implies that we hope there won't be many breaking changes. I suppose the way I will use semver is as follows: a patch release may cause slight compilation annoyances, but nothing that would cause any rewriting of code. A minor release may similarly require some tweaks but hopefully nothing too major. A major release could be anything up-to a complete re-write.

**Historical Note: October 6, 2016:** Significant progress has been made on the `api` namespace, which is a simplified set of data structures to represent a MusicXML document.  These are being implemented as mostly-POD structs.  Currently the importing of data into these structures is well-underway, but the exporting from these data structures has not been implemented.

**Historical Note: August 16, 2016:** All tests are passing (core, xml and import).  The remaining items to do on the ximport feature are
- search for all `\\TODO's` and fix those that can be fixed
- Standardize the code file copyrights and bump to version 0.2
- Rename XElement functions to better names
- Update the readme including information about the XDoc XElement classes and how to implement those interfaces.

**Historical Note: August 11, 2016:** The feature to import MusicXML files is essentially complete with most "round-trip" integration tests passing.

**Historical Note: June 27, 2016:** An "integration" tester has been added to the MxTest executable.  This exists in ImportTest.cpp, and ImportTestImpl.h/cpp.  A top level directory "Resources" has been added to hold test input (i.e. "golden") files.  The input files are gathered from Recordare, Lilypond and MuseScore and to this I have added a few of my own scores as exported by Finale Dolet.

Each of these test input files has been "scrubbed" using the XDoc classes (i.e. it has been round-tripped through pugixml and has been updated to a normalized MusicXML 3.0 header format.  The resultant scrubbed files are in Resources/expected.  During the test run, a csv file is written in Resources/testOutput recording a row for each test (Pass/Fail, duration of test, messages, etc).  Each time a test failure is encountered the expected file and the error file will be saved to the Resources/testOutput directory to allow for visual inspection.

Currently this tester is a "wire-up".  All 263 of these round-trip import/export tests fail because the implementation does not yet exist in mx::core.  The next body of work will be the mx::core implementation.

**Historical Note: June 20, 2016:** A simple interface to for XML DOM has been added in the ::ezxml:: namespace.  The key classes (pure virtual) are XDoc, XElement, XAttribute, XElementIterator, XAttributeIterator.  These are implemented by concrete classes PugiDoc, PugiElement, etc. which serve as a wrapper for the pugixml library (http://pugixml.org/).  Although this is a static library, a class XFactory can be used to create a Pugi instance of the XDoc interface.

The idea behind using a pure virtual interface is that the client of the Music XML Class Library can, in theory choose a different XML DOM library (Xerces, TinyXML, etc) and wrap with instances of the XDoc interfaces and the Music XML core classes will not know the difference.

**Historical Note: June 20, 2016:** The GitHub repo was bloated and has been entirely wiped-out and restarted.
