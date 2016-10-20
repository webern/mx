// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_IMPORT_TESTS

#include "mxtest/control/Path.h"
#include "cpul/cpulTestHarness.h"
#include "ExpectedFiles.h"
#include "mxtest/import/ImportTestImpl.h"

#include <cstdio>
#include <fstream>

using namespace mx::xml;
using namespace MxTest;

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//#define MXTEST_DELETE_EXPECTED_FILES
#ifdef MXTEST_DELETE_EXPECTED_FILES

    TEST( DeleteExpected, Import )
    {
        MxTest::deleteExpectedFiles();
    }
    T_END

#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MXTEST_GENERATE_EXPECTED_FILES
#ifdef MXTEST_GENERATE_EXPECTED_FILES

    TEST( GenerateExpected, Import )
    {
        MxTest::generateExpectedFiles();
    }
    T_END

#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MXTEST_DELETE_TEST_OUTPUT_FILES
#ifdef MXTEST_DELETE_TEST_OUTPUT_FILES

    TEST( DeleteTestOutputFiles, Import )
    {
        MxTest::deleteTestOutputFiles();
    }
    T_END

#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MXTEST_CREATE_CSV_FILE
#ifdef MXTEST_CREATE_CSV_FILE

    TEST( CreateCsvFile, Import )
    {
        std::string csvFilePath = MxTest::getTestRecordFilepath();
        deleteFileNoThrow( csvFilePath );
        std::ofstream ofile;
        openAppendFile( ofile, csvFilePath );
        ofile << MxTest::ImportRoundTripResult::getHeaderRow() << std::endl;
        ofile.close();
    }
    T_END

#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////

class ImportTests
{
public:
    ImportTests()
    {
        TestFiles t;
        auto files = t.getTestFiles();
        for( const auto& file : files )
        {
            tests.emplace_back( file );
        }
    }
    std::vector<ImportTestCpul> tests;
} importTestsInstance;

#if 1==0
MXTEST_IMPORT_ROUNDTRIP( lysuite, ly01a_Pitches_Pitches, xml , MxTest::RESOURCES_DIRECTORY_PATH)
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly01b_Pitches_Intervals, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly01c_Pitches_NoVoiceElement, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly01d_Pitches_Microtones, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END

