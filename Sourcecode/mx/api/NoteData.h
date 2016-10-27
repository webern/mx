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
    		none,
            both
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
            
            // This is separate from the tie curves themselves. This
            // states that the note should be tied but noteAttachment.curve
            // items are needed to draw the ties visibly
            bool isTieStart;
            bool isTieStop;
            
            NoteType noteType;            // normal, cue, grace
            PitchData pitchData;          // step, alter, octave, accidental, etc
            //int staffIndex;             // this should be one less than the number shown in the <staff> element
            int userRequestedVoiceNumber;
            Stem stem;
            
            // the location of the note, timewise, within the measure
            // denominated in ticksPerQuarter as defined by ScoreData
            int tickTimePosition;
            DurationData durationData;
            
            std::vector<Beam> beams;
            PositionData positionData;
            PrintData printData;
            
            NoteAttachmentData noteAttachmentData;
    	};
        
        
        MXAPI_EQUALS_BEGIN( NoteData )
        MXAPI_EQUALS_FIRST_MEMBER( absoluteNoteIndex )
        MXAPI_EQUALS_NEXT_MEMBER( isRest )
        MXAPI_EQUALS_NEXT_MEMBER( isMeasureRest )
        MXAPI_EQUALS_NEXT_MEMBER( isUnpitched )
        MXAPI_EQUALS_NEXT_MEMBER( isDisplayStepOctaveSpecified )
        MXAPI_EQUALS_NEXT_MEMBER( isChord )
        MXAPI_EQUALS_NEXT_MEMBER( isTieStart )
        MXAPI_EQUALS_NEXT_MEMBER( isTieStop )
        MXAPI_EQUALS_NEXT_MEMBER( noteType )
        MXAPI_EQUALS_NEXT_MEMBER( pitchData )
        MXAPI_EQUALS_NEXT_MEMBER( userRequestedVoiceNumber )
        MXAPI_EQUALS_NEXT_MEMBER( stem )
        MXAPI_EQUALS_NEXT_MEMBER( tickTimePosition )
        MXAPI_EQUALS_NEXT_MEMBER( durationData )
        MXAPI_EQUALS_NEXT_MEMBER( beams )
        MXAPI_EQUALS_NEXT_MEMBER( positionData )
        MXAPI_EQUALS_NEXT_MEMBER( printData )
        MXAPI_EQUALS_LAST_MEMBER( noteAttachmentData )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( NoteData );
    }
}
