// MusicXML Class Library v0.1.1
// Copyright (c) 2015 - 2016 by Matthew James Briggs

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
            
            virtual void parse( std::istream& is ) override;
            virtual void write( std::ostream& os ) const override;
            
            virtual void parse( const std::string& filename ) override;
            virtual void write( const std::string& filename ) const override;
            
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

        private:
            pugi::xml_document myDoc;
            XmlVersion myXmlVersion;
            Encoding myEncoding;
            bool myIsStandalone;
            
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