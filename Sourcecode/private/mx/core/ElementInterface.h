// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include <iosfwd>
#include <memory>
#include "mx/core/UnusedParameter.h"
#include "mx/core/AttributesInterface.h"
#include "mx/core/ProcessingInstruction.h"

namespace ezxml
{
    class XElement;
}

namespace mx
{
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
            virtual bool fromXElement( std::ostream& message, ezxml::XElement& xelement ) final;
            virtual const ProcessingInstructions& getProcessingInstructions() const final;
            virtual void clearProcessingInstructions() final;
            virtual void addProcessingInstruction( ProcessingInstruction inProcessingInstruction ) final;

        private:
            virtual bool fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement ) = 0;

        private:
            ProcessingInstructions myProcessingInstructions;
            std::ostream& writeChildProcessingInstructions( std::ostream& os, const int indentLevel ) const;
            std::ostream& writeSiblingProcessingInstructions( std::ostream& os, const int indentLevel ) const;
            std::ostream& writeAllProcessingInstructions( std::ostream& os, const int indentLevel ) const;
            std::ostream& streamWithoutProcessingInstructions( std::ostream& os, const int indentLevel ) const;
            std::ostream& streamWithProcessingInstructions( std::ostream& os, const int indentLevel, const bool inHasChildren ) const;
        };
        
        std::ostream& indent( std::ostream& os, const int indentLevel );
        std::ostream& operator<<( std::ostream& os, const ElementInterface& value );
    }
}
