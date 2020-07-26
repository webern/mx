// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include <iostream>
#include <string>

namespace mx
{
    namespace core
    {
        using StringType = std::string;
        
        class XsString
        {
        public:
            XsString();
            XsString( const StringType& value );
            virtual ~XsString();
            StringType getValue() const;
            virtual void setValue( const StringType& value );
        private:
            StringType myValue;
        };
        
        StringType toString( const XsString& xsstring );
        std::ostream& toStream( std::ostream& os, const XsString& xsstring, bool useXmlEscapeCharacters = true );
        std::ostream& operator<<( std::ostream& os, const XsString& xsstring );
    }
}
