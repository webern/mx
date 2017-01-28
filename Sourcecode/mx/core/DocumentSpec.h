// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include <ostream>
#include <string>
#include <memory>

namespace mx
{
    namespace xml
    {
        class XDoc;
        using XDocCPtr = std::shared_ptr<const XDoc>;
    }
    
    namespace core
    {

        enum DocumentChoice
        {
            partwise,
            timewise
        };
        
        const DocumentChoice DEFAULT_DOCUMENT_CHOICE = DocumentChoice::partwise;
        std::ostream& toStream( std::ostream& os, const DocumentChoice value );
        std::string toString( const DocumentChoice value );
        DocumentChoice parseDocumentChoice( const std::string& str );
        
        
        
        enum class MusicXmlVersion
        {
            unknown,
            threePointZero
        };

        const MusicXmlVersion DEFAULT_MUSIC_XML_VERSION = MusicXmlVersion::threePointZero;
        std::ostream& toStream( std::ostream& os, const MusicXmlVersion value );
        std::string toString( const MusicXmlVersion value );
        MusicXmlVersion parseMusicXmlVersion( const std::string& str );
        
        enum class SpecificationType
        {
            unknown,
            dtd,
            xsd
        };
        
        const SpecificationType DEFAULT_SPECIFICATION_TYPE = SpecificationType::xsd;
        std::ostream& toStream( std::ostream& os, const SpecificationType value );
        std::string toString( const SpecificationType value );
        SpecificationType parseSpecificationType( const std::string& str );
        
        struct DocumentSpec
        {
            DocumentSpec();
            //XmlVersion xmlVersion;
            //Encoding encoding;
            DocumentChoice documentChoice;
            MusicXmlVersion musicXmlVersion;
        };
        
        DocumentSpec parseDocumentSpec( const xml::XDocCPtr& doc );
        
    }
}
