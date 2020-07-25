// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/XsToken.h"
#include <vector>

namespace mx
{
    namespace core
    {
        using XsTokenSet = std::vector<XsToken>;
        using XsTokenSetIter = XsTokenSet::iterator;
        using XsTokenSetIterConst = XsTokenSet::const_iterator;

        class CommaSeparatedText
        {
        public:
            CommaSeparatedText();
            CommaSeparatedText( const StringType& value );
            CommaSeparatedText( const XsTokenSet& values );
            const XsTokenSet& getValues() const;
            XsTokenSetIter getValuesBegin();
            XsTokenSetIter getValuesEnd();
            XsTokenSetIterConst getValuesBeginConst() const;
            XsTokenSetIterConst getValuesEndConst() const;
            void addValue( const XsToken& value );
            void setValues( const XsTokenSet& values );
            void parse( const StringType& commaSeparatedText );
            
        private:
            XsTokenSet myValues;
        
        };

        StringType toString( const CommaSeparatedText& value );
        std::ostream& toStream( std::ostream& os, const CommaSeparatedText& value );
        std::ostream& operator<<( std::ostream& os, const CommaSeparatedText& value );
    }
}
