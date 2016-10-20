// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/control/Path.h"
#include "mx/api/DocumentManager.h"
#include "mx/api/LayoutData.h"
#include "mx/core/Document.h"
#include "mx/core/elements/PageMargins.h"
#include "mx/core/elements/LeftMargin.h"
#include "mx/core/elements/RightMargin.h"
#include "mx/core/elements/TopMargin.h"
#include "mx/core/elements/BottomMargin.h"
#include "mxtest/control/File.h"

using namespace std;
using namespace mx::api;

inline int loadDoc()
{
    auto& docMngr = DocumentManager::getInstance();
    return docMngr.createFromFile( std::string{ mxtest::RESOURCES_DIRECTORY_PATH } + std::string{ "/recsuite/Dichterliebe01.xml" } );
}

inline int loadActorPreludeDoc()
{
    auto& docMngr = DocumentManager::getInstance();
    return docMngr.createFromFile( std::string{ mxtest::RESOURCES_DIRECTORY_PATH } + std::string{ "/recsuite/ActorPreludeSample.xml" } );
}

inline void destroyDoc( int documentId )
{
    auto& docMngr = DocumentManager::getInstance();
    docMngr.destroyDocument( documentId );
}


inline ScoreData getScore()
{
    auto documentId = loadDoc();
    auto score = DocumentManager::getInstance().getData( documentId );
    destroyDoc( documentId );
    return score;
}

inline ScoreData getActorPreludeScore()
{
    auto documentId = loadActorPreludeDoc();
    auto score = DocumentManager::getInstance().getData( documentId );
    destroyDoc( documentId );
    return score;
}

