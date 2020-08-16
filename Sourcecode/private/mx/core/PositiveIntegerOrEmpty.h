// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/Integers.h"

#include <ostream>
#include <string>
#include <variant>

namespace mx
{
    namespace core
    {
        /// The positive-integer-or-empty values can be either a positive integer or an
        /// empty string.
        class PositiveIntegerOrEmpty
        {
        public:
            explicit PositiveIntegerOrEmpty();
            explicit PositiveIntegerOrEmpty( PositiveInteger value );
            explicit PositiveIntegerOrEmpty( const std::string& value );
            bool getIsPositiveInteger() const;
            void setPositiveInteger( PositiveInteger value );
            PositiveInteger getValuePositiveInteger() const;
            bool parse( const std::string& value );
            
        private:
            std::variant<PositiveInteger> myValue;
        };
        
        std::string toString( const PositiveIntegerOrEmpty& value );
		std::ostream& toStream( std::ostream& os, const PositiveIntegerOrEmpty& value );
		std::ostream& operator<<( std::ostream& os, const PositiveIntegerOrEmpty& value );
    }
}
