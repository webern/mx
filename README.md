MusicXML Class Library
======================

Author: Matthew James Briggs
License: MIT
Version: 0.2
Datestamp: August 17, 2016
Supported MusicXML Version: 3.0

-----------------------------------------

## Introduction
This project is a static C++ class library for working with MusicXML files.  The MusicXML file is represented in a strongly-typed object-oriented class structure.  MusicXML can be deserialized into the class structure and serialized from class structure to MusicXML.

One of the goals of this project is to strongly-type all aspects of the MusicXML XSD specification such that **any program that compiles will represent a valid MusicXML document.**  This principle is only extended as far as XSD validation is concerned.  The MusicXML specification allows for the creation of valid MusicXML documents which are nonetheless musical gibberish.  Additionally the MusicXML specification has many requirements which are described in comments in the XSD specification.  In other words, an XSD-validated MusicXML document does not necessarily represent valid MusicXML.

Note: XsIDREF and XsID constraints are not enforced by this library.  Thus the goal stated above has not entirely been achieved.  If you introduce duplicative XsID values, for example, an XSD validation of your document will fail and most implementors will throw when reading your MusicXML file.  I don't know how to solve this problem right now.

## Compiling
The project has been built with the following compilers successfully.
* Xcode 7.3.1 Apple LLVM version 7.3.0 (clang-703.0.31)
* Visual Studio 2015

