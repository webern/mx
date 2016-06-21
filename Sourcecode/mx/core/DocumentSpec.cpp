// MusicXML Class Library v0.1.1
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/DocumentSpec.h"
#include "mx/utility/Parse.h"
#include <sstream>

namespace mx
{
    namespace core
    {
        std::ostream& toStream( std::ostream& os, const DocumentChoice value )
        {
            switch ( value )
            {
                case DocumentChoice::partwise:
                    os << "partwise";
                    break;
                case DocumentChoice::timewise:
                    os << "timewise";
                default:
                    os << "error";
                    break;
            }
            return os;
        }
        
        
        std::string toString( const DocumentChoice value )
        {
            std::stringstream ss;
            toStream( ss, value );
            return ss.str();
        }
        
        
        DocumentChoice parseDocumentChoice( const std::string& str )
        {
            if( utility::compareCaseInsensitive( str, "partwise" ) )
            {
                return DocumentChoice::partwise;
            }
            else if ( utility::compareCaseInsensitive( str, "timewise" ) )
            {
                return DocumentChoice::timewise;
            }
            return DEFAULT_DOCUMENT_CHOICE;
        }
        
 
        std::ostream& toStream( std::ostream& os, const MusicXmlVersion value )
        {
            switch ( value )
            {
                case MusicXmlVersion::unknown:
                    os << "unknown";
                    break;
                case MusicXmlVersion::threePointZero:
                    os << "3.0";
                default:
                    os << "error";
                    break;
            }
            return os;
        }
        
        
        std::string toString( const MusicXmlVersion value )
        {
            std::stringstream ss;
            toStream( ss, value );
            return ss.str();
        }
        
        
        MusicXmlVersion parseMusicXmlVersion( const std::string& str )
        {
            if( utility::compareCaseInsensitive( str, "3.0" ) )
            {
                return MusicXmlVersion::threePointZero;
            }
            return DEFAULT_MUSIC_XML_VERSION;
        }

        
        std::ostream& toStream( std::ostream& os, const SpecificationType value )
        {
            switch ( value )
            {
                case SpecificationType::unknown:
                    os << "unknown";
                    break;
                case SpecificationType::dtd:
                    os << "dtd";
                case SpecificationType::xsd:
                    os << "xsd";
                default:
                    os << "error";
                    break;
            }
            return os;
        }
        
        
        std::string toString( const SpecificationType value )
        {
            std::stringstream ss;
            toStream( ss, value );
            return ss.str();
        }
        
        
        SpecificationType parseSpecificationType( const std::string& str )
        {
            if( utility::compareCaseInsensitive( str, "dtd" ) )
            {
                return SpecificationType::dtd;
            }
            else if( utility::compareCaseInsensitive( str, "xsd" ) )
            {
                return SpecificationType::dtd;
            }
            return DEFAULT_SPECIFICATION_TYPE;
        }
        
        
        DocumentSpec::DocumentSpec()
           // : xmlVersion( DEFAULT_XML_VERSION )
            //, encoding( DEFAULT_ENCODING )
            //, documentChoice( DEFAULT_DOCUMENT_CHOICE )
            //, musicXmlVersion( DEFAULT_MUSIC_XML_VERSION )
        {
            
        }
        
        
        DocumentSpec parseDocumentSpec( const xml::XDocCPtr& doc )
        {
            throw std::runtime_error( "not implemented" );
        }
    }
}