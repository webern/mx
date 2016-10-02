// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

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
    }
}
