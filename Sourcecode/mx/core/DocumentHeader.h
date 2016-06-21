// MusicXML Class Library v0.1.1
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include <iostream>
#include <memory>

namespace mx
{
    namespace core
    {
        enum class DocumentType
        {
            partwise = 1,
            timewise = 2
        };
#ifndef _MSC_VER /* Visual Studio Doesn't implement contexpr */
		constexpr
#endif 
		const DocumentType kDefaultDocumentType = DocumentType::partwise;
        std::ostream& stream( std::ostream& os, const DocumentType value );
        
        enum class XmlVersion
        {
            onePointZero = 1
        };
#ifndef _MSC_VER /* Visual Studio Doesn't implement contexpr */
		constexpr
#endif 
		const XmlVersion kDefaultXmlVersion = XmlVersion::onePointZero;
        std::ostream& stream( std::ostream& os, const XmlVersion value );
        
        enum class DocumentEncoding
        {
            utfEight = 1
        };
#ifndef _MSC_VER /* Visual Studio Doesn't implement contexpr */
		constexpr
#endif 
		const DocumentEncoding kDefaultDocumentEncoding = DocumentEncoding::utfEight;
        std::ostream& stream( std::ostream& os, const DocumentEncoding value );
        
        enum class MusicXmlVersion
        {
            ThreePointZero = 1
        };
#ifndef _MSC_VER /* Visual Studio Doesn't implement contexpr */
		constexpr
#endif 
		const MusicXmlVersion kDefaultMusicXmlVersion = MusicXmlVersion::ThreePointZero;
        std::ostream& stream( std::ostream& os, const MusicXmlVersion value );
        
        struct DocumentSpecifications
        {
#ifndef _MSC_VER /* Visual Studio Doesn't implement contexpr */
			constexpr
#endif 
			DocumentSpecifications();
            XmlVersion xmlVersion;
            DocumentEncoding documentEncoding;
            DocumentType documentType;
            MusicXmlVersion musicXmlVersion;
        };
        
        /* this class is basically a stub
         it could come in handy for forward
         compatibility later */
        class DocumentHeader;
        using DocumentHeaderPtr = std::shared_ptr<DocumentHeader>;
        DocumentHeaderPtr makeDocumentHeader();
        DocumentHeaderPtr makeDocumentHeader( const DocumentSpecifications& specs );
        
        class DocumentHeader
        {
        public:
            DocumentHeader();
            DocumentHeader( const DocumentSpecifications& specs );
            virtual ~DocumentHeader();
            virtual std::ostream& toStream( std::ostream& os ) const;
            // XmlVersion getXmlVersion() const;
            // void setXmlVersion( const XmlVersion value );
            // DocumentEncoding getDocumentEncoding() const;
            // void setDocumentEncoding( const DocumentEncoding value );
            DocumentType getDocumentType() const;
            void setDocumentType( const DocumentType value );
        private:
            DocumentSpecifications mySpecs;
        };
        
    }
}