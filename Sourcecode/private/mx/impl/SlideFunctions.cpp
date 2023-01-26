// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/api/SpannerData.h"
#include "mx/core/elements/Slide.h"
#include "mx/impl/SlideFunctions.h"
#include "mx/impl/MarkDataFunctions.h"

namespace mx
{
    namespace impl
    {

    SlideFunctions::SlideFunctions( const core::Slide& inSlide, impl::Cursor inCursor )
        : mySlide{ inSlide }
        , myCursor{ inCursor }
        {
        }
        
        void SlideFunctions::parseSlide( api::NoteAttachmentData& inNoteAttachmentData ) const
        {
//            const auto& attr = mySlide.getAttributes();
//            const auto slideType = attr->type;
//
//            api::SpannerStart spannerData;
//            spannerData.spannerType = api::SpannerType::slide;
//            spannerData.tickTimePosition = myCursor.tickTimePosition;
//
//            if( attr->hasNumber )
//            {
//                spannerData.numberLevel = attr->number.getValue();
//            }
//
//            if( slideType == core::StartStop::start )
//            {
//                inNoteAttachmentData.noteAttachmentData.spannerStarts.emplace_back( std::move( spannerData ) );
//            }
//            else
//            {
//                inNoteAttachmentData.noteAttachmentData.spannerStops.emplace_back( std::move( spannerData ) );
//            }
        }
    }
}
