// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/Decimals.h"
#include "mx/core/Enums.h"

#include <ostream>
#include <string>
#include <variant>

namespace mx
{
    namespace core
    {
        ///
        class YesNoNumber
        {
        public:
            explicit YesNoNumber();
            explicit YesNoNumber( YesNo value );
            explicit YesNoNumber( Decimal value );
            explicit YesNoNumber( const std::string& value );
            bool getIsYesNo() const;
            bool getIsDecimal() const;
            void setYesNo( YesNo value );
            void setDecimal( Decimal value );
            YesNo getValueYesNo() const;
            Decimal getValueDecimal() const;
            bool parse( const std::string& value );
            
        private:
            std::variant<YesNo, Decimal> myValue;
        };
        
        std::string toString( const YesNoNumber& value );
		std::ostream& toStream( std::ostream& os, const YesNoNumber& value );
		std::ostream& operator<<( std::ostream& os, const YesNoNumber& value );
    }
}