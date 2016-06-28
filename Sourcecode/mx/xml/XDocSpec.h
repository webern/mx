// MusicXML Class Library v0.1.1
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once
#include "mx/utility/Parse.h"

#include <string>
#include <sstream>
#include <memory>

namespace mx
{
    namespace xml
    {
        
        enum class XmlVersion
        {
            unknown,
            onePointZero,
            onePointOne
        };
        
        const XmlVersion DEFAULT_XML_VERSION = XmlVersion::onePointZero;
        
        inline std::ostream& toStream( std::ostream& os, const XmlVersion value )
        {
            switch ( value )
            {
                case XmlVersion::unknown:
                    os << "unknown";
                    break;
                case XmlVersion::onePointZero:
                    os << "1.0";
                    break;
                case XmlVersion::onePointOne:
                    os << "1.1";
                    break;
                default:
                    os << "error";
                    break;
            }
            return os;
        }


        inline std::string toString( const XmlVersion value )
        {
            std::stringstream ss;
            toStream( ss, value );
            return ss.str();
        }


        inline XmlVersion parseXmlVersion( const std::string& str )
        {
            if( utility::compareCaseInsensitive( str, "1.0" ) )
            {
                return XmlVersion::onePointZero;
            }
            
            if( utility::compareCaseInsensitive( str, "1.1" ) )
            {
                return XmlVersion::onePointOne;
            }
            
            return DEFAULT_XML_VERSION;
        }

        
        enum class Encoding
        {
            unknown,
            utfEight,
            utfSixteen
        };

        const Encoding DEFAULT_ENCODING = Encoding::utfEight;
        
        inline std::ostream& toStream( std::ostream& os, const Encoding value )
        {
            switch ( value )
            {
                case Encoding::unknown:
                    os << "unknown";
                    break;
                case Encoding::utfEight:
                    os << "UTF-8";
                    break;
                case Encoding::utfSixteen:
                    os << "UTF-16";
                    break;
                default:
                    os << "error";
                    break;
            }
            return os;
        }


        inline std::string toString( const Encoding value )
        {
            std::stringstream ss;
            toStream( ss, value );
            return ss.str();
        }


        inline Encoding parseEncoding( const std::string& str )
        {
            if( utility::compareCaseInsensitive( str, "UTF-8" ) )
            {
                return Encoding::utfEight;
            }
            if( utility::compareCaseInsensitive( str, "UTF-16" ) )
            {
                return Encoding::utfSixteen;
            }
            return DEFAULT_ENCODING;
        }
    }
}