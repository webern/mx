// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/xml/XDoc.h"
#include "mx/pugixml/pugixml.hpp"

namespace mx
{
    namespace xml
    {
        
        class PugiDoc;
        using PugiDocPtr = std::shared_ptr<PugiDoc>;

        class PugiDoc : public XDoc
        {
        public:
            virtual ~PugiDoc() = default;
            PugiDoc();
            PugiDoc( const PugiDoc& other ) = delete;
            PugiDoc( PugiDoc&& other ) = default;
            PugiDoc& operator=( const PugiDoc& other ) = delete;
            PugiDoc& operator=( PugiDoc&& other ) = default;
            
            virtual void loadStream( std::istream& is ) override;
            virtual void saveStream( std::ostream& os ) const override;
            
            virtual void loadFile( const std::string& filename ) override;
            virtual void saveFile( const std::string& filename ) const override;
            
            // Xml Declaration
            virtual XmlVersion getXmlVersion() const override;
            virtual void setXmlVersion( XmlVersion value ) override;
            virtual Encoding getEncoding() const override;
            virtual void setEncoding( Encoding value ) override;
            virtual bool getHasStandaloneAttribute() const override;
            virtual void setHasStandaloneAttribute( bool value ) override;
            virtual bool getIsStandalone() const override;
            virtual void setIsStandalone( bool value ) override;
            
            // Doctype Declaration
            virtual bool getHasDoctypeDeclaration() const override;
            virtual void setHasDoctypeDeclaration( bool value ) override;
            virtual std::string getDoctypeValue() const override;
            virtual void setDoctypeValue( const std::string& value ) override;
            
            // Node Access
            virtual XElementPtr getRoot() const override;
            
            virtual void setDoWriteByteOrderMark( bool value ) override;

        private:
            pugi::xml_document myDoc;
            XmlVersion myXmlVersion;
            Encoding myEncoding;
            bool myIsStandalone;
            bool myDoWriteBom;
            
            void parseXmlDeclarationValues();
            void parseXmlVersionFromDoc();
            void parseEncodingFromDoc();
            void parseStandalone();
            pugi::xml_node getDoctypeNode() const;
            pugi::xml_node getXmlDeclarationNode() const;
            pugi::xml_attribute getXmlDeclarationAttribute( const char* const name ) const;
        };
    }
}
