// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/PositionData.h"
#include "mx/api/LineData.h"
#include "mx/api/DurationData.h"

namespace mx
{
    namespace api
    {
        // in the tuplet ratio, for example 2:3 for
        // triplets, the 'actual' part of the ratio
        // is the '2' and the 'normal' part of the
        // ratio is the '3'

        class TupletStart
        {
        public:

            // used to id when nested
            // -1 means unspecified
            int numberLevel;
            
            PositionData positionData;

            int actualNumber;
            DurationName actualDurationName;
            int actualDots;
            
            int normalNumber;
            DurationName normalDurationName;
            int normalDots;
            
            // show the number of notes in the tuplet
            // description ratio, for example 3 for triplets
            Bool showActualNumber;
            
            // show the number of 'normal' notes in the
            // tuplet ratio, for example the 2 in 3:2 for
            // a triplet or the 4 in 5:4 for quintuplet
            Bool showNormalNumber;
            
            Bool bracket;
            
            TupletStart()
            : numberLevel{ -1 }
            , positionData{}
            , actualNumber{ -1 }
            , actualDurationName{ api::DurationName::unspecified }
            , actualDots{ -1 }
            , normalNumber{ -1 }
            , normalDurationName{ api::DurationName::unspecified }
            , normalDots{ -1 }
            , showActualNumber{ Bool::unspecified }
            , showNormalNumber{ Bool::unspecified }
            , bracket{ Bool::unspecified }
            {
                
            }
        };

        class TupletEnd
        {
        public:
            // used to id when nested
            // -1 means unspecified
            int numberLevel;
            
            PositionData positionData;
            
            TupletEnd()
            : numberLevel{ -1 }
            , positionData{}
            {
                
            }
        };
    }
}
