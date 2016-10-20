// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/TestFiles.h"
#include "mxtest/control/Path.h"
#include <fstream>

namespace MxTest
{
    
    const StringMap& TestFiles::getTestFileMap() const
    {
        return myTestFiles;
    }
    
    
    const std::string TestFiles::getFullPath( const std::string& fileName ) const
    {
        const auto& subDir = getSubdirectory( fileName );
        if( subDir.empty() )
        {
            throw std::runtime_error{ "TestFiles::getFullPath - fileName not found" };
        }
        return myPath + subDir + mySeparator + fileName;
    }
    
    
    const std::string& TestFiles::getSubdirectory( const std::string& fileName ) const
    {
        const auto iter = myTestFiles.find( fileName );
        if( iter == myTestFiles.cend() )
        {
            static std::string emptyness;
            return emptyness;
        }
        return iter->second;
    }
    
    
    inline int getTheFilesize(const char* filename)
    {
        std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
        return static_cast<int>( in.tellg() );
    }
    
    
    std::vector<TestFile> TestFiles::getTestFiles( int maxFileSizeBytes ) const
    {
        std::vector<TestFile> outFiles;
        for( const auto& stringPair : myTestFiles )
        {
            TestFile testFile;
            testFile.fileName = stringPair.first;
            testFile.subdirectory = stringPair.second;
            testFile.path = getFullPath( stringPair.first );
            testFile.isLoadFailureExpected = ( myExpectedLoadFailures.find( testFile.fileName ) ) != myExpectedLoadFailures.cend();
            testFile.sizeBytes = getTheFilesize( testFile.path.c_str() );
            
            if( maxFileSizeBytes <= 0 || testFile.sizeBytes <= maxFileSizeBytes )
            {
                outFiles.emplace_back( std::move( testFile ) );
            }
        }
        return outFiles;
    }

    
    TestFiles::TestFiles()
    : myPath{ MxTest::RESOURCES_DIRECTORY_PATH }
    , mySeparator{ MxTest::FILE_PATH_SEPARATOR }
    , myTestFiles{
                    // lily files
                    { "ly01a_Pitches_Pitches.xml", "lysuite" },
                    { "ly01b_Pitches_Intervals.xml", "lysuite" },
                    { "ly01c_Pitches_NoVoiceElement.xml", "lysuite" },
                    { "ly01d_Pitches_Microtones.xml", "lysuite" },
                    { "ly01e_Pitches_ParenthesizedAccidentals.xml", "lysuite" },
                    { "ly01f_Pitches_ParenthesizedMicrotoneAccidentals.xml", "lysuite" },
                    { "ly02a_Rests_Durations.xml", "lysuite" },
                    { "ly02b_Rests_PitchedRests.xml", "lysuite" },
                    { "ly02c_Rests_MultiMeasureRests.xml", "lysuite" },
                    { "ly02d_Rests_Multimeasure_TimeSignatures.xml", "lysuite" },
                    { "ly02e_Rests_NoType.xml", "lysuite" },
                    { "ly03a_Rhythm_Durations.xml", "lysuite" },
                    { "ly03b_Rhythm_Backup.xml", "lysuite" },
                    { "ly03c_Rhythm_DivisionChange.xml", "lysuite" },
                    { "ly03d_Rhythm_DottedDurations_Factors.xml", "lysuite" },
                    { "ly11a_TimeSignatures.xml", "lysuite" },
                    { "ly11b_TimeSignatures_NoTime.xml", "lysuite" },
                    { "ly11c_TimeSignatures_CompoundSimple.xml", "lysuite" },
                    { "ly11d_TimeSignatures_CompoundMultiple.xml", "lysuite" },
                    { "ly11e_TimeSignatures_CompoundMixed.xml", "lysuite" },
                    { "ly11f_TimeSignatures_SymbolMeaning.xml", "lysuite" },
                    { "ly11g_TimeSignatures_SingleNumber.xml", "lysuite" },
                    { "ly11h_TimeSignatures_SenzaMisura.xml", "lysuite" },
                    { "ly12a_Clefs.xml", "lysuite" },
                    { "ly12b_Clefs_NoKeyOrClef.xml", "lysuite" },
                    { "ly13a_KeySignatures.xml", "lysuite" },
                    { "ly13b_KeySignatures_ChurchModes.xml", "lysuite" },
                    { "ly13c_KeySignatures_NonTraditional.xml", "lysuite" },
                    { "ly13d_KeySignatures_Microtones.xml", "lysuite" },
                    { "ly14a_StaffDetails_LineChanges.xml", "lysuite" },
                    { "ly21a_Chord_Basic.xml", "lysuite" },
                    { "ly21b_Chords_TwoNotes.xml", "lysuite" },
                    { "ly21c_Chords_ThreeNotesDuration.xml", "lysuite" },
                    { "ly21d_Chords_SchubertStabatMater.xml", "lysuite" },
                    { "ly21e_Chords_PickupMeasures.xml", "lysuite" },
                    { "ly21f_Chord_ElementInBetween.xml", "lysuite" },
                    { "ly22a_Noteheads.xml", "lysuite" },
                    { "ly22b_Staff_Notestyles.xml", "lysuite" },
                    { "ly22c_Noteheads_Chords.xml", "lysuite" },
                    { "ly22d_Parenthesized_Noteheads.xml", "lysuite" },
                    { "ly23a_Tuplets.xml", "lysuite" },
                    { "ly23b_Tuplets_Styles.xml", "lysuite" },
                    { "ly23c_Tuplet_Display_NonStandard.xml", "lysuite" },
                    { "ly23d_Tuplets_Nested.xml", "lysuite" },
                    { "ly23e_Tuplets_Tremolo.xml", "lysuite" },
                    { "ly23f_Tuplets_DurationButNoBracket.xml", "lysuite" },
                    { "ly24a_GraceNotes.xml", "lysuite" },
                    { "ly24b_ChordAsGraceNote.xml", "lysuite" },
                    { "ly24c_GraceNote_MeasureEnd.xml", "lysuite" },
                    { "ly24d_AfterGrace.xml", "lysuite" },
                    { "ly24e_GraceNote_StaffChange.xml", "lysuite" },
                    { "ly24f_GraceNote_Slur.xml", "lysuite" },
                    { "ly31a_Directions.xml", "lysuite" },
                    { "ly31c_MetronomeMarks.xml", "lysuite" },
                    { "ly32a_Notations.xml", "lysuite" },
                    { "ly32b_Articulations_Texts.xml", "lysuite" },
                    { "ly32c_MultipleNotationChildren.xml", "lysuite" },
                    { "ly32d_Arpeggio.xml", "lysuite" },
                    { "ly33a_Spanners.xml", "lysuite" },
                    { "ly33b_Spanners_Tie.xml", "lysuite" },
                    { "ly33c_Spanners_Slurs.xml", "lysuite" },
                    { "ly33d_Spanners_OctaveShifts.xml", "lysuite" },
                    { "ly33e_Spanners_OctaveShifts_InvalidSize.xml", "lysuite" },
                    { "ly33f_Trill_EndingOnGraceNote.xml", "lysuite" },
                    { "ly33g_Slur_ChordedNotes.xml", "lysuite" },
                    { "ly33h_Spanners_Glissando.xml", "lysuite" },
                    { "ly33i_Ties_NotEnded.xml", "lysuite" },
                    { "ly41a_MultiParts_Partorder.xml", "lysuite" },
                    { "ly41b_MultiParts_MoreThan10.xml", "lysuite" },
                    { "ly41c_StaffGroups.xml", "lysuite" },
                    { "ly41d_StaffGroups_Nested.xml", "lysuite" },
                    { "ly41e_StaffGroups_InstrumentNames_Linebroken.xml", "lysuite" },
                    { "ly41f_StaffGroups_Overlapping.xml", "lysuite" },
                    { "ly41g_PartNoId.xml", "lysuite" },
                    { "ly41h_TooManyParts.xml", "lysuite" },
                    { "ly41i_PartNameDisplay_Override.xml", "lysuite" },
                    { "ly42a_MultiVoice_TwoVoicesOnStaff_Lyrics.xml", "lysuite" },
                    { "ly42b_MultiVoice_MidMeasureClefChange.xml", "lysuite" },
                    { "ly43a_PianoStaff.xml", "lysuite" },
                    { "ly43b_MultiStaff_DifferentKeys.xml", "lysuite" },
                    { "ly43c_MultiStaff_DifferentKeysAfterBackup.xml", "lysuite" },
                    { "ly43d_MultiStaff_StaffChange.xml", "lysuite" },
                    { "ly43e_Multistaff_ClefDynamics.xml", "lysuite" },
                    { "ly45a_SimpleRepeat.xml", "lysuite" },
                    { "ly45b_RepeatWithAlternatives.xml", "lysuite" },
                    { "ly45c_RepeatMultipleTimes.xml", "lysuite" },
                    { "ly45d_Repeats_Nested_Alternatives.xml", "lysuite" },
                    { "ly45e_Repeats_Nested_Alternatives.xml", "lysuite" },
                    { "ly45f_Repeats_InvalidEndings.xml", "lysuite" },
                    { "ly45g_Repeats_NotEnded.xml", "lysuite" },
                    { "ly46a_Barlines.xml", "lysuite" },
                    { "ly46b_MidmeasureBarline.xml", "lysuite" },
                    { "ly46c_Midmeasure_Clef.xml", "lysuite" },
                    { "ly46d_PickupMeasure_ImplicitMeasures.xml", "lysuite" },
                    { "ly46e_PickupMeasure_SecondVoiceStartsLater.xml", "lysuite" },
                    { "ly46f_IncompleteMeasures.xml", "lysuite" },
                    { "ly46g_PickupMeasure_Chordnames_FiguredBass.xml", "lysuite" },
                    { "ly51b_Header_Quotes.xml", "lysuite" },
                    { "ly51c_MultipleRights.xml", "lysuite" },
                    { "ly51d_EmptyTitle.xml", "lysuite" },
                    { "ly52a_PageLayout.xml", "lysuite" },
                    { "ly52b_Breaks.xml", "lysuite" },
                    { "ly61a_Lyrics.xml", "lysuite" },
                    { "ly61b_MultipleLyrics.xml", "lysuite" },
                    { "ly61c_Lyrics_Pianostaff.xml", "lysuite" },
                    { "ly61d_Lyrics_Melisma.xml", "lysuite" },
                    { "ly61e_Lyrics_Chords.xml", "lysuite" },
                    { "ly61f_Lyrics_GracedNotes.xml", "lysuite" },
                    { "ly61g_Lyrics_NameNumber.xml", "lysuite" },
                    { "ly61h_Lyrics_BeamsMelismata.xml", "lysuite" },
                    { "ly61i_Lyrics_Chords.xml", "lysuite" },
                    { "ly61j_Lyrics_Elisions.xml", "lysuite" },
                    { "ly61k_Lyrics_SpannersExtenders.xml", "lysuite" },
                    { "ly71a_Chordnames.xml", "lysuite" },
                    { "ly71c_ChordsFrets.xml", "lysuite" },
                    { "ly71d_ChordsFrets_Multistaff.xml", "lysuite" },
                    { "ly71e_TabStaves.xml", "lysuite" },
                    { "ly71f_AllChordTypes.xml", "lysuite" },
                    { "ly71g_MultipleChordnames.xml", "lysuite" },
                    { "ly72a_TransposingInstruments.xml", "lysuite" },
                    { "ly72b_TransposingInstruments_Full.xml", "lysuite" },
                    { "ly72c_TransposingInstruments_Change.xml", "lysuite" },
                    { "ly73a_Percussion.xml", "lysuite" },
                    { "ly74a_FiguredBass.xml", "lysuite" },
                    { "ly75a_AccordionRegistrations.xml", "lysuite" },
                    { "ly99a_Sibelius5_IgnoreBeaming.xml", "lysuite" },
                    { "ly99b_Lyrics_BeamsMelismata_IgnoreBeams.xml", "lysuite" },
                    
                    // mjb files
                    { "ezinar.xml", "mjbsuite" },
                    { "freezing.xml", "mjbsuite" },
                    { "hello_timewise.xml", "mjbsuite" },
                    { "lyre_timewise.xml", "mjbsuite" },
                    
                    // museScore
                    { "A_Walk_through_the_Park.xml", "musuite" },
                    { "different_keysigs_1.xml", "musuite" },
                    { "different_keysigs_2.xml", "musuite" },
                    { "test_harmony.xml", "musuite" },
                    { "test98.xml", "musuite" },
                    { "testAccidentals1.xml", "musuite" },
                    { "testAccidentals2.xml", "musuite" },
                    { "testArpGliss1.xml", "musuite" },
                    { "testArpGliss2.xml", "musuite" },
                    { "testBarStyles.xml", "musuite" },
                    { "testChord.xml", "musuite" },
                    { "testChordDiagrams1.xml", "musuite" },
                    { "testChordNoVoice.xml", "musuite" },
                    { "testChordNoVoice_ref.xml", "musuite" },
                    { "testClefs1.xml", "musuite" },
                    { "testCompleteMeasureRests.xml", "musuite" },
                    { "testDalSegno.xml", "musuite" },
                    { "testDCalCoda.xml", "musuite" },
                    { "testDCalFine.xml", "musuite" },
                    { "testDirections1.xml", "musuite" },
                    { "testDrumset1.xml", "musuite" },
                    { "testDrumset2.xml", "musuite" },
                    { "testDurationRoundingError.xml", "musuite" },
                    { "testDurationRoundingError_ref.xml", "musuite" },
                    { "testDynamics1.xml", "musuite" },
                    { "testDynamics2.xml", "musuite" },
                    { "testDynamics3.xml", "musuite" },
                    { "testDynamics3_ref.xml", "musuite" },
                    { "testEmptyMeasure.xml", "musuite" },
                    { "testEmptyMeasure_ref.xml", "musuite" },
                    { "testEmptyVoice1.xml", "musuite" },
                    { "testEmptyVoice1_ref.xml", "musuite" },
                    { "testFiguredBass1.xml", "musuite" },
                    { "testFiguredBass2.xml", "musuite" },
                    { "testGrace1.xml", "musuite" },
                    { "testGrace2.xml", "musuite" },
                    { "testHarmony1.xml", "musuite" },
                    { "testHarmony2.xml", "musuite" },
                    { "testHarmony3.xml", "musuite" },
                    { "testHello.xml", "musuite" },
                    { "testImplicitMeasure1.xml", "musuite" },
                    { "testInvalid.xml", "musuite" },
                    { "testInvisibleElements.xml", "musuite" },
                    { "testKeysig1.xml", "musuite" },
                    { "testLineBreaks.xml", "musuite" },
                    { "testLines1.xml", "musuite" },
                    { "testLines1_ref.xml", "musuite" },
                    { "testLines2.xml", "musuite" },
                    { "testLines2_ref.xml", "musuite" },
                    { "testLyricsVoice2a.xml", "musuite" },
                    { "testLyricsVoice2b.xml", "musuite" },
                    { "testLyricsVoice2b_ref.xml", "musuite" },
                    { "testManualBreaks.xml", "musuite" },
                    { "testMeasureLength.xml", "musuite" },
                    { "testMeasureLength_ref.xml", "musuite" },
                    { "testMetaData.xml", "musuite" },
                    { "testMultiMeasureRest1.xml", "musuite" },
                    { "testMultiMeasureRest1_ref.xml", "musuite" },
                    { "testMultiMeasureRest2.xml", "musuite" },
                    { "testMultiMeasureRest2_ref.xml", "musuite" },
                    { "testMultiMeasureRest3.xml", "musuite" },
                    { "testMultiMeasureRest3_ref.xml", "musuite" },
                    { "testMultipleNotations.xml", "musuite" },
                    { "testMultipleNotations_ref.xml", "musuite" },
                    { "testNonUniqueThings.xml", "musuite" },
                    { "testNonUniqueThings_ref.xml", "musuite" },
                    { "testNoteAttributes1.xml", "musuite" },
                    { "testNoteAttributes2.xml", "musuite" },
                    { "testNoteAttributes2_ref.xml", "musuite" },
                    { "testNoteAttributes3.xml", "musuite" },
                    { "testNoteheads.xml", "musuite" },
                    { "testNotesRests1.xml", "musuite" },
                    { "testNotesRests2.xml", "musuite" },
                    { "testNumberedLyrics.xml", "musuite" },
                    { "testNumberedLyrics_ref.xml", "musuite" },
                    { "testPartsSpecialCases.xml", "musuite" },
                    { "testPartsSpecialCases_ref.xml", "musuite" },
                    { "testPiano.xml", "musuite" },
                    { "testRestsNoType.xml", "musuite" },
                    { "testRestsNoType_ref.xml", "musuite" },
                    { "testSlurs.xml", "musuite" },
                    { "testStaffTwoKeySigs.xml", "musuite" },
                    { "testStringVoiceName.xml", "musuite" },
                    { "testStringVoiceName_ref.xml", "musuite" },
                    { "testSystemBrackets1.xml", "musuite" },
                    { "testSystemBrackets2.xml", "musuite" },
                    { "testTablature1.xml", "musuite" },
                    { "testTablature2.xml", "musuite" },
                    { "testTablature3.xml", "musuite" },
                    { "testTempo1.xml", "musuite" },
                    { "testTimesig1.xml", "musuite" },
                    { "testTimesig3.xml", "musuite" },
                    { "testTremolo.xml", "musuite" },
                    { "testTuplets1.xml", "musuite" },
                    { "testTuplets1_ref.xml", "musuite" },
                    { "testTuplets2.xml", "musuite" },
                    { "testTuplets2_ref.xml", "musuite" },
                    { "testTuplets3.xml", "musuite" },
                    { "testTuplets3_ref.xml", "musuite" },
                    { "testTuplets4.xml", "musuite" },
                    { "testUnusualDurations.xml", "musuite" },
                    { "testUnusualDurations_ref.xml", "musuite" },
                    { "testVoiceMapper1.xml", "musuite" },
                    { "testVoiceMapper1_ref.xml", "musuite" },
                    { "testVoiceMapper2.xml", "musuite" },
                    { "testVoiceMapper2_ref.xml", "musuite" },
                    { "testVoiceMapper3.xml", "musuite" },
                    { "testVoiceMapper3_ref.xml", "musuite" },
                    { "testVoicePiano1.xml", "musuite" },
                    { "testVolta1.xml", "musuite" },
                    { "testWedge1.xml", "musuite" },
                    { "testWedge2.xml", "musuite" },
                    { "testWedge2_ref.xml", "musuite" },
                    { "testWords1.xml", "musuite" },
                    { "ActorPreludeSample.xml", "recsuite" },
                    { "BeetAnGeSample.xml", "recsuite" },
                    { "Binchois.xml", "recsuite" },
                    { "BrahWiMeSample.xml", "recsuite" },
                    { "BrookeWestSample.xml", "recsuite" },
                    { "Chant.xml", "recsuite" },
                    { "DebuMandSample.xml", "recsuite" },
                    { "Dichterliebe01.xml", "recsuite" },
                    { "Echigo_Jishi.xml", "recsuite" },
                    { "FaurReveSample.xml", "recsuite" },
                    { "MahlFaGe4Sample.xml", "recsuite" },
                    { "MozaChloSample.xml", "recsuite" },
                    { "MozaChloSample2.xml", "recsuite" },
                    { "MozartPianoSonata.xml", "recsuite" },
                    { "MozartTrio.xml", "recsuite" },
                    { "MozaVeilSample.xml", "recsuite" },
                    { "Saltarello.xml", "recsuite" },
                    { "SchbAvMaSample.xml", "recsuite" },
                    { "Telemann.xml", "recsuite" },
                    
                    // ksuite
                    { "k001a_Articulations.xml", "ksuite" },
                    { "k001b_Articulations_Above.xml", "ksuite" },
                    { "k001c_Articulations_Below.xml", "ksuite" },
                    { "k002a_Fermatas.xml", "ksuite" },
                    { "k003a_Ornaments.xml", "ksuite" },
                    { "k004a_Technical.xml", "ksuite" },
                    { "k005a_Spanners_Slides.xml", "ksuite" },
                    { "k006a_Clefs_sign_G_ln_2_oct_0_Treble.xml", "ksuite" },
                    { "k006a_Header_Scaling_Decimals.xml", "ksuite" },
                    { "k006b_Clefs_sign_F_ln_4_oct_0_Bass.xml", "ksuite" },
                    { "k006c_Clefs_sign_C_ln_3_oct_0_Viola.xml", "ksuite" },
                    { "k006d_Clefs_sign_C_ln_4_oct_0_Tenor.xml", "ksuite" },
                    { "k006e_Clefs_sign_C_ln_5_oct_0_Baritone.xml", "ksuite" },
                    { "k006f_Clefs_sign_C_ln_1_oct_0_Soprano.xml", "ksuite" },
                    { "k006g_Clefs_sign_C_ln_2_oct_0_Mezzo_Soprano.xml", "ksuite" },
                    { "k006h_Clefs_sign_G_ln_1_oct_0_French.xml", "ksuite" },
                    { "k006i_Clefs_sign_F_ln_3_oct_0_Varbaritone.xml", "ksuite" },
                    { "k006j_Clefs_sign_F_ln_5_oct_0_Subbass.xml", "ksuite" },
                    { "k006k_Clefs_sign_G_ln_2_oct_neg2_G_15MB.xml", "ksuite" },
                    { "k006l_Clefs_sign_G_ln_2_oct_neg1_G_8VB.xml", "ksuite" },
                    { "k006m_Clefs_sign_G_ln_2_oct_1_G_8VA.xml", "ksuite" },
                    { "k006n_Clefs_sign_G_ln_2_oct_2_G_15MA.xml", "ksuite" },
                    { "k006o_Clefs_sign_F_ln_4_oct_neg2_F_15MB.xml", "ksuite" },
                    { "k006p_Clefs_sign_F_ln_4_oct_neg1_F_8VB.xml", "ksuite" },
                    { "k006q_Clefs_sign_F_ln_4_oct_1_F_8VA.xml", "ksuite" },
                    { "k006r_Clefs_sign_F_ln_4_oct_2_F_15MA.xml", "ksuite" },
                    { "k006s_Clefs_sign_percussion_ln_3_oct_0_Percussion.xml", "ksuite" },
                    { "k006t_Clefs_sign_TAB_ln_3_oct_0_Tab.xml", "ksuite" },
                    { "k006u_Clefs_sign_jianpu_ln_3_oct_0_Jianpu.xml", "ksuite" },
                    { "k006v_Clefs_sign_none_ln_3_oct_0_None.xml", "ksuite" },
                    { "k007a_Notations_Dynamics.xml", "ksuite" },
                    { "k008a_Beaming.xml", "ksuite" },
                    { "k009a_Slurs.xml", "ksuite" },
                    { "k010a_Ties.xml", "ksuite" },
                    { "k010b_Bad_Ties.xml", "ksuite" },
                    { "k011a_Tuplets.xml", "ksuite" },
                    { "k012a_Tempo_Markings.xml", "ksuite" },
                    { "k013a_OrchestralScoreFinale.xml", "ksuite" },
                    { "k013b_OrchestralScoreSibelius.xml", "ksuite" },
                    
                    // other
                    { "logic01a_homoSapiens.xml", "logicpro" },
                    { "Schubert_der_Mueller.xml", "foundsuite"  },
                }
    , myExpectedLoadFailures
                {
                    { "ly41g_PartNoId.xml" },
                }
    {

    }
}
