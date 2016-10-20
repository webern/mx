// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/file/MxFileRepository.h"
#include "mxtest/control/Path.h"

#include <fstream>

namespace mxtest
{

    MxFileRepository::MxFileRepository()
    : myPath{ RESOURCES_DIRECTORY_PATH }
    , mySeparator{ FILE_PATH_SEPARATOR }
    , myExpectedLoadFailures{}
    , myNameSubdirectoryMap{}
    , myTestFiles{}
    {
        initializeNameSubdirectoryMap();
        initializeExpectedLoadFailures();
        
        // should run last
        initializeTestFiles();
    }

    
    const std::string MxFileRepository::getFullPath( const std::string& fileName )
    {
        const auto& subDir = getSubdirectory( fileName );
        if( subDir.empty() )
        {
            throw std::runtime_error{ "TestFiles::getFullPath - fileName not found" };
        }
        const auto& instance = getInstance();
        return instance.myPath + subDir + instance.mySeparator + fileName;
    }
    
    
    const std::string& MxFileRepository::getSubdirectory( const std::string& fileName )
    {
        const auto& instance = getInstance();
        const auto iter = instance.myNameSubdirectoryMap.find( fileName );
        if( iter == instance.myNameSubdirectoryMap.cend() )
        {
            static std::string emptyness;
            return emptyness;
        }
        return iter->second;
    }
    
    
    MxFiles MxFileRepository::getTestFiles( int maxFileSizeBytes )
    {
        MxFiles outFiles;
        for( const auto& file : getTestFiles() )
        {
            if( maxFileSizeBytes <= 0 || file.sizeBytes <= maxFileSizeBytes )
            {
                outFiles.push_back( file );
            }
        }
        return outFiles;
    }
    
    
    const MxFiles& MxFileRepository::getTestFiles()
    {
        const auto& instance = getInstance();
        return instance.myTestFiles;
    }
    
    
    const MxFileRepository& MxFileRepository::getInstance()
    {
        static MxFileRepository ourInstance;
        return ourInstance;
    }
    
    
    void MxFileRepository::initializeExpectedLoadFailures()
    {
        myExpectedLoadFailures.emplace( "ly41g_PartNoId.xml" );
    }
    
    
    inline int MxFileRepositoryGetTheFilesize(const char* filename)
    {
        std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
        return static_cast<int>( in.tellg() );
    }

    
    void MxFileRepository::initializeTestFiles()
    {
        for( const auto& stringPair : myNameSubdirectoryMap )
        {
            MxFile file;
            file.fileName = stringPair.first;
            file.subdirectory = stringPair.second;
            file.path = myPath + file.subdirectory + mySeparator + file.fileName;
            file.isLoadFailureExpected = ( myExpectedLoadFailures.find( file.fileName ) ) != myExpectedLoadFailures.cend();
            file.sizeBytes = MxFileRepositoryGetTheFilesize( file.path.c_str() );
            myTestFiles.emplace_back( std::move( file ) );
        }
    }
    
    
    void MxFileRepository::initializeNameSubdirectoryMap()
    {
       // lily files
        myNameSubdirectoryMap.emplace( "ly01a_Pitches_Pitches.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly01b_Pitches_Intervals.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly01c_Pitches_NoVoiceElement.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly01d_Pitches_Microtones.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly01e_Pitches_ParenthesizedAccidentals.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly01f_Pitches_ParenthesizedMicrotoneAccidentals.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly02a_Rests_Durations.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly02b_Rests_PitchedRests.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly02c_Rests_MultiMeasureRests.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly02d_Rests_Multimeasure_TimeSignatures.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly02e_Rests_NoType.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly03a_Rhythm_Durations.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly03b_Rhythm_Backup.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly03c_Rhythm_DivisionChange.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly03d_Rhythm_DottedDurations_Factors.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly11a_TimeSignatures.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly11b_TimeSignatures_NoTime.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly11c_TimeSignatures_CompoundSimple.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly11d_TimeSignatures_CompoundMultiple.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly11e_TimeSignatures_CompoundMixed.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly11f_TimeSignatures_SymbolMeaning.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly11g_TimeSignatures_SingleNumber.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly11h_TimeSignatures_SenzaMisura.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly12a_Clefs.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly12b_Clefs_NoKeyOrClef.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly13a_KeySignatures.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly13b_KeySignatures_ChurchModes.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly13c_KeySignatures_NonTraditional.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly13d_KeySignatures_Microtones.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly14a_StaffDetails_LineChanges.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly21a_Chord_Basic.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly21b_Chords_TwoNotes.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly21c_Chords_ThreeNotesDuration.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly21d_Chords_SchubertStabatMater.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly21e_Chords_PickupMeasures.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly21f_Chord_ElementInBetween.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly22a_Noteheads.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly22b_Staff_Notestyles.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly22c_Noteheads_Chords.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly22d_Parenthesized_Noteheads.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly23a_Tuplets.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly23b_Tuplets_Styles.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly23c_Tuplet_Display_NonStandard.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly23d_Tuplets_Nested.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly23e_Tuplets_Tremolo.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly23f_Tuplets_DurationButNoBracket.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly24a_GraceNotes.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly24b_ChordAsGraceNote.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly24c_GraceNote_MeasureEnd.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly24d_AfterGrace.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly24e_GraceNote_StaffChange.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly24f_GraceNote_Slur.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly31a_Directions.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly31c_MetronomeMarks.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly32a_Notations.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly32b_Articulations_Texts.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly32c_MultipleNotationChildren.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly32d_Arpeggio.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly33a_Spanners.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly33b_Spanners_Tie.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly33c_Spanners_Slurs.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly33d_Spanners_OctaveShifts.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly33e_Spanners_OctaveShifts_InvalidSize.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly33f_Trill_EndingOnGraceNote.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly33g_Slur_ChordedNotes.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly33h_Spanners_Glissando.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly33i_Ties_NotEnded.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly41a_MultiParts_Partorder.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly41b_MultiParts_MoreThan10.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly41c_StaffGroups.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly41d_StaffGroups_Nested.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly41e_StaffGroups_InstrumentNames_Linebroken.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly41f_StaffGroups_Overlapping.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly41g_PartNoId.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly41h_TooManyParts.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly41i_PartNameDisplay_Override.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly42a_MultiVoice_TwoVoicesOnStaff_Lyrics.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly42b_MultiVoice_MidMeasureClefChange.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly43a_PianoStaff.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly43b_MultiStaff_DifferentKeys.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly43c_MultiStaff_DifferentKeysAfterBackup.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly43d_MultiStaff_StaffChange.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly43e_Multistaff_ClefDynamics.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly45a_SimpleRepeat.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly45b_RepeatWithAlternatives.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly45c_RepeatMultipleTimes.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly45d_Repeats_Nested_Alternatives.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly45e_Repeats_Nested_Alternatives.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly45f_Repeats_InvalidEndings.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly45g_Repeats_NotEnded.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly46a_Barlines.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly46b_MidmeasureBarline.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly46c_Midmeasure_Clef.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly46d_PickupMeasure_ImplicitMeasures.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly46e_PickupMeasure_SecondVoiceStartsLater.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly46f_IncompleteMeasures.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly46g_PickupMeasure_Chordnames_FiguredBass.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly51b_Header_Quotes.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly51c_MultipleRights.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly51d_EmptyTitle.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly52a_PageLayout.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly52b_Breaks.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly61a_Lyrics.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly61b_MultipleLyrics.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly61c_Lyrics_Pianostaff.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly61d_Lyrics_Melisma.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly61e_Lyrics_Chords.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly61f_Lyrics_GracedNotes.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly61g_Lyrics_NameNumber.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly61h_Lyrics_BeamsMelismata.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly61i_Lyrics_Chords.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly61j_Lyrics_Elisions.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly61k_Lyrics_SpannersExtenders.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly71a_Chordnames.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly71c_ChordsFrets.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly71d_ChordsFrets_Multistaff.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly71e_TabStaves.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly71f_AllChordTypes.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly71g_MultipleChordnames.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly72a_TransposingInstruments.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly72b_TransposingInstruments_Full.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly72c_TransposingInstruments_Change.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly73a_Percussion.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly74a_FiguredBass.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly75a_AccordionRegistrations.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly99a_Sibelius5_IgnoreBeaming.xml", "lysuite" );
        myNameSubdirectoryMap.emplace( "ly99b_Lyrics_BeamsMelismata_IgnoreBeams.xml", "lysuite" );
        
        // mjb files
        myNameSubdirectoryMap.emplace( "ezinar.xml", "mjbsuite" );
        myNameSubdirectoryMap.emplace( "freezing.xml", "mjbsuite" );
        myNameSubdirectoryMap.emplace( "hello_timewise.xml", "mjbsuite" );
        myNameSubdirectoryMap.emplace( "lyre_timewise.xml", "mjbsuite" );
        
        // museScore
        myNameSubdirectoryMap.emplace( "A_Walk_through_the_Park.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "different_keysigs_1.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "different_keysigs_2.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "test_harmony.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "test98.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testAccidentals1.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testAccidentals2.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testArpGliss1.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testArpGliss2.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testBarStyles.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testChord.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testChordDiagrams1.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testChordNoVoice.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testChordNoVoice_ref.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testClefs1.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testCompleteMeasureRests.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testDalSegno.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testDCalCoda.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testDCalFine.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testDirections1.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testDrumset1.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testDrumset2.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testDurationRoundingError.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testDurationRoundingError_ref.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testDynamics1.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testDynamics2.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testDynamics3.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testDynamics3_ref.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testEmptyMeasure.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testEmptyMeasure_ref.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testEmptyVoice1.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testEmptyVoice1_ref.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testFiguredBass1.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testFiguredBass2.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testGrace1.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testGrace2.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testHarmony1.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testHarmony2.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testHarmony3.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testHello.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testImplicitMeasure1.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testInvalid.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testInvisibleElements.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testKeysig1.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testLineBreaks.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testLines1.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testLines1_ref.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testLines2.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testLines2_ref.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testLyricsVoice2a.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testLyricsVoice2b.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testLyricsVoice2b_ref.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testManualBreaks.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testMeasureLength.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testMeasureLength_ref.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testMetaData.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testMultiMeasureRest1.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testMultiMeasureRest1_ref.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testMultiMeasureRest2.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testMultiMeasureRest2_ref.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testMultiMeasureRest3.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testMultiMeasureRest3_ref.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testMultipleNotations.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testMultipleNotations_ref.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testNonUniqueThings.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testNonUniqueThings_ref.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testNoteAttributes1.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testNoteAttributes2.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testNoteAttributes2_ref.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testNoteAttributes3.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testNoteheads.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testNotesRests1.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testNotesRests2.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testNumberedLyrics.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testNumberedLyrics_ref.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testPartsSpecialCases.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testPartsSpecialCases_ref.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testPiano.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testRestsNoType.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testRestsNoType_ref.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testSlurs.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testStaffTwoKeySigs.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testStringVoiceName.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testStringVoiceName_ref.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testSystemBrackets1.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testSystemBrackets2.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testTablature1.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testTablature2.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testTablature3.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testTempo1.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testTimesig1.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testTimesig3.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testTremolo.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testTuplets1.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testTuplets1_ref.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testTuplets2.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testTuplets2_ref.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testTuplets3.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testTuplets3_ref.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testTuplets4.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testUnusualDurations.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testUnusualDurations_ref.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testVoiceMapper1.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testVoiceMapper1_ref.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testVoiceMapper2.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testVoiceMapper2_ref.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testVoiceMapper3.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testVoiceMapper3_ref.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testVoicePiano1.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testVolta1.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testWedge1.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testWedge2.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testWedge2_ref.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "testWords1.xml", "musuite" );
        myNameSubdirectoryMap.emplace( "ActorPreludeSample.xml", "recsuite" );
        myNameSubdirectoryMap.emplace( "BeetAnGeSample.xml", "recsuite" );
        myNameSubdirectoryMap.emplace( "Binchois.xml", "recsuite" );
        myNameSubdirectoryMap.emplace( "BrahWiMeSample.xml", "recsuite" );
        myNameSubdirectoryMap.emplace( "BrookeWestSample.xml", "recsuite" );
        myNameSubdirectoryMap.emplace( "Chant.xml", "recsuite" );
        myNameSubdirectoryMap.emplace( "DebuMandSample.xml", "recsuite" );
        myNameSubdirectoryMap.emplace( "Dichterliebe01.xml", "recsuite" );
        myNameSubdirectoryMap.emplace( "Echigo_Jishi.xml", "recsuite" );
        myNameSubdirectoryMap.emplace( "FaurReveSample.xml", "recsuite" );
        myNameSubdirectoryMap.emplace( "MahlFaGe4Sample.xml", "recsuite" );
        myNameSubdirectoryMap.emplace( "MozaChloSample.xml", "recsuite" );
        myNameSubdirectoryMap.emplace( "MozaChloSample2.xml", "recsuite" );
        myNameSubdirectoryMap.emplace( "MozartPianoSonata.xml", "recsuite" );
        myNameSubdirectoryMap.emplace( "MozartTrio.xml", "recsuite" );
        myNameSubdirectoryMap.emplace( "MozaVeilSample.xml", "recsuite" );
        myNameSubdirectoryMap.emplace( "Saltarello.xml", "recsuite" );
        myNameSubdirectoryMap.emplace( "SchbAvMaSample.xml", "recsuite" );
        myNameSubdirectoryMap.emplace( "Telemann.xml", "recsuite" );
        
        // ksuite
        myNameSubdirectoryMap.emplace( "k001a_Articulations.xml", "ksuite" );
        myNameSubdirectoryMap.emplace( "k001b_Articulations_Above.xml", "ksuite" );
        myNameSubdirectoryMap.emplace( "k001c_Articulations_Below.xml", "ksuite" );
        myNameSubdirectoryMap.emplace( "k002a_Fermatas.xml", "ksuite" );
        myNameSubdirectoryMap.emplace( "k003a_Ornaments.xml", "ksuite" );
        myNameSubdirectoryMap.emplace( "k004a_Technical.xml", "ksuite" );
        myNameSubdirectoryMap.emplace( "k005a_Spanners_Slides.xml", "ksuite" );
        myNameSubdirectoryMap.emplace( "k006a_Clefs_sign_G_ln_2_oct_0_Treble.xml", "ksuite" );
        myNameSubdirectoryMap.emplace( "k006a_Header_Scaling_Decimals.xml", "ksuite" );
        myNameSubdirectoryMap.emplace( "k006b_Clefs_sign_F_ln_4_oct_0_Bass.xml", "ksuite" );
        myNameSubdirectoryMap.emplace( "k006c_Clefs_sign_C_ln_3_oct_0_Viola.xml", "ksuite" );
        myNameSubdirectoryMap.emplace( "k006d_Clefs_sign_C_ln_4_oct_0_Tenor.xml", "ksuite" );
        myNameSubdirectoryMap.emplace( "k006e_Clefs_sign_C_ln_5_oct_0_Baritone.xml", "ksuite" );
        myNameSubdirectoryMap.emplace( "k006f_Clefs_sign_C_ln_1_oct_0_Soprano.xml", "ksuite" );
        myNameSubdirectoryMap.emplace( "k006g_Clefs_sign_C_ln_2_oct_0_Mezzo_Soprano.xml", "ksuite" );
        myNameSubdirectoryMap.emplace( "k006h_Clefs_sign_G_ln_1_oct_0_French.xml", "ksuite" );
        myNameSubdirectoryMap.emplace( "k006i_Clefs_sign_F_ln_3_oct_0_Varbaritone.xml", "ksuite" );
        myNameSubdirectoryMap.emplace( "k006j_Clefs_sign_F_ln_5_oct_0_Subbass.xml", "ksuite" );
        myNameSubdirectoryMap.emplace( "k006k_Clefs_sign_G_ln_2_oct_neg2_G_15MB.xml", "ksuite" );
        myNameSubdirectoryMap.emplace( "k006l_Clefs_sign_G_ln_2_oct_neg1_G_8VB.xml", "ksuite" );
        myNameSubdirectoryMap.emplace( "k006m_Clefs_sign_G_ln_2_oct_1_G_8VA.xml", "ksuite" );
        myNameSubdirectoryMap.emplace( "k006n_Clefs_sign_G_ln_2_oct_2_G_15MA.xml", "ksuite" );
        myNameSubdirectoryMap.emplace( "k006o_Clefs_sign_F_ln_4_oct_neg2_F_15MB.xml", "ksuite" );
        myNameSubdirectoryMap.emplace( "k006p_Clefs_sign_F_ln_4_oct_neg1_F_8VB.xml", "ksuite" );
        myNameSubdirectoryMap.emplace( "k006q_Clefs_sign_F_ln_4_oct_1_F_8VA.xml", "ksuite" );
        myNameSubdirectoryMap.emplace( "k006r_Clefs_sign_F_ln_4_oct_2_F_15MA.xml", "ksuite" );
        myNameSubdirectoryMap.emplace( "k006s_Clefs_sign_percussion_ln_3_oct_0_Percussion.xml", "ksuite" );
        myNameSubdirectoryMap.emplace( "k006t_Clefs_sign_TAB_ln_3_oct_0_Tab.xml", "ksuite" );
        myNameSubdirectoryMap.emplace( "k006u_Clefs_sign_jianpu_ln_3_oct_0_Jianpu.xml", "ksuite" );
        myNameSubdirectoryMap.emplace( "k006v_Clefs_sign_none_ln_3_oct_0_None.xml", "ksuite" );
        myNameSubdirectoryMap.emplace( "k007a_Notations_Dynamics.xml", "ksuite" );
        myNameSubdirectoryMap.emplace( "k008a_Beaming.xml", "ksuite" );
        myNameSubdirectoryMap.emplace( "k009a_Slurs.xml", "ksuite" );
        myNameSubdirectoryMap.emplace( "k010a_Ties.xml", "ksuite" );
        myNameSubdirectoryMap.emplace( "k010b_Bad_Ties.xml", "ksuite" );
        myNameSubdirectoryMap.emplace( "k011a_Tuplets.xml", "ksuite" );
        myNameSubdirectoryMap.emplace( "k012a_Tempo_Markings.xml", "ksuite" );
        myNameSubdirectoryMap.emplace( "k013a_OrchestralScoreFinale.xml", "ksuite" );
        myNameSubdirectoryMap.emplace( "k013b_OrchestralScoreSibelius.xml", "ksuite" );
        
        // other
        myNameSubdirectoryMap.emplace( "logic01a_homoSapiens.xml", "logicpro" );
        myNameSubdirectoryMap.emplace( "Schubert_der_Mueller.xml", "foundsuite" );
    }
}
