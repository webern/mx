// Copyright (c) Matthew James Briggs

#pragma once

#include "ezxml/XDocSpec.h"
#include "ezxml/XElement.h"

#include <iostream>
#include <memory>

namespace ezxml
{
    class XDoc;
    using XDocPtr = std::shared_ptr<XDoc>;
    using XDocCPtr = std::shared_ptr<const XDoc>;

    class XDoc : public std::enable_shared_from_this<XDoc>
    {
    public:
        virtual ~XDoc() = default;

        // these can throw std::runtime_error
        virtual void loadStream( std::istream& is ) = 0;
        virtual void saveStream( std::ostream& os ) const = 0;

        // these can throw std::runtime_error
        virtual void loadFile( const std::string& filename ) = 0;
        virtual void saveFile( const std::string& filename ) const = 0;

        // Xml Declaration
        virtual XmlVersion getXmlVersion() const = 0;
        virtual void setXmlVersion( XmlVersion value ) = 0;
        virtual Encoding getEncoding() const = 0;
        virtual void setEncoding( Encoding value ) = 0;
        virtual bool getHasStandaloneAttribute() const = 0;
        virtual void setHasStandaloneAttribute( bool value ) = 0;
        virtual bool getIsStandalone() const = 0;
        virtual void setIsStandalone( bool value ) = 0;

        // Doctype Declaration
        virtual bool getHasDoctypeDeclaration() const = 0;
        virtual void setHasDoctypeDeclaration( bool value ) = 0;
        virtual std::string getDoctypeValue() const = 0;
        virtual void setDoctypeValue( const std::string& value ) = 0;

        // when calling the write function, this value will
        // determine whether or not to include the BOM
        virtual void setDoWriteByteOrderMark( bool value ) = 0;

        // Node Access
        virtual XElementPtr getRoot() const = 0;
    };
}
