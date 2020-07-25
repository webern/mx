// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/XsString.h"

#include <string>
#include <iostream>
#include <vector>

namespace mx
{
    namespace core
    {
        /// @brief An XML Processing Instruction
        ///
        /// @detailed Will be written/read to/from the XML
        /// document as Processing Instructions
        ///
        class ProcessingInstruction
        {
        public:
            ProcessingInstruction();
            ProcessingInstruction( StringType inName, StringType inData );

        public:
            const StringType& getName() const;
            void setName( StringType inName );
            const StringType& getData() const;
            void setData( StringType inData );
            std::ostream& toStream( std::ostream& os ) const;
            StringType toString() const;

            /// place the instruction as the first child if possible. this may
            /// not be possible if the element has no other child elements.
            bool getIsChild() const;

            /// place the instruction as the first child if possible. this may
            /// not be possible if the element has no other child elements.
            void setIsChild( bool inValue );

        private:
            StringType myName;
            StringType myData;
            bool mIsChild;
        };

        using ProcessingInstructions = std::vector<ProcessingInstruction>;
    }
}
