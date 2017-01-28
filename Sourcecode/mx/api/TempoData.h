// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once
#include "mx/api/PrintData.h"
#include "mx/api/PositionData.h"
#include "mx/api/DurationData.h"

namespace mx
{
    namespace api
    {
        enum class TempoType
        {
            unspecified,
            beatsPerMinute,
            metricModulation,
            tempoText
        };
        
        struct BeatsPerMinute
        {
            DurationName durationName;
            int dots;
            int beatsPerMinute;
            
            BeatsPerMinute()
            : durationName{ DurationName::unspecified }
            , dots{ -1 }
            , beatsPerMinute{ -1 }
            {
                
            }
        };
        
        struct MetricModulation
        {
            DurationName leftDurationName;
            int leftDots;
            DurationName rightDurationName;
            int rightDots;
            BeatsPerMinute playbackBeatsPerMinute;

            MetricModulation()
            : leftDurationName{ DurationName::unspecified }
            , leftDots{ -1 }
            , rightDurationName{ DurationName::unspecified }
            , rightDots{ -1 }
            , playbackBeatsPerMinute{}
            {
                
            }
        };
        
        struct TempoText
        {
            std::string tempoText;
            BeatsPerMinute playbackBeatsPerMinute;
        };
        
        class TempoData
        {
        public:
            int tickTime;
            Bool isParenthetical;
            PrintData printData;
            PositionData positionData;
            TempoType tempoType;
            
            // only used when tempoType is 'beatsPerMinute'
            BeatsPerMinute beatsPerMinute;
            
            // only used when tempoType is 'metricModulation'
            MetricModulation metricModulation;
            
            // only used when tempoType is 'tempoText'
            TempoText tempoText;
        };

        MXAPI_EQUALS_BEGIN( BeatsPerMinute )
        MXAPI_EQUALS_MEMBER( durationName )
        MXAPI_EQUALS_MEMBER( dots )
        MXAPI_EQUALS_MEMBER( beatsPerMinute )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( BeatsPerMinute );
        
        MXAPI_EQUALS_BEGIN( MetricModulation )
        MXAPI_EQUALS_MEMBER( leftDurationName )
        MXAPI_EQUALS_MEMBER( leftDots )
        MXAPI_EQUALS_MEMBER( rightDurationName )
        MXAPI_EQUALS_MEMBER( rightDots )
        MXAPI_EQUALS_MEMBER( playbackBeatsPerMinute )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( MetricModulation );
        
        MXAPI_EQUALS_BEGIN( TempoText )
        MXAPI_EQUALS_MEMBER( tempoText )
        MXAPI_EQUALS_MEMBER( playbackBeatsPerMinute )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( TempoText );
        
        MXAPI_EQUALS_BEGIN( TempoData )
        MXAPI_EQUALS_MEMBER( tickTime )
        MXAPI_EQUALS_MEMBER( isParenthetical )
        MXAPI_EQUALS_MEMBER( printData )
        MXAPI_EQUALS_MEMBER( positionData )
        MXAPI_EQUALS_MEMBER( tempoType )
        MXAPI_EQUALS_MEMBER( beatsPerMinute )
        MXAPI_EQUALS_MEMBER( metricModulation )
        MXAPI_EQUALS_MEMBER( tempoText )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( TempoData );
    }
}