GCC is not working because GCC incorrectly implements the vector erase function with no overload that takes a const iterator.  If you get it to compile with GCC please create a pull request.  [Issue #6](https://github.com/Webern/MusicXML-Class-Library/issues/6) exists for the GCC problem.  Here are the [GCC bugs](https://gcc.gnu.org/bugzilla/buglist.cgi?bug_id=55675%2C57158).

To compile with Xcode, just open `YourRepoRoot/Xcode/mx.xcworkspace` and build.

To compile with any other system, use cmake, here is a Visual Studio example

```
git clone https://github.com/Webern/MusicXML-Class-Library.git MxRepo
mkdir MxBuild
cmake ..\MxRepo
start Mx.sln
```

Visual Studio will open with a solution file and be ready to compile.

**Warning: The Tests Take Forever to Compile**: Both the Xcode workspace and the cmake project include compilation of the unit and integ tests.  These take forever to compile.  If you just want the Mx library and don't want the tests then you can either skip building the test project or you can comment out a few lines to prevent the test code from compiling.

`YourRepoRoot/Sourcecode/mxtest/control/CompileControl.h`

```
// Comment out these defines below to
// prevent tests from compiling.  The
// core tests take a long time to compile.

//#define MX_COMPILE_CORE_TESTS
//#define MX_COMPILE_IMPORT_TESTS
//#define MX_COMPILE_UTILTIY_TESTS
//#define MX_COMPILE_XML_TESTS
```

## Usage
The MusicXML classes are tightly bound to the musicxml.xsd specification.  MusicXML can be challenging to use and the mx class structure mirrors the complexity of the MusicXML specification.  A facade or api for simplifying the interactions with MusicXML documents is planned for future development.

##### Namespaces

```
using namespace mx::core;
using namespace mx::xml;
using namespace mx::utility;
```

The `mx::core` namespace contains the MusicXML representation objects such as elements and attributes.  In the musicxml.xsd there are many cases of 'xs:choice' or 'xs:group' being used.  These constructs are typically represented in the mx::core class structure the same way that they are found in the musicxml.xsd specification.  The interfaces in this namespace are relatively stable, however they are tightly bound to MusicXML's specification and thus they will change when it comes time to support a future version of MusicXML.

The `mx::xml` namespace contains generic XML DOM functionality.  Under the hood [pugixml](http://pugixml.org/) is being used.  See the XML DOM section for more information.

The `mx::utility` namespace contains the beginnings of an api for simplifying the interactions with MusicXML.  This namespace and its interfaces will change completely and should be avoided for now.

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

### XML DOM (mx::xml)

Any XML document can be read and manipulated with the classes in the `mx::xml` namespace.  Most notably, look at the following pure virtual interfaces XDoc, XElement, XAttribute.  Also look at the STL-compliant iterators XElementIterator and XAttributeIterator.

These interfaces are designed to wrap any underlying XML DOM software so that `mx::core` does not care or know about the XML DOM code.  A set of implementation classes wrapping pugixml are provided, but if you need to use, say Xerces or RapidXML, you can look at the PugiElement, PugiDoc, etc classes and wrap whatever library you need.

Here's how you can read a MusicXML document into `mx::core` classes by way of `mx::xml`.

```
#include "mx/core/Document.h"
#include "mx/utility/Utility.h"
#include "functions.h"
#include "mx/xml/XFactory.h"
#include "mx/xml/XDoc.h"

#include <iostream>
#include <string>
#include <sstream>

int main(int argc, const char *argv[])
{
    // allocate the objects
    mx::core::DocumentPtr mxDoc = makeDocument();
    mx::xml::XDocPtr xmlDoc = mx::xml::XFactory::makeXDoc();
    
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

An executable program named MxTest is also included in the project.  MxTest utilizes the CppUnitLite macro library by Michael Feathers.  Licensing of this library is not clear, here is a link to the source of this library http://c2.com/cgi/wiki?CppUnitLite
CppUnitLite appears to be abandonware, but it is very useful.
Here are some additional CppUnitLite links
http://www.objectmentor.com/resources/downloads.html
https://github.com/smikes/CppUnitLite

The tests are slow to compile, see the *Compiling* section for more info on how to skip compilation of the tests.

### Known Issues and Todo List
- There is no easy way to "deep copy" anything.  Each element and attributes class needs a "clone" function.
- XsID uniqueness is not constrained.  Should it be?
- XsIDREF's, especially in the ScorePartList are not constrained to relate to the PartList XsID's.  Thus it is possible to create an unparsable MusicXML document.  A feature could be added to enforce this one-to-one constraint.  Should this feature be added in the core model?  Maybe not.
- A simplified and stable facade API should be added to make it much, much easier to understand and interact with MusicXML documents.

##### Historical Notes

**Update: August 16, 2016:** All tests are passing (core, xml and import).  The remaining items to do on the ximport feature are
- search for all `\\TODO's` and fix those that can be fixed
- Standardize the code file copyrights and bump to version 0.2
- Rename XElement functions to better names
- Update the readme including information about the XDoc XElement classes and how to implement those interfaces.

**Update: August 11, 2016:** The feature to import MusicXML files is essentially complete with most "round-trip" integration tests passing.

**Update: June 27, 2016:** An "integration" tester has been added to the MxTest executable.  This exists in ImportTest.cpp, and ImportTestImpl.h/cpp.  A top level directory "Resources" has been added to hold test input (i.e. "golden") files.  The input files are gathered from Recordare, Lilypond and MuseScore and to this I have added a few of my own scores as exported by Finale Dolet.

Each of these test input files has been "scrubbed" using the XDoc classes (i.e. it has been round-tripped through pugixml and has been updated to a normalized MusicXML 3.0 header format.  The resultant scrubbed files are in Resources/expected.  During the test run, a csv file is written in Resources/testOutput recording a row for each test (Pass/Fail, duration of test, messages, etc).  Each time a test failure is encountered the expected file and the error file will be saved to the Resources/testOutput directory to allow for visual inspection.

Currently this tester is a "wire-up".  All 263 of these round-trip import/export tests fail because the implementation does not yet exist in mx::core.  The next body of work will be the mx::core implementation.


**Update: June 20, 2016:** A simple interface to for XML DOM has been added in the mx::xml namespace.  The key classes (pure virtual) are XDoc, XElement, XAttribute, XElementIterator, XAttributeIterator.  These are implemented by concrete classes PugiDoc, PugiElement, etc. which serve as a wrapper for the pugixml library (http://pugixml.org/).  Although this is a static library, a class XFactory can be used to create a Pugi instance of the XDoc interface.

The idea behind using a pure virtual interface is that the client of the Music XML Class Library can, in theory choose a different XML DOM library (Xerces, TinyXML, etc) and wrap with instances of the XDoc interfaces and the Music XML core classes will not know the difference.

**Update: June 20, 2016:** The GitHub repo was bloated and has been entirely wiped-out and restarted.