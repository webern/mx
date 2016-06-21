// MusicXML Class Library v0.1.1
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once


#include "mx/xml/XDocSpec.h"
#include "mx/xml/XElement.h"

#include <iostream>
#include <memory>

namespace mx
{
    namespace xml
    {
        
        class XDoc;
        using XDocPtr = std::shared_ptr<XDoc>;
        using XDocCPtr = std::shared_ptr<const XDoc>;
        
        class XDoc : public std::enable_shared_from_this<XDoc>
        {
        public:
            virtual ~XDoc() {}

            // these can throw std::runtime_error
            virtual void parse( std::istream& is ) = 0;
            virtual void write( std::ostream& os ) const = 0;
            
            // these can throw std::runtime_error
            virtual void parse( const std::string& filename ) = 0;
            virtual void write( const std::string& filename ) const = 0;
            
            // Xml Declaration
            virtual XmlVersion getXmlVersion() const = 0;
            virtual void setXmlVersion( XmlVersion value ) = 0;
            virtual Encoding getEncoding() const = 0;
            virtual void setEncoding( Encoding value ) = 0;
            virtual bool getHasStandaloneAttribute() const = 0;
            virtual void setHasStandaloneAttribute( bool value ) = 0;
            virtual bool getIsStandaloneValueYes() const = 0;
            virtual void setIsStandaloneValueYes( bool value ) = 0;
            
            // Doctype Declaration
            virtual bool getHasDoctypeDeclaration() const = 0;
            virtual void setHasDoctypeDeclaration( bool value ) = 0;
            virtual std::string getDoctypeDeclaration() const = 0;
            virtual void setDoctypeDeclaration( const std::string& value ) = 0;
            
            // Node Access
            virtual XElementPtr getRoot() const = 0;
            
        };

    }
}