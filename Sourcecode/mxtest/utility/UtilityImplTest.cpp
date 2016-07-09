// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_UTILTIY_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/utility/UtilityImpl.h"
#include "mx/utility/Utility.h"

#include <string>
#include <sstream>
#include <memory>


using namespace std;
using namespace mx::core;
using namespace mx::utility;


TEST( makePartGroupStartTagDefault, MxUtilityImpl )
{
    PartGroupParams params;
    mx::core::PartGroupPtr obj = makePartGroupStartTag( params );

    std::stringstream expected;
    expected << R"(<part-group type="start">)" << endl;
    expected << R"(   <group-name-display>)" << endl;
    expected << R"(      <display-text></display-text>)" << endl;
    expected << R"(   </group-name-display>)" << endl;
    expected << R"(   <group-abbreviation-display>)" << endl;
    expected << R"(      <display-text></display-text>)" << endl;
    expected << R"(   </group-abbreviation-display>)" << endl;
    expected << R"(   <group-symbol>bracket</group-symbol>)" << endl;
    expected << R"(   <group-barline>yes</group-barline>)" << endl;
    expected << R"(</part-group>)";
    
    std::stringstream actual;
    obj->toStream( actual, 0 );
    
    CHECK_EQUAL( expected.str(), actual.str() )
}
T_END


TEST( makePartGroupStartTagNameOnly, MxUtilityImpl )
{
    PartGroupParams params( "Strings" );
    mx::core::PartGroupPtr obj = makePartGroupStartTag( params );
    
    std::stringstream expected;
    expected << R"(<part-group type="start">)" << endl;
    expected << R"(   <group-name>Strings</group-name>)" << endl;
    expected << R"(   <group-name-display>)" << endl;
    expected << R"(      <display-text></display-text>)" << endl;
    expected << R"(   </group-name-display>)" << endl;
    expected << R"(   <group-abbreviation-display>)" << endl;
    expected << R"(      <display-text></display-text>)" << endl;
    expected << R"(   </group-abbreviation-display>)" << endl;
    expected << R"(   <group-symbol>bracket</group-symbol>)" << endl;
    expected << R"(   <group-barline>yes</group-barline>)" << endl;
    expected << R"(</part-group>)";
    
    std::stringstream actual;
    obj->toStream( actual, 0 );
    
    CHECK_EQUAL( expected.str(), actual.str() )
}
T_END

TEST( makePartGroupAddAbbreviation, MxUtilityImpl )
{
    PartGroupParams params( "Strings" );
    params.abbreviation = "Str.";
    mx::core::PartGroupPtr obj = makePartGroupStartTag( params );
    
    std::stringstream expected;
    expected << R"(<part-group type="start">)" << endl;
    expected << R"(   <group-name>Strings</group-name>)" << endl;
    expected << R"(   <group-name-display>)" << endl;
    expected << R"(      <display-text></display-text>)" << endl;
    expected << R"(   </group-name-display>)" << endl;
    expected << R"(   <group-abbreviation>Str.</group-abbreviation>)" << endl;
    expected << R"(   <group-abbreviation-display>)" << endl;
    expected << R"(      <display-text></display-text>)" << endl;
    expected << R"(   </group-abbreviation-display>)" << endl;
    expected << R"(   <group-symbol>bracket</group-symbol>)" << endl;
    expected << R"(   <group-barline>yes</group-barline>)" << endl;
    expected << R"(</part-group>)";
    
    std::stringstream actual;
    obj->toStream( actual, 0 );
    
    CHECK_EQUAL( expected.str(), actual.str() )
}
T_END


TEST( makePartGroupAddNumber, MxUtilityImpl )
{
    PartGroupParams params( "Strings" );
    params.abbreviation = "Str.";
    params.number = 0;
    mx::core::PartGroupPtr obj = makePartGroupStartTag( params );
    
    std::stringstream expected;
    expected << R"(<part-group type="start" number="0">)" << endl;
    expected << R"(   <group-name>Strings</group-name>)" << endl;
    expected << R"(   <group-name-display>)" << endl;
    expected << R"(      <display-text></display-text>)" << endl;
    expected << R"(   </group-name-display>)" << endl;
    expected << R"(   <group-abbreviation>Str.</group-abbreviation>)" << endl;
    expected << R"(   <group-abbreviation-display>)" << endl;
    expected << R"(      <display-text></display-text>)" << endl;
    expected << R"(   </group-abbreviation-display>)" << endl;
    expected << R"(   <group-symbol>bracket</group-symbol>)" << endl;
    expected << R"(   <group-barline>yes</group-barline>)" << endl;
    expected << R"(</part-group>)";
    
    std::stringstream actual;
    obj->toStream( actual, 0 );
    
    CHECK_EQUAL( expected.str(), actual.str() )
}
T_END


