// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/TimeSignatureData.h"
#include "mx/api/KeyData.h"
#include "mx/api/ClefData.h"
#include "mx/api/TransposeData.h"
#include <memory>

namespace mx
{
    namespace core
    {
        class PartwiseMeasure;
        using PartwiseMeasurePtr = std::shared_ptr<PartwiseMeasure>;
        class Properties;
        using PropertiesPtr = std::shared_ptr<Properties>;
        class Key;
        using KeyPtr = std::shared_ptr<Key>;
    }

    namespace impl
    {
        /// Provides a way to ensure that properties (i.e. things in the `<attributes>` element) are
        /// added to a single <attributes> element instead of creating a new <attributes> element
        /// for each property.
        ///
        /// The design is weird. It holds the current meausre that we are writing, and acts like a
        /// buffer of properties. You can write things like clefs and time signatures to the
        /// PropertiesWriter, and then when you call `flushBuffer()` everything gets written to the
        /// measure.
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
            static void writeTraditionalKey( const api::KeyData& inKeyData, mx::core::KeyPtr& ioKey );
            static void writeNonTraditionalKey( const api::KeyData& inKeyData, mx::core::KeyPtr& ioKey );
            void writeTime( const api::TimeSignatureData& value );
            void writeNumStaves( int value );
            void writeClef( int staffIndex, const api::ClefData& inClefData );
            void writeTranspose( const api::TransposeData& inTransposeData );

        private:
            void allocate();

        private:
            core::PropertiesPtr myProperties;
            const core::PartwiseMeasurePtr& myPartwiseMeasure;
        };
    }
}
