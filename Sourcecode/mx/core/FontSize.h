// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include <iostream>
#include "mx/core/Decimals.h"
#include "mx/core/Enums.h"

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
            void parse( const std::string& value );
            
        private:
            class impl;
            std::unique_ptr<impl> myImpl;
        };
        
        std::string toString( const FontSize& value );
		std::ostream& toStream( std::ostream& os, const FontSize& value );
		std::ostream& operator<<( std::ostream& os, const FontSize& value );
    }
}