TEST( makePartGroupAddDisplayName, MxUtilityImpl )
{
    PartGroupParams params( "Strings" );
    params.abbreviation = "Str.";
    params.number = 1;
    params.displayName = "Chili You Ugly";
    mx::core::PartGroupPtr obj = makePartGroupStartTag( params );
    
    std::stringstream expected;
    expected << R"(<part-group type="start" number="1">)" << endl;
    expected << R"(   <group-name>Strings</group-name>)" << endl;
    expected << R"(   <group-name-display>)" << endl;
    expected << R"(      <display-text>Chili You Ugly</display-text>)" << endl;
    expected << R"(   </group-name-display>)" << endl;
    expected << R"(   <group-abbreviation>Str.</group-abbreviation>)" << endl;
    expected << R"(   <group-abbreviation-display>)" << endl;
    expected << R"(      <display-text></display-text>)" << endl;
    expected << R"(   </group-abbreviation-display>)" << endl;
    expected << R"(   <group-symbol>bracket</group-symbol>)" << endl;
    expected << R"(   <group-barline>yes</group-barline>)" << endl;
    expected << R"(</part-group>)";
    
    std::stringstream actual;
    obj->toStream( actual, 0 );
    
    CHECK_EQUAL( expected.str(), actual.str() )
}
T_END


TEST( makePartGroupAddDisplayAbbreviation, MxUtilityImpl )
{
    PartGroupParams params( "Strings" );
    params.abbreviation = "Str.";
    params.number = 1;
    params.displayName = "Chili You Ugly";
    params.displayAbbreviation = "Bones";
    mx::core::PartGroupPtr obj = makePartGroupStartTag( params );
    
    std::stringstream expected;
    expected << R"(<part-group type="start" number="1">)" << endl;
    expected << R"(   <group-name>Strings</group-name>)" << endl;
    expected << R"(   <group-name-display>)" << endl;
    expected << R"(      <display-text>Chili You Ugly</display-text>)" << endl;
    expected << R"(   </group-name-display>)" << endl;
    expected << R"(   <group-abbreviation>Str.</group-abbreviation>)" << endl;
    expected << R"(   <group-abbreviation-display>)" << endl;
    expected << R"(      <display-text>Bones</display-text>)" << endl;
    expected << R"(   </group-abbreviation-display>)" << endl;
    expected << R"(   <group-symbol>bracket</group-symbol>)" << endl;
    expected << R"(   <group-barline>yes</group-barline>)" << endl;
    expected << R"(</part-group>)";
    
    std::stringstream actual;
    obj->toStream( actual, 0 );
    
    CHECK_EQUAL( expected.str(), actual.str() )
}
T_END


TEST( makePartGroupNoBarline, MxUtilityImpl )
{
    PartGroupParams params( "Strings" );
    params.abbreviation = "Str.";
    params.number = 1;
    params.displayName = "Chili You Ugly";
    params.displayAbbreviation = "Bones";
    params.isGroupBarline = false;
    mx::core::PartGroupPtr obj = makePartGroupStartTag( params );
    
    std::stringstream expected;
    expected << R"(<part-group type="start" number="1">)" << endl;
    expected << R"(   <group-name>Strings</group-name>)" << endl;
    expected << R"(   <group-name-display>)" << endl;
    expected << R"(      <display-text>Chili You Ugly</display-text>)" << endl;
    expected << R"(   </group-name-display>)" << endl;
    expected << R"(   <group-abbreviation>Str.</group-abbreviation>)" << endl;
    expected << R"(   <group-abbreviation-display>)" << endl;
    expected << R"(      <display-text>Bones</display-text>)" << endl;
    expected << R"(   </group-abbreviation-display>)" << endl;
    expected << R"(   <group-symbol>bracket</group-symbol>)" << endl;
    expected << R"(   <group-barline>no</group-barline>)" << endl;
    expected << R"(</part-group>)";
    
    std::stringstream actual;
    obj->toStream( actual, 0 );
    
    CHECK_EQUAL( expected.str(), actual.str() )
}
T_END


TEST( makePartGroupNoSymbol, MxUtilityImpl )
{
    PartGroupParams params( "Strings" );
    params.abbreviation = "Str.";
    params.number = 1;
    params.displayName = "Chili You Ugly";
    params.displayAbbreviation = "Bones";
    params.isGroupBarline = false;
    params.groupSymbol = GroupSymbolValue::none;
    mx::core::PartGroupPtr obj = makePartGroupStartTag( params );
    
    std::stringstream expected;
    expected << R"(<part-group type="start" number="1">)" << endl;
    expected << R"(   <group-name>Strings</group-name>)" << endl;
    expected << R"(   <group-name-display>)" << endl;
    expected << R"(      <display-text>Chili You Ugly</display-text>)" << endl;
    expected << R"(   </group-name-display>)" << endl;
    expected << R"(   <group-abbreviation>Str.</group-abbreviation>)" << endl;
    expected << R"(   <group-abbreviation-display>)" << endl;
    expected << R"(      <display-text>Bones</display-text>)" << endl;
    expected << R"(   </group-abbreviation-display>)" << endl;
    expected << R"(   <group-symbol>none</group-symbol>)" << endl;
    expected << R"(   <group-barline>no</group-barline>)" << endl;
    expected << R"(</part-group>)";
    
    std::stringstream actual;
    obj->toStream( actual, 0 );
    
    CHECK_EQUAL( expected.str(), actual.str() )
}
T_END


