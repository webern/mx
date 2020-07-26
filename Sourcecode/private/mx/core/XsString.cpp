// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/XsString.h"

namespace mx
{
    namespace core
    {
        XsString::XsString()
        :myValue( "" )
        {

        }

        
        XsString::XsString( const StringType& value )
        :myValue( value )
        {

        }
        

        XsString::~XsString()
        {

        }
        

        StringType XsString::getValue() const
        {
            return myValue;
        }


        void XsString::setValue( const StringType& value )
        {
            myValue = value;
        }


        StringType toString( const XsString& xsstring )
        {
            return xsstring.getValue();
        }


        std::ostream& toStream(
            std::ostream& os,
            const XsString& xsstring,
            bool useXmlEscapeCharacters )
        {
            if( !useXmlEscapeCharacters )
            {
                return os << xsstring.getValue();
            }
            for( auto c : xsstring.getValue() )
            {
                switch ( c )
                {
                    case '<':
                        os << "&lt;";
                        break;
                        
                    case '>':
                        os << "&gt;";
                        break;
                        
                    case '&':
                        os << "&amp;";
                        break;
                        
                    default:
                        os << c;
                        break;
                }
            }
            return os;
        }


        std::ostream& operator<<( std::ostream& os, const XsString& xsstring )
        {
            return toStream( os, xsstring );
        }
    }
}
