// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ScoreData.h"
#include "mx/impl/Converter.h"
#include "mx/core/Enums.h"

namespace mxtest
{
    inline mx::api::ScoreData apiK015aLayoutScoreData()
    {
        using namespace mx::api;
        ScoreData score;
        score.parts.emplace_back( PartData{} );
        score.ticksPerQuarter = 240;
        score.movementTitle = "k015a_System_Layout";
        
        SupportedItem supported;
        supported.elementName = "print";
        supported.attributeName = "new-system";
        supported.specificValue = "yes";
        score.encoding.supportedItems.emplace_back( supported );
        supported.attributeName = "new-page";
        score.encoding.supportedItems.emplace_back( supported );
        score.layout.pageLayout.size = SizeData{ 1545, 1194 };
        score.layout.scalingMillimeters = 7.2319;
        score.layout.scalingTenths = 40;
        score.layout.pageLayout.margins.odd = MarginsData{70, 70, 88, 88 };
        score.layout.pageLayout.margins.even = MarginsData{70, 70, 88, 88 };
        score.layout.systemLayout.margins = LeftRight{ 125, 503 };
        score.layout.systemLayout.topSystemDistance = 70;
        score.layout.systemLayout.systemDistance = 121;
        
        SystemData system;
        MeasureIndex measureIndex = 0;
        system.layout.margins = LeftRight{ 0, 515 };
        system.layout.topSystemDistance = 211;
        score.xxbadnames[measureIndex].system = system;
        
        /*
         <print page-number="1">
           <system-layout>
             <system-margins>
               <left-margin>515</left-margin>
               <right-margin>0</right-margin>
             </system-margins>
               <top-system-distance>211</top-system-distance>
           </system-layout>
           <measure-numbering>system</measure-numbering>
         </print>
         */
        
        auto& part = score.parts.front();
        part.uniqueId = "P1";
        part.name = "MusicXML Part";

        // 1
        part.measures.emplace_back( MeasureData{} );
        auto measure = &part.measures.back();
        measure->width = 0.0;
        measure->timeSignature.beats = 4;
        measure->timeSignature.beatType = 4;
        measure->timeSignature.isImplicit = false;
        measure->staves.emplace_back( StaffData{} );
        auto staff = &measure->staves.back();
        staff->clefs.emplace_back( ClefData{} );
        auto& clef = staff->clefs.back();
        clef.line = 2;
        
        staff->voices[0].notes.emplace_back( NoteData{} );
        

        return score;
    }
}
