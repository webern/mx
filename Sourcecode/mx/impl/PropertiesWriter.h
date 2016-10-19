// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/TimeSignatureData.h"
#include "mx/api/KeyData.h"
#include "mx/api/ClefData.h"

namespace mx
{
	namespace core
	{
        class PartwiseMeasure;
        using PartwiseMeasurePtr = std::shared_ptr<PartwiseMeasure>;
        class Properties;
        using PropertiesPtr = std::shared_ptr<Properties>;
	}

    namespace impl
    {
        class PropertiesWriter
        {
        public:

            PropertiesWriter( const core::PartwiseMeasurePtr& inPartwiseMeasure );
            
            // destroy and reallocate a new properties element
            // without inserting it into the measure
            void clearBuffer();

            // if the properties element has any data it will
            // be added to the measure before being cleared
            // and reallocated.  if the properties element
            // does not have any data then nothing happens.
            void flushBuffer();

            // returns true if the properties element does
            // not contain any information
            bool isPropertiesEmpty();

            void writeDivisions( int value );
            void writeKey( int staffIndex, const api::KeyData& inKeyData );
            void writeTime( const api::TimeSignatureData& value );
            void writeNumStaves( int value );
            void writeClef( int staffIndex, const api::ClefData& inClefData );

        private:
            void allocate();

        private:
            core::PropertiesPtr myProperties;
            const core::PartwiseMeasurePtr& myPartwiseMeasure;
        };
    }
}
