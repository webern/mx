// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once
#ifdef FAIL

#include "mx/api/MarkData.h"
#include "mx/api/ApiCommon.h"
#include "mx/impl/Converter.h"

namespace mx
{
	namespace core
	{
        class PartwiseMeasure;
        using PartwiseMeasurePtr = std::shared_ptr<PartwiseMeasure>;
        class Direction;
        using DirectionPtr = std::shared_ptr<Direction>;
	}

    namespace impl
    {
        class DirectionWriter
        {
        public:

            DirectionWriter( const core::PartwiseMeasurePtr& inPartwiseMeasure );
            
            // get the current direction buffer's values
            api::Placement getPlacement() const;
            
            // will return -1 if no staff element is present
            int getStaffIndex() const;
            
            // will return 0 if no offset element is present
            int getOffset() const;
            
            // destroy and reallocate a new direction element
            // without inserting it into the measure. the
            // placement, offset and staff values remain unset
            // until the first direction is added to the buffer
            void newBuffer( api::Placement placement );

            // if the direction element has any data it will
            // be added to the measure before being cleared
            // and reallocated.  if the properties element
            // does not have any data then nothing happens.
            void flushBuffer();

            // returns true if the direction element does
            // not contain any information
            bool getIsDirectionEmpty() const;

            // this will add the direction to the current
            // direction buffer provided that the Placement
            // value of the mark, as well as the offset and
            // staff values, are consistent with the
            // buffer's values.  If not, then a flush will
            // occur and a new buffer will be started, to
            // which your mark will be added
            void writeDirection( const api::MarkData mark, int staffIndex );
            
        private:
            void allocate();
            api::Placement getPlacement( const core::Direction& direction ) const;
            int getStaffIndex( const core::Direction& direction ) const;
            int getOffset( const core::Direction& direction ) const;
            bool isCompatible( const api::MarkData mark, int staffIndex ) const;

        private:
            core::DirectionPtr myDirection;
            const core::PartwiseMeasurePtr& myPartwiseMeasure;
            const Converter myConverter;
        };
    }
}
#endif
