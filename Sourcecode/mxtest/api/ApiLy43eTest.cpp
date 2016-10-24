// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "mxtest/api/ApiTester.h"
#include "mxtest/file/MxFileTest.h"
#include "mxtest/file/MxFileTestGroup.h"
#include "mx/api/DocumentManager.h"
#include "cpul/cpulTestHarness.h"
#include "mx/api/ScoreData.h"
#include "mxtest/api/ApiTester.h"

using namespace mx::api;

namespace mxtest
{
	constexpr const char* const testFileName = "ly43e_Multistaff_ClefDynamics.xml";
    
    class ApiLy43eScoreData : public ScoreDataCreator
    {
        virtual mx::api::ScoreData createScoreData() const override
        {
            using namespace mx::api;
            ScoreData score;
            score.parts.emplace_back( PartData{} );
            score.ticksPerQuarter = 8;
            auto& part = score.parts.front();
            part.uniqueId = "P1";
            part.name = "MusicXML Part";
            
            // measure 1 - setup
            part.measures.emplace_back( MeasureData{} );
            auto measureP = &part.measures.front();
            measureP->keys.emplace_back( KeyData{} );
            auto keyP = &measureP->keys.back();
            keyP->mode = KeyMode::major;
            measureP->timeSignature.isImplicit = false;
            measureP->timeSignature.symbol = TimeSignatureSymbol::common;
            measureP->staves.emplace_back( StaffData{} );
            measureP->staves.emplace_back( StaffData{} );
            auto staff1P = &measureP->staves.at( 0 );
            staff1P->clefs.emplace_back( ClefData{} );
            auto staff2P = &measureP->staves.at( 1 );
            staff2P->clefs.emplace_back( ClefData{} );
            auto clefP = &staff2P->clefs.back();
            clefP->symbol = ClefSymbol::f;
            clefP->line = 4;
            staff1P->directions.emplace_back( DirectionData{} );
            auto directionP = &staff1P->directions.back();
            directionP->placement = Placement::below;
            directionP->marks.emplace_back( DirectionMark{} );
            auto directionMarkP = &directionP->marks.back();
            directionMarkP->markType = MarkType::ffff;
            directionMarkP->name = "ffff";
            auto glyph = &MarkSmufl::getSmuflGlyphname( directionMarkP->markType );
            directionMarkP->smuflName = glyph->below;
            directionMarkP->smuflCodepoint = Smufl::findCodepoint( directionMarkP->smuflName );
            
            // measure 1 - music
            int voice = 0;
            staff1P->voices[voice].notes.emplace_back( NoteData{} );
            auto noteP = &staff1P->voices[voice].notes.back();
            noteP->userRequestedVoiceNumber = voice + 1;
            noteP->tickTimePosition = 0;
            noteP->pitchData.step = Step::c;
            noteP->pitchData.octave = 5;
            noteP->pitchData.alter = 0;
            noteP->pitchData.accidental = Accidental::none;
            noteP->durationData.durationName = DurationName::quarter;
            noteP->durationData.durationTimeTicks = 8;
            
            staff1P->voices[voice].notes.emplace_back( NoteData{} );
            noteP = &staff1P->voices[voice].notes.back();
            noteP->userRequestedVoiceNumber = voice + 1;
            noteP->tickTimePosition = 8;
            noteP->pitchData.step = Step::b;
            noteP->pitchData.octave = 4;
            noteP->pitchData.alter = 0;
            noteP->pitchData.accidental = Accidental::none;
            noteP->durationData.durationName = DurationName::quarter;
            noteP->durationData.durationTimeTicks = 8;
            
            staff1P->voices[voice].notes.emplace_back( NoteData{} );
            noteP = &staff1P->voices[voice].notes.back();
            noteP->userRequestedVoiceNumber = voice + 1;
            noteP->tickTimePosition = 16;
            noteP->pitchData.step = Step::a;
            noteP->pitchData.octave = 4;
            noteP->pitchData.alter = 0;
            noteP->pitchData.accidental = Accidental::none;
            noteP->durationData.durationName = DurationName::quarter;
            noteP->durationData.durationTimeTicks = 8;
            
            staff1P->directions.emplace_back( DirectionData{} );
            directionP = &staff1P->directions.back();
            directionP->tickTimePosition = 24;
            directionP->isOffsetSpecified = true;
            directionP->offset = 1;
            directionP->placement = Placement::below;
            directionP->isStaffValueSpecified = true;
            directionP->marks.emplace_back( DirectionMark{} );
            directionMarkP = &directionP->marks.back();
            directionMarkP->markType = MarkType::p;
            directionMarkP->name = "p";
            glyph = &MarkSmufl::getSmuflGlyphname( directionMarkP->markType );
            directionMarkP->smuflName = glyph->below;
            directionMarkP->smuflCodepoint = Smufl::findCodepoint( directionMarkP->smuflName );
            
            staff1P->voices[voice].notes.emplace_back( NoteData{} );
            noteP = &staff1P->voices[voice].notes.back();
            noteP->userRequestedVoiceNumber = voice + 1;
            noteP->tickTimePosition = 24;
            noteP->pitchData.step = Step::g;
            noteP->pitchData.octave = 4;
            noteP->pitchData.alter = 0;
            noteP->pitchData.accidental = Accidental::none;
            noteP->durationData.durationName = DurationName::quarter;
            noteP->durationData.durationTimeTicks = 8;
            
            voice = 1;
            staff2P->voices[voice].notes.emplace_back( NoteData{} );
            noteP = &staff2P->voices[voice].notes.back();
            noteP->userRequestedVoiceNumber = voice + 1;
            noteP->tickTimePosition = 0;
            noteP->pitchData.step = Step::a;
            noteP->pitchData.octave = 2;
            noteP->pitchData.alter = 0;
            noteP->pitchData.accidental = Accidental::none;
            noteP->durationData.durationName = DurationName::quarter;
            noteP->durationData.durationTimeTicks = 8;
            
            staff2P->directions.emplace_back( DirectionData{} );
            directionP = &staff2P->directions.back();
            directionP->tickTimePosition = 0;
            directionP->isOffsetSpecified = false;
            directionP->offset = 0;
            directionP->placement = Placement::below;
            directionP->isStaffValueSpecified = true;
            directionP->wedgeStarts.emplace_back( WedgeStart{} );
            auto wedgeStartP = &directionP->wedgeStarts.back();
            wedgeStartP->wedgeType = WedgeType::crescendo;
            wedgeStartP->isSpreadSpecified = true;
            wedgeStartP->spread = 0.0;
            
            staff2P->voices[voice].notes.emplace_back( NoteData{} );
            noteP = &staff2P->voices[voice].notes.back();
            noteP->userRequestedVoiceNumber = voice + 1;
            noteP->tickTimePosition = 8;
            noteP->pitchData.step = Step::b;
            noteP->pitchData.octave = 2;
            noteP->pitchData.alter = 0;
            noteP->pitchData.accidental = Accidental::none;
            noteP->durationData.durationName = DurationName::quarter;
            noteP->durationData.durationTimeTicks = 8;
            
            staff2P->directions.emplace_back( DirectionData{} );
            directionP = &staff2P->directions.back();
            directionP->tickTimePosition = 16;
            directionP->isOffsetSpecified = false;
            directionP->offset = 0;
            directionP->placement = Placement::unspecified;
            directionP->isStaffValueSpecified = true;
            directionP->wedgeStops.emplace_back( WedgeStop{} );
            auto wedgeStopP = &directionP->wedgeStops.back();
            wedgeStopP->isSpreadSpecified = true;
            wedgeStopP->spread = 15.0;
            
            staff2P->voices[voice].notes.emplace_back( NoteData{} );
            noteP = &staff2P->voices[voice].notes.back();
            noteP->userRequestedVoiceNumber = voice + 1;
            noteP->tickTimePosition = 16;
            noteP->pitchData.step = Step::c;
            noteP->pitchData.octave = 3;
            noteP->pitchData.alter = 0;
            noteP->pitchData.accidental = Accidental::none;
            noteP->durationData.durationName = DurationName::quarter;
            noteP->durationData.durationTimeTicks = 8;
            
            staff2P->voices[voice].notes.emplace_back( NoteData{} );
            noteP = &staff2P->voices[voice].notes.back();
            noteP->userRequestedVoiceNumber = voice + 1;
            noteP->tickTimePosition = 24;
            noteP->pitchData.step = Step::d;
            noteP->pitchData.octave = 3;
            noteP->pitchData.alter = 0;
            noteP->pitchData.accidental = Accidental::none;
            noteP->durationData.durationName = DurationName::quarter;
            noteP->durationData.durationTimeTicks = 8;
            
            // measure 2 - setup
            part.measures.emplace_back( MeasureData{} );
            measureP = &part.measures.back();
            measureP->keys.emplace_back( KeyData{} );
            keyP = &measureP->keys.back();
            keyP->fifths = 2;
            keyP->mode = KeyMode::major;
            measureP->staves.emplace_back( StaffData{} );
            measureP->staves.emplace_back( StaffData{} );
            staff1P = &measureP->staves.at( 0 );
            staff2P = &measureP->staves.at( 1 );
            staff2P->clefs.emplace_back( ClefData{} );
            clefP = &staff2P->clefs.back();
            clefP->symbol = ClefSymbol::g;
            clefP->line = 2;
            
            // measure 2 - music
            voice = 0;
            staff1P = &measureP->staves.at( 0 );
            staff1P->voices[voice].notes.emplace_back( NoteData{} );
            noteP = &staff1P->voices[voice].notes.back();
            noteP->userRequestedVoiceNumber = voice + 1;
            noteP->tickTimePosition = 0;
            noteP->pitchData.step = Step::a;
            noteP->pitchData.octave = 4;
            noteP->pitchData.alter = 0;
            noteP->pitchData.accidental = Accidental::none;
            noteP->durationData.durationName = DurationName::quarter;
            noteP->durationData.durationTimeTicks = 8;
            
            staff1P->voices[voice].notes.emplace_back( NoteData{} );
            noteP = &staff1P->voices[voice].notes.back();
            noteP->userRequestedVoiceNumber = voice + 1;
            noteP->tickTimePosition = 8;
            noteP->pitchData.step = Step::b;
            noteP->pitchData.octave = 4;
            noteP->pitchData.alter = 0;
            noteP->pitchData.accidental = Accidental::none;
            noteP->durationData.durationName = DurationName::quarter;
            noteP->durationData.durationTimeTicks = 8;
            
            staff1P->voices[voice].notes.emplace_back( NoteData{} );
            noteP = &staff1P->voices[voice].notes.back();
            noteP->userRequestedVoiceNumber = voice + 1;
            noteP->tickTimePosition = 16;
            noteP->pitchData.step = Step::c;
            noteP->pitchData.octave = 5;
            noteP->pitchData.alter = 1;
            noteP->pitchData.accidental = Accidental::none;
            noteP->durationData.durationName = DurationName::quarter;
            noteP->durationData.durationTimeTicks = 8;
            
            staff1P->voices[voice].notes.emplace_back( NoteData{} );
            noteP = &staff1P->voices[voice].notes.back();
            noteP->userRequestedVoiceNumber = voice + 1;
            noteP->tickTimePosition = 24;
            noteP->pitchData.step = Step::d;
            noteP->pitchData.octave = 5;
            noteP->pitchData.alter = 0;
            noteP->pitchData.accidental = Accidental::none;
            noteP->durationData.durationName = DurationName::quarter;
            noteP->durationData.durationTimeTicks = 8;
            
            voice = 1;
            staff2P = &measureP->staves.at( 1 );
            staff2P->voices[voice].notes.emplace_back( NoteData{} );
            noteP = &staff2P->voices[voice].notes.back();
            noteP->userRequestedVoiceNumber = voice + 1;
            noteP->tickTimePosition = 0;
            noteP->pitchData.step = Step::f;
            noteP->pitchData.octave = 4;
            noteP->pitchData.alter = 1;
            noteP->pitchData.accidental = Accidental::none;
            noteP->durationData.durationName = DurationName::quarter;
            noteP->durationData.durationTimeTicks = 8;
            
            staff2P->voices[voice].notes.emplace_back( NoteData{} );
            noteP = &staff2P->voices[voice].notes.back();
            noteP->userRequestedVoiceNumber = voice + 1;
            noteP->tickTimePosition = 8;
            noteP->pitchData.step = Step::g;
            noteP->pitchData.octave = 4;
            noteP->pitchData.alter = 0;
            noteP->pitchData.accidental = Accidental::none;
            noteP->durationData.durationName = DurationName::quarter;
            noteP->durationData.durationTimeTicks = 8;
            
            staff2P->voices[voice].notes.emplace_back( NoteData{} );
            noteP = &staff2P->voices[voice].notes.back();
            noteP->userRequestedVoiceNumber = voice + 1;
            noteP->tickTimePosition = 16;
            noteP->pitchData.step = Step::a;
            noteP->pitchData.octave = 4;
            noteP->pitchData.alter = 0;
            noteP->pitchData.accidental = Accidental::none;
            noteP->durationData.durationName = DurationName::quarter;
            noteP->durationData.durationTimeTicks = 8;
            
            staff2P->voices[voice].notes.emplace_back( NoteData{} );
            noteP = &staff2P->voices[voice].notes.back();
            noteP->userRequestedVoiceNumber = voice + 1;
            noteP->tickTimePosition = 24;
            noteP->pitchData.step = Step::b;
            noteP->pitchData.octave = 4;
            noteP->pitchData.alter = 0;
            noteP->pitchData.accidental = Accidental::none;
            noteP->durationData.durationName = DurationName::quarter;
            noteP->durationData.durationTimeTicks = 8;
            
            // measure 3 - setup
            part.measures.emplace_back( MeasureData{} );
            measureP = &part.measures.back();
            measureP->staves.emplace_back( StaffData{} );
            measureP->staves.emplace_back( StaffData{} );
            staff1P = &measureP->staves.at( 0 );
            staff2P = &measureP->staves.at( 1 );
            staff1P->clefs.emplace_back( ClefData{} );
            clefP = &staff1P->clefs.back();
            clefP->symbol = ClefSymbol::c;
            clefP->line = 2;
            
            // measure 3 - music
            voice = 0;
            staff1P = &measureP->staves.at( 0 );
            staff1P->voices[voice].notes.emplace_back( NoteData{} );
            noteP = &staff1P->voices[voice].notes.back();
            noteP->userRequestedVoiceNumber = voice + 1;
            noteP->tickTimePosition = 0;
            noteP->pitchData.step = Step::d;
            noteP->pitchData.octave = 5;
            noteP->pitchData.alter = 0;
            noteP->pitchData.accidental = Accidental::none;
            noteP->durationData.durationName = DurationName::quarter;
            noteP->durationData.durationTimeTicks = 8;
            
            staff1P->voices[voice].notes.emplace_back( NoteData{} );
            noteP = &staff1P->voices[voice].notes.back();
            noteP->userRequestedVoiceNumber = voice + 1;
            noteP->tickTimePosition = 8;
            noteP->pitchData.step = Step::c;
            noteP->pitchData.octave = 5;
            noteP->pitchData.alter = 1;
            noteP->pitchData.accidental = Accidental::none;
            noteP->durationData.durationName = DurationName::quarter;
            noteP->durationData.durationTimeTicks = 8;
            
            staff1P->voices[voice].notes.emplace_back( NoteData{} );
            noteP = &staff1P->voices[voice].notes.back();
            noteP->userRequestedVoiceNumber = voice + 1;
            noteP->tickTimePosition = 16;
            noteP->pitchData.step = Step::b;
            noteP->pitchData.octave = 4;
            noteP->pitchData.alter = 0;
            noteP->pitchData.accidental = Accidental::none;
            noteP->durationData.durationName = DurationName::quarter;
            noteP->durationData.durationTimeTicks = 8;
            
            staff1P->voices[voice].notes.emplace_back( NoteData{} );
            noteP = &staff1P->voices[voice].notes.back();
            noteP->userRequestedVoiceNumber = voice + 1;
            noteP->tickTimePosition = 24;
            noteP->pitchData.step = Step::a;
            noteP->pitchData.octave = 4;
            noteP->pitchData.alter = 0;
            noteP->pitchData.accidental = Accidental::none;
            noteP->durationData.durationName = DurationName::quarter;
            noteP->durationData.durationTimeTicks = 8;
            
            voice = 1;
            staff2P->voices[voice].notes.emplace_back( NoteData{} );
            noteP = &staff2P->voices[voice].notes.back();
            noteP->userRequestedVoiceNumber = voice + 1;
            noteP->tickTimePosition = 0;
            noteP->pitchData.step = Step::a;
            noteP->pitchData.octave = 4;
            noteP->pitchData.alter = 0;
            noteP->pitchData.accidental = Accidental::none;
            noteP->durationData.durationName = DurationName::quarter;
            noteP->durationData.durationTimeTicks = 8;
            
            staff2P->voices[voice].notes.emplace_back( NoteData{} );
            noteP = &staff2P->voices[voice].notes.back();
            noteP->userRequestedVoiceNumber = voice + 1;
            noteP->tickTimePosition = 8;
            noteP->pitchData.step = Step::b;
            noteP->pitchData.octave = 4;
            noteP->pitchData.alter = 0;
            noteP->pitchData.accidental = Accidental::none;
            noteP->durationData.durationName = DurationName::quarter;
            noteP->durationData.durationTimeTicks = 8;
            
            staff2P->voices[voice].notes.emplace_back( NoteData{} );
            noteP = &staff2P->voices[voice].notes.back();
            noteP->userRequestedVoiceNumber = voice + 1;
            noteP->tickTimePosition = 16;
            noteP->pitchData.step = Step::c;
            noteP->pitchData.octave = 5;
            noteP->pitchData.alter = 1;
            noteP->pitchData.accidental = Accidental::none;
            noteP->durationData.durationName = DurationName::quarter;
            noteP->durationData.durationTimeTicks = 8;
            
            staff2P->voices[voice].notes.emplace_back( NoteData{} );
            noteP = &staff2P->voices[voice].notes.back();
            noteP->userRequestedVoiceNumber = voice + 1;
            noteP->tickTimePosition = 24;
            noteP->pitchData.step = Step::d;
            noteP->pitchData.octave = 5;
            noteP->pitchData.alter = 0;
            noteP->pitchData.accidental = Accidental::none;
            noteP->durationData.durationName = DurationName::quarter;
            noteP->durationData.durationTimeTicks = 8;
            
            // measure 4 - setup
            part.measures.emplace_back( MeasureData{} );
            measureP = &part.measures.back();
            measureP->staves.emplace_back( StaffData{} );
            measureP->staves.emplace_back( StaffData{} );
            staff1P = &measureP->staves.at( 0 );
            staff2P = &measureP->staves.at( 1 );
            
            // measure 4 - music
            voice = 0;
            staff1P = &measureP->staves.at( 0 );
            staff1P->voices[voice].notes.emplace_back( NoteData{} );
            noteP = &staff1P->voices[voice].notes.back();
            noteP->userRequestedVoiceNumber = voice + 1;
            noteP->tickTimePosition = 0;
            noteP->pitchData.step = Step::c;
            noteP->pitchData.octave = 4;
            noteP->pitchData.alter = 0;
            noteP->pitchData.accidental = Accidental::none;
            noteP->durationData.durationName = DurationName::whole;
            noteP->isRest = true;
            noteP->durationData.durationTimeTicks = 32;
            
            voice = 1;
            staff2P->voices[voice].notes.emplace_back( NoteData{} );
            noteP = &staff2P->voices[voice].notes.back();
            noteP->userRequestedVoiceNumber = voice + 1;
            noteP->tickTimePosition = 0;
            noteP->pitchData.step = Step::c;
            noteP->pitchData.octave = 4;
            noteP->pitchData.alter = 0;
            noteP->pitchData.accidental = Accidental::none;
            noteP->durationData.durationName = DurationName::whole;
            noteP->isRest = true;
            noteP->durationData.durationTimeTicks = 32;
            
            measureP->barlines.emplace_back( BarlineData{} );
            auto barlineP = &measureP->barlines.back();
            barlineP->tickTimePosition = std::numeric_limits<int>::max();
            barlineP->barlineType = BarlineType::lightHeavy;
            
            return score;
        }
    };
    
