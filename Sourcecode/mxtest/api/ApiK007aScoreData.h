// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/ScoreData.h"
#include "mx/api/Smufl.h"

namespace
{
    
    
    inline void addNoteToMeasure( mx::api::MarkType markType, mx::api::MeasureData* measureP )
    {
        using namespace mx::api;
        auto staffP = &measureP->staves.at( 0 );
        int voice = 0;
        staffP->voices[voice].notes.emplace_back( NoteData{} );
        auto noteP = &staffP->voices[voice].notes.back();
        noteP->userRequestedVoiceNumber = voice + 1;
        noteP->tickTimePosition = 0;
        noteP->pitchData.step = Step::c;
        noteP->pitchData.octave = 4;
        noteP->pitchData.alter = 0;
        noteP->pitchData.accidental = Accidental::none;
        noteP->durationData.durationName = DurationName::whole;
        noteP->durationData.durationTimeTicks = 8;
        noteP->noteAttachmentData.marks.emplace_back( MarkData{} );
        auto& markData = noteP->noteAttachmentData.marks.back();
        markData.markType = markType;
        markData.smuflName = MarkSmufl::getName( markType );
        markData.name = markData.smuflName;
        markData.smuflCodepoint = MarkSmufl::getCodepoint( markType );
        markData.tickTimePosition = noteP->tickTimePosition;
        markData.positionData.placement = Placement::below;
    }
    
    inline void addMeasureWithNote( mx::api::MarkType markType, mx::api::PartData& outPartData )
    {
        using namespace mx::api;
        outPartData.measures.emplace_back( MeasureData{} );
        auto measureP = &outPartData.measures.back();
        measureP->staves.emplace_back( StaffData{} );
        addNoteToMeasure( markType, measureP );
    }
    
    inline void addFirstMeasureWithNote( mx::api::MarkType markType, mx::api::PartData& outPartData )
    {
        using namespace mx::api;
        outPartData.measures.emplace_back( MeasureData{} );
        auto measureP = &outPartData.measures.front();
        //measureP->keys.emplace_back( KeyData{} );
        //auto keyP = &measureP->keys.back();
        //keyP->mode = KeyMode::major;
        measureP->timeSignature.isImplicit = false;
        measureP->timeSignature.symbol = TimeSignatureSymbol::unspecified;
        measureP->staves.emplace_back( StaffData{} );
        auto staffP = &measureP->staves.at( 0 );
        staffP->clefs.emplace_back( ClefData{} );
        auto clefP = &staffP->clefs.back();
        clefP->symbol = ClefSymbol::g;
        clefP->line = 2;
        addNoteToMeasure( markType, measureP );
    }
}

namespace mxtest
{
    inline mx::api::ScoreData apiK007aScoreData()
    {
        using namespace mx::api;
        ScoreData score;
        score.parts.emplace_back( PartData{} );
        score.ticksPerQuarter = 2;
        auto& part = score.parts.front();
        part.uniqueId = "P1";
        part.name = "Dynamics";
        
        addFirstMeasureWithNote( MarkType::p, part );
        addMeasureWithNote( MarkType::pp, part );
        addMeasureWithNote( MarkType::ppp, part );
        addMeasureWithNote( MarkType::pppp, part );
        addMeasureWithNote( MarkType::ppppp, part );
        addMeasureWithNote( MarkType::pppppp, part );
        addMeasureWithNote( MarkType::f, part );
        addMeasureWithNote( MarkType::ff, part );
        addMeasureWithNote( MarkType::fff, part );
        addMeasureWithNote( MarkType::ffff, part );
        addMeasureWithNote( MarkType::fffff, part );
        addMeasureWithNote( MarkType::ffffff, part );
        addMeasureWithNote( MarkType::mp, part );
        addMeasureWithNote( MarkType::mf, part );
        addMeasureWithNote( MarkType::sf, part );
        addMeasureWithNote( MarkType::sfp, part );
        addMeasureWithNote( MarkType::sfpp, part );
        addMeasureWithNote( MarkType::fp, part );
        addMeasureWithNote( MarkType::rf, part );
        addMeasureWithNote( MarkType::rfz, part );
        addMeasureWithNote( MarkType::sfz, part );
        addMeasureWithNote( MarkType::sffz, part );
        addMeasureWithNote( MarkType::fz, part );
        addMeasureWithNote( MarkType::otherDynamics, part );
        
        auto& lastMeasure = part.measures.back();
        auto& lastDynamic = lastMeasure.staves.back().voices[0].notes.back().noteAttachmentData.marks.back();
        const std::string name = "dynamicZ";
        lastDynamic.smuflName = name;
        lastDynamic.name = name;
        lastDynamic.smuflCodepoint = Smufl::findCodepoint( name );
        
        return score;
    }
}
