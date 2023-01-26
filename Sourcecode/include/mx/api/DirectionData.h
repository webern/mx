// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/ChordData.h"
#include "mx/api/CodaData.h"
#include "mx/api/MarkData.h"
#include "mx/api/OttavaData.h"
#include "mx/api/RehearsalData.h"
#include "mx/api/SegnoData.h"
#include "mx/api/TempoData.h"
#include "mx/api/WedgeData.h"
#include "mx/api/WordsData.h"

namespace mx
{
    namespace api
    {

        // MusicXML Documentation: A direction is a musical indication that is not attached to a specific
        // note. Two or more may be combined to indicate starts and stops of wedges, dashes, etc.
        //
        // By default, a series of direction-type elements and a series of child elements of a direction-
        // type within a single direction element follow one another in sequence visually. For a series of
        // direction-type children, non-positional formatting attributes are carried over from the
        // previous element by default.
    	struct DirectionData
    	{
            int tickTimePosition;
            Placement placement;

            // mx::api will place the DirectionData element in the correct place by using an offset element.
            // MusicXML Documentation: An offset is represented in terms of divisions, and indicates where
            // the direction will appear relative to the current musical location. This affects the visual
            // appearance of the direction. If the sound attribute is "yes", then the offset affects
            // playback too. If the sound attribute is "no", then any sound associated with the direction
            // takes effect at the current location. The sound attribute is "no" by default for
            // compatibility with earlier versions of the MusicXML format. If an element within a
            // direction includes a default-x attribute, the offset value will be ignored when determining
            // the appearance of that element.

            // voice value of -1 means unspecified
            int voice;
            
            // Direction elements are placed inside the StaffData object in mx api.  This would mean that
            // they are alyways assigned to a staff, however MusicXML makes the staff element optional.
            // in the case that we want a direction element to have no <staff> element then me must place
            // the direction on staffIndex = 0 and set isStaffValueSpecified. otherwise the we will
            // assume that the <staff> element is present with a value of staffIndex + 1. This bool is
            // only relevant for Directions placed on staff zero, it is otherwise ignored.
            bool isStaffValueSpecified;
            
            // TODO - sound element
            
            std::vector<TempoData> tempos;
            std::vector<MarkData> marks;
            std::vector<WedgeStart> wedgeStarts;
            std::vector<WedgeStop> wedgeStops;
            std::vector<OttavaStart> ottavaStarts;
            std::vector<SpannerStop> ottavaStops;
            std::vector<SpannerStart> bracketStarts;
            std::vector<SpannerStop> bracketStops;
            std::vector<WordsData> words;
            std::vector<ChordData> chords;
            std::vector<SegnoData> segnos;
            std::vector<CodaData> codas;
            std::vector<RehearsalData> rehearsals;
            
            DirectionData()
            : tickTimePosition{ 0 }
            , placement{ Placement::unspecified }
            , voice{ -1 }
            , isStaffValueSpecified{ true }
            , marks{}
            , wedgeStarts{}
            , wedgeStops{}
            , ottavaStarts{}
            , ottavaStops{}
            , bracketStarts{}
            , bracketStops{}
            , words{}
            , chords{}
            , segnos{}
            {
                
            }
    	};
        
        inline bool isDirectionDataEmpty( const DirectionData& directionData )
        {
            return directionData.tempos.size() == 0 &&
            directionData.marks.size() == 0 &&
            directionData.wedgeStarts.size() == 0 &&
            directionData.wedgeStops.size() == 0 &&
            directionData.bracketStarts.size() == 0 &&
            directionData.bracketStops.size() == 0 &&
            directionData.tempos.size() == 0 &&
            directionData.ottavaStarts.size() == 0 &&
            directionData.ottavaStops.size() == 0 &&
            directionData.words.size() == 0 &&
            directionData.segnos.size() == 0 &&
            directionData.codas.size() == 0;
        }
        
        MXAPI_EQUALS_BEGIN( DirectionData )
        MXAPI_EQUALS_MEMBER( tickTimePosition )
        MXAPI_EQUALS_MEMBER( placement )
        MXAPI_EQUALS_MEMBER( voice )
        MXAPI_EQUALS_MEMBER( isStaffValueSpecified )
        MXAPI_EQUALS_MEMBER( tempos )
        MXAPI_EQUALS_MEMBER( marks )
        MXAPI_EQUALS_MEMBER( wedgeStarts )
        MXAPI_EQUALS_MEMBER( wedgeStops )
        MXAPI_EQUALS_MEMBER( ottavaStarts )
        MXAPI_EQUALS_MEMBER( ottavaStops )
        MXAPI_EQUALS_MEMBER( bracketStarts )
        MXAPI_EQUALS_MEMBER( bracketStops )
        MXAPI_EQUALS_MEMBER( words )
        MXAPI_EQUALS_MEMBER( chords )
        MXAPI_EQUALS_MEMBER( segnos )
        MXAPI_EQUALS_MEMBER( codas )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( DirectionData );
	}
}
