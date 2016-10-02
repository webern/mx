// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/MeasureData.h"

#include <string>
#include <vector>

namespace mx
{
    namespace api
    {
        enum class SoloOrEnsemble
        {
            unspecified,
            solo,
            ensemble
        };
        
        struct InstrumentData
        {
            std::string uniqueId;
            std::string name;
            std::string abbreviation;
            std::string sound;
            SoloOrEnsemble soloOrEnsemble;
        };
        
        struct MidiData
        {
            std::string virtualLibrary;
            std::string virtualName;
            std::string device;
            std::string uniqueId;
            std::string name;
            
            // -1 indicates absence of value
            int bank;

            // -1 indicates absence of value
            int channel;

            // -1 indicates absence of value
            int program;
            
            // -1 indicates absence of value
            int unpitched;
            
            // percent, valid range 0.0 to 100.0
            long double volume;
            bool isVolumeSpecified;

            // rotation degrees, valid range -180.0 to +180.0
            long double pan;
            bool isPanSpecified;

            // rotation degrees, valid range -180.0 to +180.0
            long double elevation;
            bool isElevationSpecified;
            
            MidiData()
            : virtualLibrary{}
            , virtualName{}
            , device{}
            , uniqueId{}
            , name{}
            , bank{ -1 }
            , channel{ -1 }
            , program{ -1 }
            , unpitched{ -1 }
            , volume{ 0.0 }
            , isVolumeSpecified{ false }
            , pan{ 0.0 }
            , isPanSpecified{ false }
            , elevation{ 0.0 }
            , isElevationSpecified{ false }
            {
                
            }
        };
        
        class PartData
        {
        public:
            std::string uniqueId;
            std::string name;
            std::string abbreviation;
            std::string displayName;
            std::string displayAbbreviation;
            InstrumentData instrumentData;
            MidiData midiData;
            std::vector<MeasureData> measures;
            
            inline int getNumStaves() const
            {
                int numStaves = 0;
                
                for( const auto& measure : measures )
                {
                    int temp = static_cast<int>( measure.staves.size() );
                    if( temp > numStaves )
                    {
                        numStaves = temp;
                    }
                }
                
                return numStaves;
            }
        };
    }
}
