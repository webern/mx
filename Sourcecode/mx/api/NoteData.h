// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/PrintData.h"
#include "mx/api/PositionData.h"
#include "mx/api/DurationData.h"
#include "mx/api/PitchData.h"
#include "mx/api/NoteAttachmentData.h"

#include <vector>


namespace mx
{
    namespace api
    {
    	enum class NoteType
    	{
    		normal,
    		grace,
    		cue
    	};

        enum class Beam
        {
            unspecified,
            begin,
            end,
            extend,
            forwardBroken,
            backwardBroken
        };
        
        enum class Stem
    	{
    		unspecified,
    		up,
    		down,
    		none
    	};

    	class NoteData
    	{
    	public:

            NoteData();

            // when reading a MusicXML file this will be incremented
            // for each note that is encountered. when writing a
            // MusicXML file it will be ignored.
            int absoluteNoteIndex;
            
    		bool isRest;                        // if isRest is true then isUnpitched can be ignored
            bool isMeasureRest;                 // only valid if isRest is true
            bool isUnpitched;                   // only relevant if isRest is false
            bool isDisplayStepOctaveSpecified;  // MusicXML can optionally specify display step and octave for rests and unpitched notes
            
            // TODO - MusicXML is totally screwed up in that a note is part of
            // a chord if the *following* note says it is. Need to decide
            // whether to mirror that approach in the API or make it more sane.
            bool isChord;
            
            NoteType noteType;                  // normal, cue, grace
            PitchData pitchData;                // step, alter, octave, accidental, etc
            int staffIndex;                     // this should be one less than the number shown in the <staff> element
            int userRequestedVoiceNumber;
            Stem stem;
            
            int startTimeTicks;
            DurationData durationData;
            
            std::vector<Beam> beams;
            PositionData positionData;
            PrintData printData;
            
            NoteAttachmentData noteAttachmentData;
    	};
    }
}