//#endif
MXTEST_IMPORT_ROUNDTRIP( lysuite, ly01e_Pitches_ParenthesizedAccidentals, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly01f_Pitches_ParenthesizedMicrotoneAccidentals, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly02a_Rests_Durations, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly02b_Rests_PitchedRests, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly02c_Rests_MultiMeasureRests, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly02d_Rests_Multimeasure_TimeSignatures, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly02e_Rests_NoType, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly03a_Rhythm_Durations, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly03b_Rhythm_Backup, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly03c_Rhythm_DivisionChange, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly03d_Rhythm_DottedDurations_Factors, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly11a_TimeSignatures, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly11b_TimeSignatures_NoTime, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly11c_TimeSignatures_CompoundSimple, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly11d_TimeSignatures_CompoundMultiple, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly11e_TimeSignatures_CompoundMixed, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly11f_TimeSignatures_SymbolMeaning, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly11g_TimeSignatures_SingleNumber, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly11h_TimeSignatures_SenzaMisura, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly12a_Clefs, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly12b_Clefs_NoKeyOrClef, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly13a_KeySignatures, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly13b_KeySignatures_ChurchModes, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly13c_KeySignatures_NonTraditional, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly13d_KeySignatures_Microtones, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly14a_StaffDetails_LineChanges, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END

//#endif
MXTEST_IMPORT_ROUNDTRIP( lysuite, ly21a_Chord_Basic, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly21b_Chords_TwoNotes, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly21c_Chords_ThreeNotesDuration, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly21d_Chords_SchubertStabatMater, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly21e_Chords_PickupMeasures, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly21f_Chord_ElementInBetween, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly22a_Noteheads, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly22b_Staff_Notestyles, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly22c_Noteheads_Chords, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly22d_Parenthesized_Noteheads, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly23a_Tuplets, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly23b_Tuplets_Styles, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly23c_Tuplet_Display_NonStandard, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly23d_Tuplets_Nested, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END
//#endif

MXTEST_IMPORT_ROUNDTRIP( lysuite, ly23e_Tuplets_Tremolo, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly23f_Tuplets_DurationButNoBracket, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly24a_GraceNotes, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly24b_ChordAsGraceNote, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly24c_GraceNote_MeasureEnd, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly24d_AfterGrace, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly24e_GraceNote_StaffChange, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly24f_GraceNote_Slur, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly31a_Directions, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END
//#endif

MXTEST_IMPORT_ROUNDTRIP( lysuite, ly31c_MetronomeMarks, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly32a_Notations, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly32b_Articulations_Texts, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly32c_MultipleNotationChildren, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly32d_Arpeggio, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END
//#endif

MXTEST_IMPORT_ROUNDTRIP( lysuite, ly33a_Spanners, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly33b_Spanners_Tie, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly33c_Spanners_Slurs, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly33d_Spanners_OctaveShifts, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly33e_Spanners_OctaveShifts_InvalidSize, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly33f_Trill_EndingOnGraceNote, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly33g_Slur_ChordedNotes, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly33h_Spanners_Glissando, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly33i_Ties_NotEnded, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly41a_MultiParts_Partorder, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly41b_MultiParts_MoreThan10, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly41c_StaffGroups, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly41d_StaffGroups_Nested, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly41e_StaffGroups_InstrumentNames_Linebroken, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly41f_StaffGroups_Overlapping, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END
//#endif

MXTEST_IMPORT_ROUNDTRIP( lysuite, ly41g_PartNoId, xml , MxTest::RESOURCES_DIRECTORY_PATH);
    // unusual test case - this file has a part which is missing the 'id' attribute.
    // in this case we expect mx to return false from the fromXElement function.
    isSuccess = test.runTest( msgsstr );
    CHECK( !isSuccess )
    const std::string m = msgsstr.str();
    CHECK_EQUAL( "PartAttributes: 'id' is a required", m.substr(0,34) );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly41h_TooManyParts, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly41i_PartNameDisplay_Override, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly42a_MultiVoice_TwoVoicesOnStaff_Lyrics, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly42b_MultiVoice_MidMeasureClefChange, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly43a_PianoStaff, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly43b_MultiStaff_DifferentKeys, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly43c_MultiStaff_DifferentKeysAfterBackup, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly43d_MultiStaff_StaffChange, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly43e_Multistaff_ClefDynamics, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly45a_SimpleRepeat, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly45b_RepeatWithAlternatives, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly45c_RepeatMultipleTimes, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly45d_Repeats_Nested_Alternatives, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly45e_Repeats_Nested_Alternatives, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly45f_Repeats_InvalidEndings, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly45g_Repeats_NotEnded, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly46a_Barlines, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly46b_MidmeasureBarline, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly46c_Midmeasure_Clef, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly46d_PickupMeasure_ImplicitMeasures, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly46e_PickupMeasure_SecondVoiceStartsLater, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly46f_IncompleteMeasures, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly46g_PickupMeasure_Chordnames_FiguredBass, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly51b_Header_Quotes, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly51c_MultipleRights, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly51d_EmptyTitle, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly52a_PageLayout, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly52b_Breaks, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly61a_Lyrics, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly61b_MultipleLyrics, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly61c_Lyrics_Pianostaff, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly61d_Lyrics_Melisma, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly61e_Lyrics_Chords, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly61f_Lyrics_GracedNotes, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly61g_Lyrics_NameNumber, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly61h_Lyrics_BeamsMelismata, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly61i_Lyrics_Chords, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly61j_Lyrics_Elisions, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly61k_Lyrics_SpannersExtenders, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly71a_Chordnames, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly71c_ChordsFrets, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly71d_ChordsFrets_Multistaff, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly71e_TabStaves, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly71f_AllChordTypes, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly71g_MultipleChordnames, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly72a_TransposingInstruments, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly72b_TransposingInstruments_Full, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly72c_TransposingInstruments_Change, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly73a_Percussion, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly74a_FiguredBass, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly75a_AccordionRegistrations, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly99a_Sibelius5_IgnoreBeaming, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( lysuite, ly99b_Lyrics_BeamsMelismata_IgnoreBeams, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( mjbsuite, ezinar, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( mjbsuite, freezing, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END
//#endif

MXTEST_IMPORT_ROUNDTRIP( mjbsuite, hello_timewise, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( mjbsuite, lyre_timewise, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, A_Walk_through_the_Park, xml , MxTest::RESOURCES_DIRECTORY_PATH);
    isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, different_keysigs_1, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, different_keysigs_2, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, test_harmony, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, test98, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testAccidentals1, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testAccidentals2, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testArpGliss1, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testArpGliss2, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testBarStyles, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testChord, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testChordDiagrams1, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testChordNoVoice, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testChordNoVoice_ref, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testClefs1, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testCompleteMeasureRests, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testDalSegno, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testDCalCoda, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testDCalFine, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testDirections1, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testDrumset1, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testDrumset2, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testDurationRoundingError, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testDurationRoundingError_ref, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testDynamics1, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testDynamics2, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testDynamics3, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testDynamics3_ref, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testEmptyMeasure, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testEmptyMeasure_ref, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testEmptyVoice1, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testEmptyVoice1_ref, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testFiguredBass1, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testFiguredBass2, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testGrace1, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testGrace2, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testHarmony1, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testHarmony2, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testHarmony3, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testHello, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testImplicitMeasure1, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testInvalid, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testInvisibleElements, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testKeysig1, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testLineBreaks, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testLines1, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testLines1_ref, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testLines2, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testLines2_ref, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testLyricsVoice2a, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testLyricsVoice2b, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testLyricsVoice2b_ref, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testManualBreaks, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testMeasureLength, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testMeasureLength_ref, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testMetaData, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testMultiMeasureRest1, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testMultiMeasureRest1_ref, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testMultiMeasureRest2, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testMultiMeasureRest2_ref, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testMultiMeasureRest3, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testMultiMeasureRest3_ref, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testMultipleNotations, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testMultipleNotations_ref, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testNonUniqueThings, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testNonUniqueThings_ref, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testNoteAttributes1, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testNoteAttributes2, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testNoteAttributes2_ref, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testNoteAttributes3, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testNoteheads, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testNotesRests1, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testNotesRests2, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testNumberedLyrics, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testNumberedLyrics_ref, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testPartsSpecialCases, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testPartsSpecialCases_ref, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testPiano, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testRestsNoType, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testRestsNoType_ref, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testSlurs, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testStaffTwoKeySigs, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testStringVoiceName, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testStringVoiceName_ref, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testSystemBrackets1, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testSystemBrackets2, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testTablature1, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testTablature2, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testTablature3, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testTempo1, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testTimesig1, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testTimesig3, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testTremolo, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testTuplets1, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testTuplets1_ref, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testTuplets2, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testTuplets2_ref, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testTuplets3, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testTuplets3_ref, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testTuplets4, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testUnusualDurations, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testUnusualDurations_ref, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testVoiceMapper1, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testVoiceMapper1_ref, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testVoiceMapper2, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testVoiceMapper2_ref, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testVoiceMapper3, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testVoiceMapper3_ref, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testVoicePiano1, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testVolta1, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testWedge1, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testWedge2, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testWedge2_ref, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( musuite, testWords1, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( recsuite, ActorPreludeSample, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( recsuite, BeetAnGeSample, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( recsuite, Binchois, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( recsuite, BrahWiMeSample, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( recsuite, BrookeWestSample, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( recsuite, Chant, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( recsuite, DebuMandSample, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( recsuite, Dichterliebe01, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( recsuite, Echigo_Jishi, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( recsuite, FaurReveSample, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( recsuite, MahlFaGe4Sample, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( recsuite, MozaChloSample, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( recsuite, MozaChloSample2, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( recsuite, MozartPianoSonata, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END
//#endif

MXTEST_IMPORT_ROUNDTRIP( recsuite, MozartTrio, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( recsuite, MozaVeilSample, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( recsuite, Saltarello, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END

//#endif
MXTEST_IMPORT_ROUNDTRIP( recsuite, SchbAvMaSample, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( recsuite, Telemann, xml , MxTest::RESOURCES_DIRECTORY_PATH);
	isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( ksuite, k001a_Articulations, xml , MxTest::RESOURCES_DIRECTORY_PATH);
    isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( ksuite, k001b_Articulations_Above, xml , MxTest::RESOURCES_DIRECTORY_PATH);
    isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( ksuite, k001c_Articulations_Below, xml , MxTest::RESOURCES_DIRECTORY_PATH);
    isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( ksuite, k002a_Fermatas, xml , MxTest::RESOURCES_DIRECTORY_PATH);
    isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( ksuite, k003a_Ornaments, xml , MxTest::RESOURCES_DIRECTORY_PATH);
    isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( ksuite, k004a_Technical, xml , MxTest::RESOURCES_DIRECTORY_PATH);
    isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( ksuite, k005a_Spanners_Slides, xml , MxTest::RESOURCES_DIRECTORY_PATH);
    isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( ksuite, k006a_Header_Scaling_Decimals, xml , MxTest::RESOURCES_DIRECTORY_PATH);
    isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( ksuite, k007a_Notations_Dynamics, xml , MxTest::RESOURCES_DIRECTORY_PATH);
    isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( logicpro, logic01a_homoSapiens, xml , MxTest::RESOURCES_DIRECTORY_PATH);
    isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END


MXTEST_IMPORT_ROUNDTRIP( foundsuite, Schubert_der_Mueller, xml , MxTest::RESOURCES_DIRECTORY_PATH);
    isSuccess = test.runTest( msgsstr );
    CHECK_WITH_MESSAGE( isSuccess, msgsstr.str() );
MXTEST_IMPORT_ROUNDTRIP_END

#endif


#endif