TEST( isPartListInitializedTrue1, MxUtilityImpl )
{
    auto doc = makeDocument( mx::core::DocumentChoice::partwise );
    auto partList = doc->getScorePartwise()->getScoreHeaderGroup()->getPartList();
    auto somePart = makePartGroupOrScorePart();
    somePart->setChoice( PartGroupOrScorePart::Choice::scorePart );
    somePart->getScorePart()->getAttributes()->id = XsID( "XXX" );
    partList->addPartGroupOrScorePart( somePart );
    CHECK( isPartListInitialized( doc ) );
}
T_END


TEST( isPartListInitializedTrue2, MxUtilityImpl )
{
    auto doc = makeDocument( mx::core::DocumentChoice::partwise );
    auto partList = doc->getScorePartwise()->getScoreHeaderGroup()->getPartList();
    auto somePart = partList->getScorePart();
    somePart->getAttributes()->id = XsID( "XXX" );
    CHECK( isPartListInitialized( doc ) );
}
T_END


TEST( isPartListInitializedFalse, MxUtilityImpl )
{
    auto doc = makeDocument( mx::core::DocumentChoice::partwise );
    CHECK( ! isPartListInitialized( doc ) );
}
T_END


TEST( makePartDeclaration, MxUtilityImpl )
{
    PartParams params;
    params.name = "Bishop";
    params.abbreviation = "Bish";
    params.uniqueId = "BISHOP_1";
    auto obj = makePartDeclaration( params );
    CHECK_EQUAL( "BISHOP_1", obj->getAttributes()->id.getValue() );
    CHECK_EQUAL( "Bishop", obj->getPartName()->getValue().getValue() );
    CHECK_EQUAL( "Bish", obj->getPartAbbreviation()->getValue().getValue() );
    CHECK_EQUAL( 0, obj->getScoreInstrumentSet().size() );
}
T_END


TEST( makePartDeclarationWithInstrument, MxUtilityImpl )
{
    PartParams params;
    params.name = "Bishop";
    params.abbreviation = "Bish";
    params.uniqueId = "BISHOP_1";
    params.instrumentName = "Flute";
    params.instrumentAbbreviation = "Fl.";
    params.instrumentUniqueId = "BISHOP_1_INSTR";
    auto obj = makePartDeclaration( params );
    CHECK_EQUAL( 1, obj->getScoreInstrumentSet().size() );
    auto scoreInstrument = *( obj->getScoreInstrumentSet().cbegin() );
    CHECK_EQUAL( "BISHOP_1_INSTR", scoreInstrument->getAttributes()->id.getValue() )
    CHECK_EQUAL( "Flute", scoreInstrument->getInstrumentName()->getValue().getValue() )
    CHECK( scoreInstrument->getHasInstrumentAbbreviation() )
    CHECK_EQUAL( "Fl.", scoreInstrument->getInstrumentAbbreviation()->getValue().getValue() );
}
T_END


TEST( makePartDeclarationWithVirtualInstrument, MxUtilityImpl )
{
    PartParams params;
    params.name = "Bishop";
    params.abbreviation = "Bish";
    params.uniqueId = "BISHOP_1";
    params.instrumentName = "Flute";
    params.instrumentAbbreviation = "Fl.";
    params.instrumentUniqueId = "BISHOP_1_INSTR";
    params.virtualInstrument = "Hello";
    params.virtualLibrary = "World";
    auto obj = makePartDeclaration( params );
    CHECK_EQUAL( 1, obj->getScoreInstrumentSet().size() );
    CHECK_EQUAL( 0, obj->getMidiDeviceInstrumentGroupSet().size() )
    auto scoreInstrument = *( obj->getScoreInstrumentSet().cbegin() );
    CHECK( scoreInstrument->getHasVirtualInstrument() )
    auto virtualInstrument = scoreInstrument->getVirtualInstrument();
    CHECK( virtualInstrument->getHasVirtualName() )
    CHECK_EQUAL( "Hello", virtualInstrument->getVirtualName()->getValue().getValue() )
    CHECK( virtualInstrument->getHasVirtualLibrary() )
    CHECK_EQUAL( "World", virtualInstrument->getVirtualLibrary()->getValue().getValue() )
}
T_END


