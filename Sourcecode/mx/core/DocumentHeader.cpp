// MusicXML Class Library v0.1.1
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/DocumentHeader.h"

namespace mx
{
    namespace core
    {
        std::ostream& stream( std::ostream& os, const DocumentType value )
        {
            switch ( value )
            {
                case DocumentType::partwise:
                {
                    os << "score-partwise";
                }
                    break;
                case DocumentType::timewise:
                {
                    os << "score-timewise";
                }
                    break;
                default:
                    break;
            }
            return os;
        }
        
        std::ostream& stream( std::ostream& os, const XmlVersion value )
        {
            switch ( value )
            {
                case XmlVersion::onePointZero:
                {
                    os << "1.0";
                }
                    break;
                default:
                    break;
            }
            return os;
        }
        
        std::ostream& stream( std::ostream& os, const DocumentEncoding value )
        {
            switch ( value )
            {
                case DocumentEncoding::utfEight:
                {
                    os << "UTF-8";
                }
                    break;
                default:
                    break;
            }
            return os;
        }
        
        std::ostream& stream( std::ostream& os, const MusicXmlVersion value )
        {
            switch ( value )
            {
                case MusicXmlVersion::ThreePointZero:
                {
                    os << "3.0";
                }
                    break;
                default:
                    break;
            }
            return os;
        }
        
#ifndef _MSC_VER /* Visual Studio Doesn't implement contexpr */
		constexpr
#endif 
		DocumentSpecifications::DocumentSpecifications()
        :xmlVersion( kDefaultXmlVersion )
        ,documentEncoding( kDefaultDocumentEncoding )
        ,documentType( kDefaultDocumentType )
        ,musicXmlVersion( kDefaultMusicXmlVersion )
        {}

        DocumentHeader::DocumentHeader()
        :mySpecs( DocumentSpecifications{} )
        {}
        
        DocumentHeader::~DocumentHeader() {}
        
        DocumentHeader::DocumentHeader( const DocumentSpecifications& specs )
        :mySpecs( specs )
        {}
        
        DocumentType DocumentHeader::getDocumentType() const
        {
            return mySpecs.documentType;
        }
        
        void DocumentHeader::setDocumentType( const DocumentType value )
        {
            mySpecs.documentType = value;
        }
        
        std::ostream& DocumentHeader::toStream( std::ostream& os ) const
        {
            os << R"(<?xml version=")";
            stream( os, mySpecs.xmlVersion );
            os << R"(" encoding=")";
            stream( os, mySpecs.documentEncoding );
            os << R"(" standalone="no"?>)";
            os << std::endl;
            
            os << R"(<!DOCTYPE )";
            stream( os, mySpecs.documentType );
            os << R"( PUBLIC "-//Recordare//DTD MusicXML )";
            stream( os, mySpecs.musicXmlVersion );
            os << " ";
            switch ( mySpecs.documentType )
            {
                case DocumentType::partwise:
                    os << "Partwise";
                    break;
                case DocumentType::timewise:
                    os << "Timewise";
                    break;
                default:
                    break;
            }
            os << R"(//EN" "http://www.musicxml.org/dtds/)";
            switch ( mySpecs.documentType )
            {
                case DocumentType::partwise:
                    os << "partwise";
                    break;
                case DocumentType::timewise:
                    os << "timewise";
                    break;
                default:
                    break;
            }
            os << R"(.dtd">)";
            return os;
        }
        
        DocumentHeaderPtr makeDocumentHeader()
        {
            return std::make_shared<DocumentHeader>();
        }
        DocumentHeaderPtr makeDocumentHeader( const DocumentSpecifications& specs )
        {
            return std::make_shared<DocumentHeader>( specs );
        }
    }
}