#define ROUND_TRIP_TEST_SCALAR( scalarType, fieldPath, fieldName, value, nameSuffix ) \
TEST( RoundTrip_##structType##_##fieldName##_##nameSuffix, DocumentManager ) \
{ \
    auto testValue = scalarType{ value }; \
    auto expectedStruct = ScoreData{}; \
    auto actualStruct = ScoreData{}; \
    auto& actualValue = actualStruct.fieldPath; \
    auto& expectedValue = expectedStruct.fieldPath; \
    expectedValue = testValue; \
    auto documentId = DocumentManager::getInstance().createFromScore( expectedStruct ); \
    std::ostringstream oss; \
    DocumentManager::getInstance().writeToStream( documentId, oss ); \
    DocumentManager::getInstance().destroyDocument( documentId ); \
    std::istringstream iss{ oss.str() }; \
    documentId = DocumentManager::getInstance().createFromStream( iss ); \
    actualStruct = DocumentManager::getInstance().getData( documentId ); \
    DocumentManager::getInstance().destroyDocument( documentId ); \
    CHECK_EQUAL( expectedValue, actualValue ); \
} \
T_END

#define ROUND_TRIP_TEST_SCALAR_DOUBLE( scalarType, fieldPath, fieldName, value, nameSuffix ) \
TEST( RoundTrip_##structType##_##fieldName##_##nameSuffix, DocumentManager ) \
{ \
auto testValue = scalarType{ value }; \
auto expectedStruct = ScoreData{}; \
auto actualStruct = ScoreData{}; \
auto& actualValue = actualStruct.fieldPath; \
auto& expectedValue = expectedStruct.fieldPath; \
expectedValue = testValue; \
auto documentId = DocumentManager::getInstance().createFromScore( expectedStruct ); \
std::ostringstream oss; \
DocumentManager::getInstance().writeToStream( documentId, oss ); \
DocumentManager::getInstance().destroyDocument( documentId ); \
std::istringstream iss{ oss.str() }; \
documentId = DocumentManager::getInstance().createFromStream( iss ); \
actualStruct = DocumentManager::getInstance().getData( documentId ); \
DocumentManager::getInstance().destroyDocument( documentId ); \
CHECK_DOUBLES_EQUAL( expectedValue, actualValue, MX_API_EQUALITY_EPSILON ); \
} \
T_END

inline bool runApiLoaderTest( std::string suiteName, std::string fileNamePart, std::string fileExtPart )
{
    auto fileName = mxtest::concatFileNameParts( fileNamePart, fileExtPart );
    auto filePath = mxtest::filePath( suiteName, fileName );

    auto& docMgr = mx::api::DocumentManager::getInstance();
    const auto docId = docMgr.createFromFile( filePath );
    const auto scoreData = docMgr.getData( docId );
    docMgr.destroyDocument( docId );
    return scoreData.parts.size() > 0;
}

#define MXTEST_API_LOAD( theSuiteName, theFileNamePart, theFileExtensionPart ) \
TEST( Load_##theSuiteName_##theFileNamePart , Api ) \
{ \
    const std::string suiteName = #theSuiteName; \
    const std::string fileNamePart = #theFileNamePart; \
    const std::string fileExtensionPart = #theFileExtensionPart;

#define MXTEST_API_LOAD_END } T_END

//#if 1==0
MXTEST_API_LOAD( lysuite, ly01a_Pitches_Pitches, xml )
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly01b_Pitches_Intervals, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly01c_Pitches_NoVoiceElement, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly01d_Pitches_Microtones, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END

//#endif
MXTEST_API_LOAD( lysuite, ly01e_Pitches_ParenthesizedAccidentals, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly01f_Pitches_ParenthesizedMicrotoneAccidentals, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly02a_Rests_Durations, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly02b_Rests_PitchedRests, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly02c_Rests_MultiMeasureRests, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly02d_Rests_Multimeasure_TimeSignatures, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly02e_Rests_NoType, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly03a_Rhythm_Durations, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly03b_Rhythm_Backup, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly03c_Rhythm_DivisionChange, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly03d_Rhythm_DottedDurations_Factors, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly11a_TimeSignatures, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly11b_TimeSignatures_NoTime, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly11c_TimeSignatures_CompoundSimple, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly11d_TimeSignatures_CompoundMultiple, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly11e_TimeSignatures_CompoundMixed, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly11f_TimeSignatures_SymbolMeaning, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly11g_TimeSignatures_SingleNumber, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly11h_TimeSignatures_SenzaMisura, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly12a_Clefs, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly12b_Clefs_NoKeyOrClef, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly13a_KeySignatures, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly13b_KeySignatures_ChurchModes, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly13c_KeySignatures_NonTraditional, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly13d_KeySignatures_Microtones, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly14a_StaffDetails_LineChanges, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END

//#endif
MXTEST_API_LOAD( lysuite, ly21a_Chord_Basic, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly21b_Chords_TwoNotes, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly21c_Chords_ThreeNotesDuration, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly21d_Chords_SchubertStabatMater, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly21e_Chords_PickupMeasures, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly21f_Chord_ElementInBetween, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly22a_Noteheads, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly22b_Staff_Notestyles, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly22c_Noteheads_Chords, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly22d_Parenthesized_Noteheads, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly23a_Tuplets, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly23b_Tuplets_Styles, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly23c_Tuplet_Display_NonStandard, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly23d_Tuplets_Nested, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END
//#endif

MXTEST_API_LOAD( lysuite, ly23e_Tuplets_Tremolo, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly23f_Tuplets_DurationButNoBracket, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly24a_GraceNotes, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly24b_ChordAsGraceNote, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly24c_GraceNote_MeasureEnd, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly24d_AfterGrace, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly24e_GraceNote_StaffChange, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly24f_GraceNote_Slur, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly31a_Directions, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END
//#endif

MXTEST_API_LOAD( lysuite, ly31c_MetronomeMarks, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly32a_Notations, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly32b_Articulations_Texts, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly32c_MultipleNotationChildren, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly32d_Arpeggio, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END
//#endif

MXTEST_API_LOAD( lysuite, ly33a_Spanners, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly33b_Spanners_Tie, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly33c_Spanners_Slurs, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly33d_Spanners_OctaveShifts, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly33e_Spanners_OctaveShifts_InvalidSize, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly33f_Trill_EndingOnGraceNote, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly33g_Slur_ChordedNotes, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly33h_Spanners_Glissando, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly33i_Ties_NotEnded, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly41a_MultiParts_Partorder, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly41b_MultiParts_MoreThan10, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly41c_StaffGroups, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly41d_StaffGroups_Nested, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly41e_StaffGroups_InstrumentNames_Linebroken, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly41f_StaffGroups_Overlapping, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END

// This is bad MusicXML and expected to throw
//MXTEST_API_LOAD( lysuite, ly41g_PartNoId, xml );
//bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
//CHECK( isSuccess );
//MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly41h_TooManyParts, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly41i_PartNameDisplay_Override, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly42a_MultiVoice_TwoVoicesOnStaff_Lyrics, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly42b_MultiVoice_MidMeasureClefChange, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly43a_PianoStaff, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly43b_MultiStaff_DifferentKeys, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly43c_MultiStaff_DifferentKeysAfterBackup, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly43d_MultiStaff_StaffChange, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly43e_Multistaff_ClefDynamics, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly45a_SimpleRepeat, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly45b_RepeatWithAlternatives, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly45c_RepeatMultipleTimes, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly45d_Repeats_Nested_Alternatives, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly45e_Repeats_Nested_Alternatives, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly45f_Repeats_InvalidEndings, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly45g_Repeats_NotEnded, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly46a_Barlines, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly46b_MidmeasureBarline, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly46c_Midmeasure_Clef, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly46d_PickupMeasure_ImplicitMeasures, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly46e_PickupMeasure_SecondVoiceStartsLater, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly46f_IncompleteMeasures, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly46g_PickupMeasure_Chordnames_FiguredBass, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly51b_Header_Quotes, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly51c_MultipleRights, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly51d_EmptyTitle, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly52a_PageLayout, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly52b_Breaks, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly61a_Lyrics, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly61b_MultipleLyrics, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly61c_Lyrics_Pianostaff, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly61d_Lyrics_Melisma, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly61e_Lyrics_Chords, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly61f_Lyrics_GracedNotes, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly61g_Lyrics_NameNumber, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly61h_Lyrics_BeamsMelismata, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly61i_Lyrics_Chords, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly61j_Lyrics_Elisions, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly61k_Lyrics_SpannersExtenders, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly71a_Chordnames, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly71c_ChordsFrets, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly71d_ChordsFrets_Multistaff, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly71e_TabStaves, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly71f_AllChordTypes, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly71g_MultipleChordnames, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly72a_TransposingInstruments, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly72b_TransposingInstruments_Full, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly72c_TransposingInstruments_Change, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly73a_Percussion, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly74a_FiguredBass, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly75a_AccordionRegistrations, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly99a_Sibelius5_IgnoreBeaming, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( lysuite, ly99b_Lyrics_BeamsMelismata_IgnoreBeams, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( mjbsuite, ezinar, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( mjbsuite, freezing, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END
//#endif

MXTEST_API_LOAD( mjbsuite, hello_timewise, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( mjbsuite, lyre_timewise, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, A_Walk_through_the_Park, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, different_keysigs_1, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, different_keysigs_2, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, test_harmony, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, test98, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testAccidentals1, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testAccidentals2, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testArpGliss1, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testArpGliss2, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testBarStyles, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testChord, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testChordDiagrams1, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testChordNoVoice, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testChordNoVoice_ref, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testClefs1, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testCompleteMeasureRests, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testDalSegno, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testDCalCoda, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testDCalFine, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testDirections1, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testDrumset1, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testDrumset2, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testDurationRoundingError, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testDurationRoundingError_ref, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testDynamics1, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testDynamics2, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testDynamics3, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testDynamics3_ref, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testEmptyMeasure, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testEmptyMeasure_ref, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testEmptyVoice1, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testEmptyVoice1_ref, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testFiguredBass1, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testFiguredBass2, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testGrace1, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testGrace2, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testHarmony1, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testHarmony2, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testHarmony3, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testHello, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testImplicitMeasure1, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testInvalid, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testInvisibleElements, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testKeysig1, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testLineBreaks, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testLines1, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testLines1_ref, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testLines2, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testLines2_ref, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testLyricsVoice2a, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testLyricsVoice2b, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testLyricsVoice2b_ref, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testManualBreaks, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testMeasureLength, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testMeasureLength_ref, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testMetaData, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testMultiMeasureRest1, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testMultiMeasureRest1_ref, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testMultiMeasureRest2, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testMultiMeasureRest2_ref, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testMultiMeasureRest3, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testMultiMeasureRest3_ref, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testMultipleNotations, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testMultipleNotations_ref, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testNonUniqueThings, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testNonUniqueThings_ref, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testNoteAttributes1, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testNoteAttributes2, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testNoteAttributes2_ref, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testNoteAttributes3, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testNoteheads, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testNotesRests1, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testNotesRests2, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testNumberedLyrics, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testNumberedLyrics_ref, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testPartsSpecialCases, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testPartsSpecialCases_ref, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testPiano, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testRestsNoType, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testRestsNoType_ref, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testSlurs, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testStaffTwoKeySigs, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testStringVoiceName, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testStringVoiceName_ref, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testSystemBrackets1, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testSystemBrackets2, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testTablature1, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testTablature2, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testTablature3, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testTempo1, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testTimesig1, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testTimesig3, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testTremolo, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testTuplets1, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testTuplets1_ref, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testTuplets2, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testTuplets2_ref, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testTuplets3, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testTuplets3_ref, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testTuplets4, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testUnusualDurations, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testUnusualDurations_ref, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testVoiceMapper1, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testVoiceMapper1_ref, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testVoiceMapper2, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testVoiceMapper2_ref, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testVoiceMapper3, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testVoiceMapper3_ref, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testVoicePiano1, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testVolta1, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testWedge1, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testWedge2, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testWedge2_ref, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( musuite, testWords1, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( recsuite, ActorPreludeSample, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( recsuite, BeetAnGeSample, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( recsuite, Binchois, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( recsuite, BrahWiMeSample, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( recsuite, BrookeWestSample, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( recsuite, Chant, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( recsuite, DebuMandSample, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( recsuite, Dichterliebe01, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( recsuite, Echigo_Jishi, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( recsuite, FaurReveSample, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( recsuite, MahlFaGe4Sample, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( recsuite, MozaChloSample, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( recsuite, MozaChloSample2, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( recsuite, MozartPianoSonata, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END
//#endif

MXTEST_API_LOAD( recsuite, MozartTrio, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( recsuite, MozaVeilSample, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( recsuite, Saltarello, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END

//#endif
MXTEST_API_LOAD( recsuite, SchbAvMaSample, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( recsuite, Telemann, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( ksuite, k001a_Articulations, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( ksuite, k001b_Articulations_Above, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( ksuite, k001c_Articulations_Below, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( ksuite, k002a_Fermatas, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( ksuite, k003a_Ornaments, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( ksuite, k004a_Technical, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( ksuite, k005a_Spanners_Slides, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( ksuite, k006a_Header_Scaling_Decimals, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END

//#endif
MXTEST_API_LOAD( ksuite, k007a_Notations_Dynamics, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( logicpro, logic01a_homoSapiens, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END


MXTEST_API_LOAD( foundsuite, Schubert_der_Mueller, xml );
bool isSuccess = runApiLoaderTest( suiteName, fileNamePart, fileExtensionPart );
CHECK( isSuccess );
MXTEST_API_LOAD_END

#endif