TEST( makePartDeclarationWithMidiData, MxUtilityImpl )
{
    PartParams params;
    params.name = "Bishop";
    params.abbreviation = "Bish";
    params.uniqueId = "BISHOP_1";
    params.instrumentName = "Flute";
    params.instrumentAbbreviation = "Fl.";
    params.instrumentUniqueId = "BISHOP_1_INSTR";
    params.midiBank = 1;
    params.midiChannel = 2;
    params.midiProgram = 3;
    params.midiVolume = 4;
    params.midiPan = 5;
    params.midiName = "Midi Name Here";
    auto obj = makePartDeclaration( params );
    CHECK_EQUAL( 1, obj->getScoreInstrumentSet().size() );
    CHECK_EQUAL( 1, obj->getMidiDeviceInstrumentGroupSet().size() );
    auto grp = *( obj->getMidiDeviceInstrumentGroupSet().cbegin() );
    CHECK( grp->getHasMidiDevice() );
    CHECK_EQUAL( "Bank 1", grp->getMidiDevice()->getValue().getValue() );
    CHECK( grp->getHasMidiInstrument() );
    auto midi = grp->getMidiInstrument();
    CHECK( midi->getHasMidiChannel() )
    CHECK_EQUAL( 2, midi->getMidiChannel()->getValue().getValue() );
    CHECK( midi->getHasMidiProgram() )
    CHECK_EQUAL( 3, midi->getMidiProgram()->getValue().getValue() );
    CHECK( midi->getHasVolume() )
    CHECK_EQUAL( 4, midi->getVolume()->getValue().getValue() );
    CHECK( midi->getHasPan() )
    CHECK_EQUAL( 4, midi->getPan()->getValue().getValue() );
    CHECK( midi->getHasMidiName() )
    CHECK_EQUAL( "Midi Name Here", midi->getMidiName()->getValue().getValue() );
}
T_END