    class ApiLy43eScoreData;
    
    static const ApiTester ApiLy43eTest
    {
        ScoreDataCreatorPtr{ new ApiLy43eScoreData{} },
        MxFileRepository::getTestFile( testFileName ),
        __FILE__,
        __LINE__
    };
}

/*
 
 ScoreDataCreatorPtr&& inScoreDataCreator,
 MxFile inFile,
 std::string cppFileName,
 int cppFileLineNumber);
 */

#endif


/*
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE score-partwise PUBLIC "-//Recordare//DTD MusicXML 1.0 Partwise//EN"
                                "http://www.musicxml.org/dtds/partwise.dtd">
<score-partwise>
  <identification>
    <miscellaneous>
      <miscellaneous-field name="description">A piano staff with dynamics and 
          clef changes, where each element (ffff, wedge and clef changes) 
          applies only to one voice or one staff, respectively.</miscellaneous-field>
    </miscellaneous>
  </identification>
  <part-list>
    <score-part id="P1">
      <part-name>MusicXML Part</part-name>
    </score-part>
  </part-list>
  <!--=========================================================-->
  <part id="P1">
    <measure number="1">
      <attributes>
        <divisions>8</divisions>
        <key>
          <fifths>0</fifths>
          <mode>major</mode>
        </key>
        <time symbol="common">
          <beats>4</beats>
          <beat-type>4</beat-type>
        </time>
        <staves>2</staves>
        <clef number="1">
          <sign>G</sign>
          <line>2</line>
        </clef>
        <clef number="2">
          <sign>F</sign>
          <line>4</line>
        </clef>
      </attributes>
      <direction placement="below">
        <direction-type>
          <dynamics>
            <ffff/>
          </dynamics>
        </direction-type>
        <staff>1</staff>
      </direction>
      <note>
        <pitch>
          <step>C</step>
          <octave>5</octave>
        </pitch>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        <staff>1</staff>
      </note>
      <note>
        <pitch>
          <step>B</step>
          <octave>4</octave>
        </pitch>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        <staff>1</staff>
      </note>
      <note>
        <pitch>
          <step>A</step>
          <octave>4</octave>
        </pitch>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        <staff>1</staff>
      </note>
      <direction placement="below">
        <direction-type>
          <dynamics>
            <p/>
          </dynamics>
        </direction-type>
        <offset>1</offset>
        <staff>1</staff>
      </direction>
      <note>
        <pitch>
          <step>G</step>
          <octave>4</octave>
        </pitch>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        <staff>1</staff>
      </note>
      <backup>
        <duration>32</duration>
      </backup>
      <direction placement="below">
        <direction-type>
          <wedge spread="0" type="crescendo"/>
        </direction-type>
        <staff>2</staff>
      </direction>
      <note>
        <pitch>
          <step>A</step>
          <octave>2</octave>
        </pitch>
        <duration>8</duration>
        <voice>2</voice>
        <type>quarter</type>
        <staff>2</staff>
      </note>
      <note>
        <pitch>
          <step>B</step>
          <octave>2</octave>
        </pitch>
        <duration>8</duration>
        <voice>2</voice>
        <type>quarter</type>
        <staff>2</staff>
      </note>
      <direction>
        <direction-type>
          <wedge spread="15" type="stop"/>
        </direction-type>
        <staff>2</staff>
      </direction>
      <note>
        <pitch>
          <step>C</step>
          <octave>3</octave>
        </pitch>
        <duration>8</duration>
        <voice>2</voice>
        <type>quarter</type>
        <staff>2</staff>
      </note>
      <note>
        <pitch>
          <step>D</step>
          <octave>3</octave>
        </pitch>
        <duration>8</duration>
        <voice>2</voice>
        <type>quarter</type>
        <staff>2</staff>
      </note>
    </measure>
    <!--=======================================================-->
    <measure number="2">
      <attributes>
        <key>
          <fifths>2</fifths>
          <mode>major</mode>
        </key>
        <clef number="2">
          <sign>G</sign>
          <line>2</line>
        </clef>
      </attributes>
      <note>
        <pitch>
          <step>A</step>
          <octave>4</octave>
        </pitch>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        <staff>1</staff>
      </note>
      <note>
        <pitch>
          <step>B</step>
          <octave>4</octave>
        </pitch>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        <staff>1</staff>
      </note>
      <note>
        <pitch>
          <step>C</step>
          <alter>1</alter>
          <octave>5</octave>
        </pitch>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        <staff>1</staff>
      </note>
      <note>
        <pitch>
          <step>D</step>
          <octave>5</octave>
        </pitch>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        <staff>1</staff>
      </note>
      <backup>
        <duration>32</duration>
      </backup>
      <note>
        <pitch>
          <step>F</step>
          <alter>1</alter>
          <octave>4</octave>
        </pitch>
        <duration>8</duration>
        <voice>2</voice>
        <type>quarter</type>
        <staff>2</staff>
      </note>
      <note>
        <pitch>
          <step>G</step>
          <octave>4</octave>
        </pitch>
        <duration>8</duration>
        <voice>2</voice>
        <type>quarter</type>
        <staff>2</staff>
      </note>
      <note>
        <pitch>
          <step>A</step>
          <octave>4</octave>
        </pitch>
        <duration>8</duration>
        <voice>2</voice>
        <type>quarter</type>
        <staff>2</staff>
      </note>
      <note>
        <pitch>
          <step>B</step>
          <octave>4</octave>
        </pitch>
        <duration>8</duration>
        <voice>2</voice>
        <type>quarter</type>
        <staff>2</staff>
      </note>
    </measure>
    <!--=======================================================-->
    <measure number="3">
      <attributes>
        <clef number="1">
          <sign>C</sign>
          <line>2</line>
        </clef>
      </attributes>
      <note>
        <pitch>
          <step>D</step>
          <octave>5</octave>
        </pitch>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        <staff>1</staff>
      </note>
      <note>
        <pitch>
          <step>C</step>
          <alter>1</alter>
          <octave>5</octave>
        </pitch>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        <staff>1</staff>
      </note>
      <note>
        <pitch>
          <step>B</step>
          <octave>4</octave>
        </pitch>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        <staff>1</staff>
      </note>
      <note>
        <pitch>
          <step>A</step>
          <octave>4</octave>
        </pitch>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        <staff>1</staff>
      </note>
      <backup>
        <duration>32</duration>
      </backup>
      <note>
        <pitch>
          <step>A</step>
          <octave>4</octave>
        </pitch>
        <duration>8</duration>
        <voice>2</voice>
        <type>quarter</type>
        <staff>2</staff>
      </note>
      <note>
        <pitch>
          <step>B</step>
          <octave>4</octave>
        </pitch>
        <duration>8</duration>
        <voice>2</voice>
        <type>quarter</type>
        <staff>2</staff>
      </note>
      <note>
        <pitch>
          <step>C</step>
          <alter>1</alter>
          <octave>5</octave>
        </pitch>
        <duration>8</duration>
        <voice>2</voice>
        <type>quarter</type>
        <staff>2</staff>
      </note>
      <note>
        <pitch>
          <step>D</step>
          <octave>5</octave>
        </pitch>
        <duration>8</duration>
        <voice>2</voice>
        <type>quarter</type>
        <staff>2</staff>
      </note>
    </measure>
    <!--=======================================================-->
    <measure number="4">
      <note>
        <rest/>
        <duration>32</duration>
        <voice>1</voice>
        <staff>1</staff>
      </note>
      <backup>
        <duration>32</duration>
      </backup>
      <note>
        <rest/>
        <duration>32</duration>
        <voice>2</voice>
        <staff>2</staff>
      </note>
      <barline location="right">
        <bar-style>light-heavy</bar-style>
      </barline>
    </measure>
  </part>
  <!--=========================================================-->
</score-partwise>

*/
