// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/Decimals.h"
#include "mx/core/Enums.h"

#include <string>
#include <variant>

namespace mx
{
    namespace core
    {
        ///
        class FontSize
        {
        public:
            explicit FontSize();
            explicit FontSize( Decimal value );
            explicit FontSize( CssFontSize value );
            explicit FontSize( const std::string& value );
            bool getIsDecimal() const;
            bool getIsCssFontSize() const;
            void setDecimal( Decimal value );
            void setCssFontSize( CssFontSize value );
            Decimal getValueDecimal() const;
            CssFontSize getValueCssFontSize() const;
            bool parse( const std::string& value );
            
        private:
            std::variant<Decimal, CssFontSize> myValue;
        };
        
        std::string toString( const FontSize& value );
		std::ostream& toStream( std::ostream& os, const FontSize& value );
		std::ostream& operator<<( std::ostream& os, const FontSize& value );
    }
}