TEST( addInitialPartToDocument, MxUtilityImpl )
{
    PartParams params;
    params.name = "Bishop";
    params.uniqueId = "BISHOP_1";
    auto doc = makeDocument( mx::core::DocumentChoice::partwise );
    addInitialPartToDocument( doc, params );
    StringType expected = "BISHOP_1";
    StringType actual = doc->getScorePartwise()->getScoreHeaderGroup()->getPartList()->getScorePart()->getAttributes()->id.getValue();
    CHECK_EQUAL( expected, actual );
    CHECK_EQUAL( 1, doc->getScorePartwise()->getPartwisePartSet().size() );
    actual = ( *( doc->getScorePartwise()->getPartwisePartSet().cbegin() ) )->getAttributes()->id.getValue();
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( addSubsequentPartToDocument, MxUtilityImpl )
{
    PartParams params;
    params.name = "Bishop";
    params.uniqueId = "BISHOP_1";
    auto doc = makeDocument( mx::core::DocumentChoice::partwise );
    addInitialPartToDocument( doc, params );
    params.name = "Bones";
    params.uniqueId = "BONES_1";
    addSubsequentPartToDocument( doc, params );
    StringType expected = "BISHOP_1";
    StringType actual = doc->getScorePartwise()->getScoreHeaderGroup()->getPartList()->getScorePart()->getAttributes()->id.getValue();
    CHECK_EQUAL( expected, actual );
    CHECK_EQUAL( 1, doc->getScorePartwise()->getScoreHeaderGroup()->getPartList()->getPartGroupOrScorePartSet().size() );
    CHECK_EQUAL( 2, doc->getScorePartwise()->getPartwisePartSet().size() );
    expected = "BONES_1";
    actual = ( *( doc->getScorePartwise()->getScoreHeaderGroup()->getPartList()->getPartGroupOrScorePartSet().cbegin() ) )->getScorePart()->getAttributes()->id.getValue();
    auto actualIter = doc->getScorePartwise()->getPartwisePartSet().cbegin();
    expected = "BISHOP_1";
    actual = (*actualIter)->getAttributes()->id.getValue();
    CHECK_EQUAL( expected, actual );
    ++actualIter;
    expected = "BONES_1";
    actual = (*actualIter)->getAttributes()->id.getValue();
    CHECK_EQUAL( expected, actual );
    //doc->toStream( cout );
}
T_END


TEST( createEmptyNote, MxUtilityImpl )
{
    auto n = createEmptyNote( NoteChoice::Choice::normal );
    CHECK( MusicDataChoice::Choice::note == n->getChoice() );
    CHECK( NoteChoice::Choice::normal == n->getNote()->getNoteChoice()->getChoice() )
    CHECK( ! n->getNote()->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->getFullNoteTypeChoice()->getPitch()->getHasAlter() );
    CHECK( ! n->getNote()->getEditorialVoiceGroup()->getHasVoice() );
    CHECK( ! n->getNote()->getHasType() );
    CHECK_EQUAL( 0, n->getNote()->getDotSet().size() );
    CHECK( ! n->getNote()->getHasAccidental() );
    CHECK( ! n->getNote()->getHasStem() )
    CHECK( ! n->getNote()->getHasStaff() );
    CHECK_EQUAL( 0, n->getNote()->getBeamSet().size() );
    CHECK( ! n->getNote()->getHasPlay() )
    CHECK( ! n->getNote()->getHasInstrument() )
    CHECK( ! n->getNote()->getHasNotehead() )
    CHECK( ! n->getNote()->getHasNoteheadText() );
    CHECK( ! n->getNote()->getHasTimeModification() );
    CHECK_EQUAL( 0, n->getNote()->getLyricSet().size() );
}
T_END


TEST( setNoteStep_c, MxUtilityImpl )
{
    auto n = createEmptyNote( NoteChoice::Choice::normal );
    setNoteStep( n, 0 );
    CHECK( FullNoteTypeChoice::Choice::pitch == n->getNote()->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->getFullNoteTypeChoice()->getChoice() );
    CHECK( StepEnum::c == n->getNote()->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->getFullNoteTypeChoice()->getPitch()->getStep()->getValue() );
}
T_END


TEST( setNoteStep_d, MxUtilityImpl )
{
    auto n = createEmptyNote( NoteChoice::Choice::normal );
    setNoteStep( n, 1 );
    CHECK( FullNoteTypeChoice::Choice::pitch == n->getNote()->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->getFullNoteTypeChoice()->getChoice() );
    CHECK( StepEnum::d == n->getNote()->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->getFullNoteTypeChoice()->getPitch()->getStep()->getValue() );
}
T_END


TEST( setNoteStep_e, MxUtilityImpl )
{
    auto n = createEmptyNote( NoteChoice::Choice::normal );
    setNoteStep( n, 2 );
    CHECK( FullNoteTypeChoice::Choice::pitch == n->getNote()->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->getFullNoteTypeChoice()->getChoice() );
    CHECK( StepEnum::e == n->getNote()->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->getFullNoteTypeChoice()->getPitch()->getStep()->getValue() );
}
T_END


TEST( setNoteStep_f, MxUtilityImpl )
{
    auto n = createEmptyNote( NoteChoice::Choice::normal );
    setNoteStep( n, 3 );
    CHECK( FullNoteTypeChoice::Choice::pitch == n->getNote()->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->getFullNoteTypeChoice()->getChoice() );
    CHECK( StepEnum::f == n->getNote()->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->getFullNoteTypeChoice()->getPitch()->getStep()->getValue() );
}
T_END


TEST( setNoteStep_g, MxUtilityImpl )
{
    auto n = createEmptyNote( NoteChoice::Choice::normal );
    setNoteStep( n, 4 );
    CHECK( FullNoteTypeChoice::Choice::pitch == n->getNote()->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->getFullNoteTypeChoice()->getChoice() );
    CHECK( StepEnum::g == n->getNote()->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->getFullNoteTypeChoice()->getPitch()->getStep()->getValue() );
}
T_END


TEST( setNoteStep_a, MxUtilityImpl )
{
    auto n = createEmptyNote( NoteChoice::Choice::normal );
    setNoteStep( n, 5 );
    CHECK( FullNoteTypeChoice::Choice::pitch == n->getNote()->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->getFullNoteTypeChoice()->getChoice() );
    CHECK( StepEnum::a == n->getNote()->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->getFullNoteTypeChoice()->getPitch()->getStep()->getValue() );
}
T_END


TEST( setNoteStep_b, MxUtilityImpl )
{
    auto n = createEmptyNote( NoteChoice::Choice::normal );
    setNoteStep( n, 6 );
    CHECK( FullNoteTypeChoice::Choice::pitch == n->getNote()->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->getFullNoteTypeChoice()->getChoice() );
    CHECK( StepEnum::b == n->getNote()->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->getFullNoteTypeChoice()->getPitch()->getStep()->getValue() );
}
T_END


TEST( setNoteAlter, MxUtilityImpl )
{
    auto n = createEmptyNote( NoteChoice::Choice::normal );
    setNoteStep( n, 6 );
    setNoteAlter( n, -1 );
    CHECK( FullNoteTypeChoice::Choice::pitch == n->getNote()->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->getFullNoteTypeChoice()->getChoice() );
    CHECK_DOUBLES_EQUAL( -1.0, n->getNote()->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->getFullNoteTypeChoice()->getPitch()->getAlter()->getValue().getValue(), 0.00001 );
}
T_END


TEST( setNoteOctave, MxUtilityImpl )
{
    auto n = createEmptyNote( NoteChoice::Choice::normal );
    setNoteStep( n, 6 );
    setNoteAlter( n, -1 );
    setNoteOctave( n, 5 );
    CHECK( FullNoteTypeChoice::Choice::pitch == n->getNote()->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->getFullNoteTypeChoice()->getChoice() );
    CHECK_EQUAL( 5, n->getNote()->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->getFullNoteTypeChoice()->getPitch()->getOctave()->getValue().getValue() );
}
T_END


TEST( setNoteDurationInt, MxUtilityImpl )
{
    auto n = createEmptyNote( NoteChoice::Choice::normal );
    setNoteStep( n, 6 );
    setNoteAlter( n, -1 );
    setNoteOctave( n, 5 );
    setNoteDurationInt( n, 111 );
    CHECK_DOUBLES_EQUAL( 111.0, n->getNote()->getNoteChoice()->getNormalNoteGroup()->getDuration()->getValue().getValue(), 0.00001 );
}
T_END


TEST( setNoteStaffNumber, MxUtilityImpl )
{
    auto n = createEmptyNote( NoteChoice::Choice::normal );
    setNoteStep( n, 6 );
    setNoteAlter( n, -1 );
    setNoteOctave( n, 5 );
    setNoteDurationInt( n, 111 );
    setNoteStaffNumber( n, 2 );
    CHECK( n->getNote()->getHasStaff() );
    CHECK_EQUAL( 2, n->getNote()->getStaff()->getValue().getValue() )
}
T_END


TEST( setNoteVoiceNumber, MxUtilityImpl )
{
    auto n = createEmptyNote( NoteChoice::Choice::normal );
    setNoteStep( n, 6 );
    setNoteAlter( n, -1 );
    setNoteOctave( n, 5 );
    setNoteDurationInt( n, 111 );
    setNoteStaffNumber( n, 2 );
    setNoteVoiceNumber( n, 3 );
    CHECK( n->getNote()->getEditorialVoiceGroup()->getHasVoice() );
    CHECK_EQUAL( std::to_string(3), n->getNote()->getEditorialVoiceGroup()->getVoice()->getValue().getValue() );
}
T_END


TEST( setStemDirectionUnspecified, MxUtilityImpl )
{
    auto n = createEmptyNote( NoteChoice::Choice::normal );
    setNoteStep( n, 6 );
    setNoteAlter( n, -1 );
    setNoteOctave( n, 5 );
    setNoteDurationInt( n, 111 );
    setNoteStaffNumber( n, 2 );
    setNoteVoiceNumber( n, 3 );
    setStemDirection( n, mx::utility::UpDown::unspecified );
    CHECK( ! n->getNote()->getHasStem() )
}
T_END


TEST( setStemDirection, MxUtilityImpl )
{
    auto n = createEmptyNote( NoteChoice::Choice::normal );
    setNoteStep( n, 6 );
    setNoteAlter( n, -1 );
    setNoteOctave( n, 5 );
    setNoteDurationInt( n, 111 );
    setNoteStaffNumber( n, 2 );
    setNoteVoiceNumber( n, 3 );
    setStemDirection( n, mx::utility::UpDown::down );
    CHECK( n->getNote()->getHasStem() );
    CHECK( StemValue::down == n->getNote()->getStem()->getValue() );
}
T_END


TEST( setNoteDurationType, MxUtilityImpl )
{
    auto n = createEmptyNote( NoteChoice::Choice::normal );
    setNoteStep( n, 6 );
    setNoteAlter( n, -1 );
    setNoteOctave( n, 5 );
    setNoteDurationInt( n, 111 );
    setNoteStaffNumber( n, 2 );
    setNoteVoiceNumber( n, 3 );
    setStemDirection( n, mx::utility::UpDown::down );
    setNoteDurationType( n, NoteTypeValue::oneHundredTwentyEighth );
    CHECK( n->getNote()->getHasType() );
    CHECK( NoteTypeValue::oneHundredTwentyEighth == n->getNote()->getType()->getValue() );
}
T_END


TEST( setNoteDurationDots, MxUtilityImpl )
{
    auto n = createEmptyNote( NoteChoice::Choice::normal );
    setNoteStep( n, 6 );
    setNoteAlter( n, -1 );
    setNoteOctave( n, 5 );
    setNoteDurationInt( n, 111 );
    setNoteStaffNumber( n, 2 );
    setNoteVoiceNumber( n, 3 );
    setStemDirection( n, mx::utility::UpDown::down );
    setNoteDurationType( n, NoteTypeValue::oneHundredTwentyEighth );
    setNoteDurationDots( n, 2 );
    CHECK_EQUAL( 2, n->getNote()->getDotSet().size() )
}
T_END


TEST( setNoteBeams, MxUtilityImpl )
{
    auto n = createEmptyNote( NoteChoice::Choice::normal );
    setNoteStep( n, 6 );
    setNoteAlter( n, -1 );
    setNoteOctave( n, 5 );
    setNoteDurationInt( n, 111 );
    setNoteStaffNumber( n, 2 );
    setNoteVoiceNumber( n, 3 );
    setStemDirection( n, mx::utility::UpDown::down );
    setNoteDurationType( n, NoteTypeValue::oneHundredTwentyEighth );
    setNoteDurationDots( n, 2 );
    vector<BeamValue> beams;
    beams.push_back( BeamValue::begin );
    beams.push_back( BeamValue::continue_ );
    beams.push_back( BeamValue::end );
    setNoteBeams( n, beams );
    auto beamItr = n->getNote()->getBeamSet().cbegin();
    auto beamEnd = n->getNote()->getBeamSet().cend();
    CHECK( beamItr != beamEnd );
    CHECK_EQUAL( 1, (*beamItr)->getAttributes()->number.getValue() );
    CHECK( BeamValue::begin == (*beamItr)->getValue() );
    ++beamItr;
    CHECK( beamItr != beamEnd );
    CHECK_EQUAL( 2, (*beamItr)->getAttributes()->number.getValue() );
    CHECK( BeamValue::continue_ == (*beamItr)->getValue() );
    ++beamItr;
    CHECK( beamItr != beamEnd );
    CHECK_EQUAL( 3, (*beamItr)->getAttributes()->number.getValue() );
    CHECK( BeamValue::end == (*beamItr)->getValue() );
    ++beamItr;
    CHECK( beamItr == beamEnd );
}
T_END


TEST( setNoteAccidental, MxUtilityImpl )
{
    auto n = createEmptyNote( NoteChoice::Choice::normal );
    setNoteStep( n, 6 );
    setNoteAlter( n, -1 );
    setNoteOctave( n, 5 );
    setNoteDurationInt( n, 111 );
    setNoteStaffNumber( n, 2 );
    setNoteVoiceNumber( n, 3 );
    setStemDirection( n, mx::utility::UpDown::down );
    setNoteDurationType( n, NoteTypeValue::oneHundredTwentyEighth );
    setNoteDurationDots( n, 2 );
    vector<BeamValue> beams;
    beams.push_back( BeamValue::begin );
    beams.push_back( BeamValue::continue_ );
    beams.push_back( BeamValue::end );
    setNoteBeams( n, beams );
    setNoteAccidental( n, -1 );
    CHECK( n->getNote()->getHasAccidental() )
    CHECK( AccidentalValue::flat == n->getNote()->getAccidental()->getValue() )
}
T_END


TEST( createCueNote, MxUtilityImpl )
{
    auto n = createEmptyNote( NoteChoice::Choice::cue );
    setNoteStep( n, 6 );
    setNoteAlter( n, -1 );
    setNoteOctave( n, 5 );
    setNoteDurationInt( n, 111 );
    setNoteStaffNumber( n, 2 );
    setNoteVoiceNumber( n, 3 );
    setStemDirection( n, mx::utility::UpDown::down );
    setNoteDurationType( n, NoteTypeValue::oneHundredTwentyEighth );
    setNoteDurationDots( n, 2 );
    vector<BeamValue> beams;
    beams.push_back( BeamValue::begin );
    beams.push_back( BeamValue::continue_ );
    beams.push_back( BeamValue::end );
    setNoteBeams( n, beams );
    setNoteAccidental( n, -1 );
    
    std::stringstream expected;
    expected << R"(<note>)" << std::endl;
    expected << R"(   <cue/>)" << std::endl;
    expected << R"(   <pitch>)" << std::endl;
    expected << R"(      <step>B</step>)" << std::endl;
    expected << R"(      <alter>-1</alter>)" << std::endl;
    expected << R"(      <octave>5</octave>)" << std::endl;
    expected << R"(   </pitch>)" << std::endl;
    expected << R"(   <duration>111</duration>)" << std::endl;
    expected << R"(   <voice>3</voice>)" << std::endl;
    expected << R"(   <type>128th</type>)" << std::endl;
    expected << R"(   <dot/>)" << std::endl;
    expected << R"(   <dot/>)" << std::endl;
    expected << R"(   <accidental>flat</accidental>)" << std::endl;
    expected << R"(   <stem>down</stem>)" << std::endl;
    expected << R"(   <staff>2</staff>)" << std::endl;
    expected << R"(   <beam number="1">begin</beam>)" << std::endl;
    expected << R"(   <beam number="2">continue</beam>)" << std::endl;
    expected << R"(   <beam number="3">end</beam>)" << std::endl;
    expected << R"(</note>)";
    std::stringstream actual;
    n->getNote()->toStream( actual, 0 );
    CHECK_EQUAL( expected.str(), actual.str() );
}
T_END


TEST( createGraceNote, MxUtilityImpl )
{
    auto n = createEmptyNote( NoteChoice::Choice::grace );
    setNoteStep( n, 6 );
    setNoteAlter( n, -1 );
    setNoteOctave( n, 5 );
    setNoteDurationInt( n, 111 );
    setNoteStaffNumber( n, 2 );
    setNoteVoiceNumber( n, 3 );
    setStemDirection( n, mx::utility::UpDown::down );
    setNoteDurationType( n, NoteTypeValue::oneHundredTwentyEighth );
    setNoteDurationDots( n, 2 );
    vector<BeamValue> beams;
    beams.push_back( BeamValue::begin );
    beams.push_back( BeamValue::continue_ );
    beams.push_back( BeamValue::end );
    setNoteBeams( n, beams );
    setNoteAccidental( n, -1 );
    
    std::stringstream expected;
    expected << R"(<note>)" << std::endl;
    expected << R"(   <grace/>)" << std::endl;
    expected << R"(   <pitch>)" << std::endl;
    expected << R"(      <step>B</step>)" << std::endl;
    expected << R"(      <alter>-1</alter>)" << std::endl;
    expected << R"(      <octave>5</octave>)" << std::endl;
    expected << R"(   </pitch>)" << std::endl;
    expected << R"(   <voice>3</voice>)" << std::endl;
    expected << R"(   <type>128th</type>)" << std::endl;
    expected << R"(   <dot/>)" << std::endl;
    expected << R"(   <dot/>)" << std::endl;
    expected << R"(   <accidental>flat</accidental>)" << std::endl;
    expected << R"(   <stem>down</stem>)" << std::endl;
    expected << R"(   <staff>2</staff>)" << std::endl;
    expected << R"(   <beam number="1">begin</beam>)" << std::endl;
    expected << R"(   <beam number="2">continue</beam>)" << std::endl;
    expected << R"(   <beam number="3">end</beam>)" << std::endl;
    expected << R"(</note>)";
    std::stringstream actual;
    n->getNote()->toStream( actual, 0 );
    CHECK_EQUAL( expected.str(), actual.str() );
}
T_END


TEST( getOrAddMeasurePropertiesGet, MxUtilityImpl )
{
    auto measure = makePartwiseMeasure();
    auto mdc = makeMusicDataChoice();
    mdc->setChoice( MusicDataChoice::Choice::properties );
    measure->getMusicDataGroup()->addMusicDataChoice( mdc );
    auto expected = mdc;
    auto actual = getOrAddMeasureProperties( measure );
    CHECK_EQUAL( expected.get(), actual.get() )
}
T_END


TEST( getOrAddMeasurePropertiesAdd, MxUtilityImpl )
{
    auto measure = makePartwiseMeasure();
    
    auto expected = getOrAddMeasureProperties( measure );
    CHECK_EQUAL( 1, measure->getMusicDataGroup()->getMusicDataChoiceSet().size() );
    auto actual = *( measure->getMusicDataGroup()->getMusicDataChoiceSet().cbegin() );
    CHECK_EQUAL( expected.get(), actual.get() )
}
T_END


TEST( setNoteIsRest, MxUtilityImpl )
{
    mx::utility::NoteParams params;
    auto note = mx::utility::createNote( params );
    //note->getNote()->toStream( std::cout, 0 );
    CHECK( note->getNote()->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->getFullNoteTypeChoice()->getChoice() == FullNoteTypeChoice::Choice::pitch );
    mx::utility::setNoteIsRest( note );
    CHECK( note->getNote()->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->getFullNoteTypeChoice()->getChoice() == FullNoteTypeChoice::Choice::rest );
    
}
T_END


TEST( setNoteIsChord, MxUtilityImpl )
{
    mx::utility::NoteParams params;
    auto note = mx::utility::createNote( params );
    //note->getNote()->toStream( std::cout, 0 );
    CHECK( ! note->getNote()->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->getHasChord() );
    mx::utility::setNoteIsChord( note );
    CHECK( note->getNote()->getNoteChoice()->getNormalNoteGroup()->getFullNoteGroup()->getHasChord() );

}
T_END


TEST( setNoteIsGrace, MxUtilityImpl )
{
    mx::utility::NoteParams params;
    auto note = mx::utility::createNote( params );
    //note->getNote()->toStream( std::cout, 0 );
    CHECK( note->getNote()->getNoteChoice()->getChoice() == NoteChoice::Choice::normal );
    mx::utility::setNoteIsGrace( note );
    //note->getNote()->toStream( std::cout, 0 );
    CHECK( note->getNote()->getNoteChoice()->getChoice() == NoteChoice::Choice::grace );
}
T_END


TEST( setNoteIsCue, MxUtilityImpl )
{
    mx::utility::NoteParams params;
    auto note = mx::utility::createNote( params );
    //note->getNote()->toStream( std::cout, 0 );
    CHECK( note->getNote()->getNoteChoice()->getChoice() == NoteChoice::Choice::normal );
    mx::utility::setNoteIsCue( note );
    //note->getNote()->toStream( std::cout, 0 );
    CHECK( note->getNote()->getNoteChoice()->getChoice() == NoteChoice::Choice::cue );
}
T_END

#endif
