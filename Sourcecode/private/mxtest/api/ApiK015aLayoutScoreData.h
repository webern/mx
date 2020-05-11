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
        score.layout.pageLayout.pageHeight = 1545;
        score.layout.pageLayout.pageWidth = 1194;
        score.layout.scalingMillimeters = 7.2319;
        score.layout.scalingTenths = 40;
        score.layout.pageLayout.pageMargins.oddPageLeftMargin = 70;
        score.layout.pageLayout.pageMargins.oddPageRightMargin = 70;
        score.layout.pageLayout.pageMargins.oddPageTopMargin = 88;
        score.layout.pageLayout.pageMargins.oddPageBottomMargin = 88;
        score.layout.pageLayout.pageMargins.evenPageLeftMargin = 70;
        score.layout.pageLayout.pageMargins.evenPageRightMargin = 70;
        score.layout.pageLayout.pageMargins.evenPageTopMargin = 88;
        score.layout.pageLayout.pageMargins.evenPageBottomMargin = 88;
        
        score.layout.systemLeftMargin = 125;
        score.layout.systemRightMargin = 503;
        score.layout.topSystemDistance = 70;
        score.layout.systemDistance = 121;
        
        SystemData system;
        system.measureIndex = 0;
        system.rightMargin = 0;
        system.leftMargin = 515;
        system.topSystemDistance = 211;
        
        /*
         : measureIndex{ -1 }
         , rightMargin{ 0 }
         , leftMargin{ 0 }
         , isMarginSpecified{ false }
         , systemDistance{ 0 }
         , isSystemDistanceSpecified{ false }
         , topSystemDistance{ 0 }
         , isTopSystemDistanceSpecified{ false }
         */
        score.systems.emplace( system );
        
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
        measure->width =
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
