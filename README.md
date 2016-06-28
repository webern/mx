MusicXML Class Library
======================

Author: Matthew James Briggs

-----------------------------------------
Update: June 27, 2016: An "integration" tester has been added to the MxTest executable.  This exists in ImportTest.cpp, and ImportTestImpl.h/cpp.  A top level directory "Resources" has been added to hold test input (i.e. "golden") files.  The input files are gathered from Recordare, Lilypond and MuseScore and to this I have added a few of my own scores as exported by Finale Dolet.

Each of these test input files has been "scrubbed" using the XDoc classes (i.e. it has been round-tripped through pugixml and has been updated to a normalized MusicXML 3.0 header format.  The resultant scrubbed files are in Resources/expected.  During the test run, a csv file is written in Resources/testOutput recording a row for each test (Pass/Fail, duration of test, messages, etc).  Each time a test failure is encountered the expected file and the error file will be saved to the Resources/testOutput directory to allow for visual inspection.

Currently this tester is a "write-up".  All 263 of these round-trip import/export tests fail because the implementation does not yet exist in mx::core.  The next body of work will be the mx::core implementation.

-----------------------------------------

Update: June 20, 2016: A simple interface to for XML DOM has been added in the mx::xml namespace.  The key classes (pure virtual) are XDoc, XElement, XAttribute, XElementIterator, XAttributeIterator.  These are implemented by concrete classes PugiDoc, PugiElement, etc. which serve as a wrapper for the pugixml library (http://pugixml.org/).  Although this is a static library, a class XFactory can be used to create a Pugi instance of the XDoc interface.

The idea behind using a pure virtual interface is that the client of the Music XML Class Library can, in theory choose a different XML DOM library (Xerces, TinyXML, etc) and wrap with instances of the XDoc interfaces and the Music XML core classes will not know the difference.

-----------------------------------------
Old Readme Text...

Changes have been made since the readme doc below was written.  Development is currently underway for MusicXML import.  Readme and documentation needs to be updated as soon as the import feature is complete.

Also the entire GitHub repo has been wiped out and restarted on June 20, 2016.


-----------------------------------------
Datestamp: May 31, 2015

### Introduction
This project is a static C++ class library which represents MusicXML in object-oriented classes.  The scope of the project is intended to be limited to the representation of and manipulation of MusicXML data through an interface that is very closely aligned with the musicxml.xsd document (and thus the syntax for using this library is as verbose as the MusicXML language itself).

One of the goals of this project is to strongly-type all aspects of the MusicXML XSD specification such that **any program that compiles will represent a valid MusicXML document.**  This principle is only extended as far as XSD validation is concerned.  The MusicXML specification allows for the creation of valid MusicXML documents which are nonetheless musical gibberish.  Additionally the MusicXML specification has many requirements which are described in comments in the XSD specification.  In other words, an XSD-validated MusicXML document does not necessarily represent valid MusicXML.

Note: XsIDREF and XsID constraints are not enforced by this library.  Thus the goal stated above has not entirely been achieved.  If you introduce duplicative XsID values, for example, an XSD validation of your document will fail.  I don't know how to solve this problem right now.

### Compiling
The project uses C++11 semantics, does not use any third-party libraries and does not employ any platform-specific commands.  It has been built with the following compilers successfully.
* Xcode 5.1.1 (5B1008) Apple LLVM version 5.1 (clang-503.0.40) (based on LLVM 3.4svn)
* Visual Studio 2013 (note use /bigobj flag due to the large size of the Elements.h/cpp translation unit)
* GCC Version 4.8

Currently the project will remain as an Xcode workspace as I have not yet had the time to explore using CMake to make the repository IDE-agnostic.  It is a simple build though and shouldn't be hard to bring it into another IDE.  The MxTest project must link to libMx.a (or Mx.lib) and must be able to find the headers from the Mx library.

### Usage
If you are not familiar with MusicXML then it will be difficult to use this library.  However one of the aims of this library is to make proper MusicXML construction "discoverable" in the sense that IDE code-completion will show you what can and cannot be done.

##### Namespaces
Everything is contained in the *mx* namespace.  Within the *mx* namespace there are three sub namespaces.  These have names a single character in length for typing convenience.  The *t* namespace contains value types (t for types).  These are things like ranged integers, enums, strings, etc.  The *e* namespace contains all of the XML element classes (e for elements).  And the *d* namespace contains a few classes for representing the MusicXML document as a whole (d for document).  There are no name clashes between the three namespaces so it is OK to do this
```
using namespace mx::e;
using namespace mx::d;
using namespace mx::t;
```

##### Partwise vs. Timewise
There are two types of MusicXML documents, "partwise" and "timewise".  A partwise document consists of a set of parts which contain measures.  A timewise document consists of a set of measures which contain parts.  Partwise is used more often by MusicXML applications while Timewise documents seem to be rare or even nonresistant.  Nonetheless *MusicXML Class Library* implements both Timewise and Partwise.  The classes DocumentPartwise and DocumentTimewise both include a static function *convert* which converts between the two types.  Note that this conversion creates shallow pointer copies of most of the data, so the original and converted documents will point to the same objects in memory.

##### Elements
Each XML element is represented by a class which derives from ElementInterface.  Elements are created and used by way of shared pointers.  Each element comes with a set of using/typedef statements as well as a convenience function for making the shared pointers.

##### Shared Pointers
Many elements contain other elements.  When they do, these data members will also be shared pointers.  Get/set functions will allow access to the data members by accepting and returning shared pointers.  If you attempt to set a data member to a nullptr, the setter function will silently do nothing.  Thus we can be reasonably assured our objects will never return nullptr.

For example

```
std::shared_ptr<Foo> foo; /* nullptr! */
bar->setFoo( foo ); /* no-op because you passed a nullptr */
auto x = bar->getFoo(); /* guaranteed not to bee null */
x->somefuntion(); /* OK to dereference without checking for nullptr */
```

##### Optional Member Data
Many of the elements in MusicXML are optional.  In these cases there is a bool which indicates whether or not the element is present.  The bool serves as a flag indicating whether or not the optional element will be output when you stream out your MusicXML document.  The bool has no side-effect on the element who's presence/absence it represents.  So for example we may set some data:
```
foo->setName( "David" );
bar->setFoo( foo );
```
But in this example, if Foo is an optional member of Bar, then we must also set HasFoo to *true* or else foo will not be in the XML output.
```
bar->toStream(...); /* Foo is not in the output! */
bar->setHasFoo( true );
bar->toStream(...); /* Now we see our Foo in the output. */
```
Also note that setting HasFoo to *false* does not mean that Foo's value is gone.
```
bar->setHasFoo( false ); /* The XML document no longer has a Foo */
bar->getFoo()->getName() == "Dave"; /* True! The value is still there even though it is not present in the XML document. */
```
##### Optional Member Data with Unbounded Occurrences
Sometimes an element may contain zero, one, or many occurrences of another element.  For example
```
<xs:element name="key" type="key" minOccurs="0" maxOccurs="unbounded">
```
In this case there will be a collection of Keys and the getter/setters will look like this, where `KeySet` is a typedef of `std::vector<Key>`.
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


##### Member Data with Bounded maxOccurs
```
<xs:element name="beam" type="beam" minOccurs="0" maxOccurs="8"/>
```
In this case if you call addBeam when there are already 8 beams in the vector, nothing will happen.

##### Handling of xs:choice Specifications

##### Handling of xs:group Specifications

##### Sample Program
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

### Unit Testing

An executable program named MxTest is also included in the project.  MxTest utilizes the CppUnitLite macro library by Michael Feathers.  Licensing of this library is not clear, here is a link to the source of this library http://c2.com/cgi/wiki?CppUnitLite
CppUnitLite appears to be abandonware, but it is very useful.
Here are some additional CppUnitLite links
http://www.objectmentor.com/resources/downloads.html
https://github.com/smikes/CppUnitLite

*MusicXML Class Library* unit testing is not comprehensive, but it is extensive.  I believe every class has at least some unit tests and most functions have at least one test.  The philosophy is lightweight TDD.  The tests are not necessarily written first, but a function/class/feature is not considered complete until unit tests have been written.

### Known Issues and Todo List
- There is no easy way to "deep copy" anything.  Each element and attributes class needs a "clone" function.  Alternatively each makeSomething() could be overloaded to create a clone.
- XsID uniqueness is not constrained...  it should be.
- XsIDREF's, especially in the ScorePartList are not constrained to relate to the PartList XsID's.  Thus it is possible to create an unparsable MusicXML document.  A feature should be added to enforce this one-to-one constraint.
- A (substantial) new feature should be added to represent and interact with the MusicXML document by way of a standard XML DOM structure.  This XML DOM structure should be presented via pure virtual interfaces so that users can wrap their favorite XML DOM program (Xerces, TinyXML, etc) with the *MusicXML Class Library* DOM for easy integration.
- class Encoding in Elements.h is implemented incorrectly, it should allow for an unbounded number of Encoding Choices.