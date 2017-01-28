// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

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
            
            // because the MuscXML specification says this "Formatting
            // attributes for the part-name element are deprecated in
            // Version 2.0 in favor of the new part-name-display and
            // part-abbreviation-display elements" the name will always
            // be written with 'print-object="no". You must populate
            // the displayName field in order to have a name displayed.
            std::string name;

            // because the MuscXML specification says this "Formatting
            // attributes for the part-name element are deprecated in
            // Version 2.0 in favor of the new part-name-display and
            // part-abbreviation-display elements" the abbreviation
            // will always be written with 'print-object="no". You
            // must populate the displayAbbreviation field in order to
            // have an abbreviation displayed.
            std::string abbreviation;

            std::string displayName;
            PrintData displayNamePrintData;
            PositionData displayNamePositionData;
            
            std::string displayAbbreviation;
            PrintData displayAbbreviationPrintData;
            PositionData displayAbbreviationPositionData;
            
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
        
        MXAPI_EQUALS_BEGIN( InstrumentData )
        MXAPI_EQUALS_MEMBER( uniqueId )
        MXAPI_EQUALS_MEMBER( name )
        MXAPI_EQUALS_MEMBER( abbreviation )
        MXAPI_EQUALS_MEMBER( sound )
        MXAPI_EQUALS_MEMBER( soloOrEnsemble )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( InstrumentData );
        
        MXAPI_EQUALS_BEGIN( MidiData )
        MXAPI_EQUALS_MEMBER( virtualLibrary )
        MXAPI_EQUALS_MEMBER( virtualName )
        MXAPI_EQUALS_MEMBER( device )
        MXAPI_EQUALS_MEMBER( uniqueId )
        MXAPI_EQUALS_MEMBER( name )
        MXAPI_EQUALS_MEMBER( bank )
        MXAPI_EQUALS_MEMBER( channel )
        MXAPI_EQUALS_MEMBER( program )
        MXAPI_EQUALS_MEMBER( unpitched )
        MXAPI_EQUALS_MEMBER( volume )
        MXAPI_EQUALS_MEMBER( isVolumeSpecified )
        MXAPI_EQUALS_MEMBER( pan )
        MXAPI_EQUALS_MEMBER( isPanSpecified )
        MXAPI_EQUALS_MEMBER( elevation )
        MXAPI_EQUALS_MEMBER( isElevationSpecified )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( MidiData );
        
        MXAPI_EQUALS_BEGIN( PartData )
        MXAPI_EQUALS_MEMBER( uniqueId )
        MXAPI_EQUALS_MEMBER( name )
        MXAPI_EQUALS_MEMBER( abbreviation )
        MXAPI_EQUALS_MEMBER( displayName )
        MXAPI_EQUALS_MEMBER( displayNamePrintData )
        MXAPI_EQUALS_MEMBER( displayNamePositionData )
        MXAPI_EQUALS_MEMBER( displayAbbreviation )
        MXAPI_EQUALS_MEMBER( displayAbbreviationPrintData )
        MXAPI_EQUALS_MEMBER( displayAbbreviationPositionData )
        MXAPI_EQUALS_MEMBER( instrumentData )
        MXAPI_EQUALS_MEMBER( midiData )
        MXAPI_EQUALS_MEMBER( measures )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( PartData );
    }
}
