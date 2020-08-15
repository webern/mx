// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/Decimals.h"
#include "mx/core/Enums.h"

#include <variant>

namespace mx
{
    namespace core
    {
        class FontSize
        {
        public:
            explicit FontSize();
            explicit FontSize( const Decimal& value );
            explicit FontSize( const CssFontSize value );
            explicit FontSize( const std::string& value );
            virtual ~FontSize();
            FontSize( const FontSize& other );
            FontSize( FontSize&& other );
            FontSize& operator=( const FontSize& other );
            FontSize& operator=( FontSize&& other );
            
            bool getIsCssFontSize() const;
            bool getIsNumber() const;
            void setValue( const CssFontSize value );
            void setValue( const Decimal& value );
            CssFontSize getValueCssFontSize() const;
            Decimal getValueNumber() const;
            bool parse( const std::string& value );
            
        private:
            std::variant<CssFontSize, Decimal> myValue;
        };
        
        std::string toString( const FontSize& value );
		std::ostream& toStream( std::ostream& os, const FontSize& value );
		std::ostream& operator<<( std::ostream& os, const FontSize& value );
    }
}
