// MusicXML Class Library v0.1.1
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include <iostream>
#include <memory>
#include "mx/core/AttributesInterface.h"

namespace mx
{
    namespace xml
    {
        class XElement;
    }
    
    namespace core
    {
        /* This will be used throughout when writing XML
         to indent the XML tree */
		extern const char* INDENT;
        
        struct ElementInterface;
        using ElementPtr = std::shared_ptr<ElementInterface>;
        using ElementUPtr = std::unique_ptr<ElementInterface>;
        
        struct ElementInterface
        {
        public:
            ElementInterface();
            virtual ~ElementInterface();
            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const = 0;
            virtual std::ostream& streamName( std::ostream& os ) const = 0;
            virtual bool hasContents() const = 0; /* if this returns false then self-closing tag will be created */
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const = 0;
            virtual std::ostream& streamOpenTag( std::ostream& os ) const;
            virtual std::ostream& streamCloseTag( std::ostream& os ) const;
            virtual std::ostream& streamSelfCloseTag( std::ostream& os ) const;
            virtual std::ostream& toStream( std::ostream& os, const int indentLevel ) const;
            virtual bool fromXElement( std::ostream& message, xml::XElement& xelement )
            {
                // TODO - remove and make it pure virtual
                streamName( message );
                message << " - import from XDoc not implemented ";
                return false;
            }
        };
        
        std::ostream& indent( std::ostream& os, const int indentLevel );
        std::ostream& operator<<( std::ostream& os, const ElementInterface& value );
    }
}
