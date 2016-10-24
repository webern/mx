// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include <iosfwd>
#include <memory>
#include "mx/core/UnusedParameter.h"
#include "mx/core/AttributesInterface.h"

namespace mx
{
    namespace xml
    {
        class XElement;
    }
    
    namespace core
    {
        // This will be used by toStream to indent the XML tree
        extern const char* INDENT;
        
        class ElementInterface;
        using ElementPtr = std::shared_ptr<ElementInterface>;
        using ElementUPtr = std::unique_ptr<ElementInterface>;
        
        class ElementInterface
        {
        public:
            ElementInterface();
            virtual ~ElementInterface();
            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const = 0;
            virtual std::ostream& streamName( std::ostream& os ) const = 0;
            virtual bool hasContents() const = 0; // if this returns false then self-closing tag will be created
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const = 0;
            virtual std::ostream& streamOpenTag( std::ostream& os ) const;
            virtual std::ostream& streamCloseTag( std::ostream& os ) const;
            virtual std::ostream& streamSelfCloseTag( std::ostream& os ) const;
            virtual std::ostream& toStream( std::ostream& os, const int indentLevel ) const;
            virtual const std::string getElementName() const;
            virtual bool fromXElement( std::ostream& message, xml::XElement& xelement ) = 0;
        };
        
        std::ostream& indent( std::ostream& os, const int indentLevel );
        std::ostream& operator<<( std::ostream& os, const ElementInterface& value );
    }
}
