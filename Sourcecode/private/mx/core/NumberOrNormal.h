// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/Decimals.h"

#include <ostream>
#include <string>
#include <variant>

namespace mx
{
    namespace core
    {
        ///
        class NumberOrNormal
        {
        public:
            explicit NumberOrNormal();
            explicit NumberOrNormal( Decimal value );
            explicit NumberOrNormal( const std::string& value );
            bool getIsDecimal() const;
            void setDecimal( Decimal value );
            Decimal getValueDecimal() const;
            bool parse( const std::string& value );
            
        private:
            std::variant<Decimal> myValue;
        };
        
        std::string toString( const NumberOrNormal& value );
		std::ostream& toStream( std::ostream& os, const NumberOrNormal& value );
		std::ostream& operator<<( std::ostream& os, const NumberOrNormal& value );
    }
}