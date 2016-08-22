// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include <iostream>
#include "mx/core/Decimals.h"
#include "mx/core/Enums.h"

namespace mx
{
    namespace core
    {
        class YesNoNumber
        {
        public:
            explicit YesNoNumber();
            explicit YesNoNumber( const Decimal& value );
            explicit YesNoNumber( const YesNo value );
            explicit YesNoNumber( const std::string& value );
            virtual ~YesNoNumber();
            YesNoNumber( const YesNoNumber& other );
            YesNoNumber( YesNoNumber&& other );
            YesNoNumber& operator=( const YesNoNumber& other );
            YesNoNumber& operator=( YesNoNumber&& other );
            
            bool getIsYesNo() const;
            bool getIsNumber() const;
            void setValue( const YesNo value );
            void setValue( const Decimal& value );
            YesNo getValueYesNo() const;
            Decimal getValueNumber() const;
            void parse( const std::string& value );
            
        private:
            class impl;
            std::unique_ptr<impl> myImpl;
        };
        
        std::string toString( const YesNoNumber& value );
		std::ostream& toStream( std::ostream& os, const YesNoNumber& value );
		std::ostream& operator<<( std::ostream& os, const YesNoNumber& value );
    }
}
