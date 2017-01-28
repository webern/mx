// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ELEMENT( ElisionSyllabicGroup )
        MX_FORWARD_DECLARE_ELEMENT( Text )
        MX_FORWARD_DECLARE_ELEMENT( ElisionSyllabicTextGroup )

        inline ElisionSyllabicTextGroupPtr makeElisionSyllabicTextGroup() { return std::make_shared<ElisionSyllabicTextGroup>(); }

        class ElisionSyllabicTextGroup : public ElementInterface
        {
        public:
            ElisionSyllabicTextGroup();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ ElisionSyllabicGroup minOccurs = 0, maxOccurs = 1 _________ */
            ElisionSyllabicGroupPtr getElisionSyllabicGroup() const;
            void setElisionSyllabicGroup( const ElisionSyllabicGroupPtr& value );
            bool getHasElisionSyllabicGroup() const;
            void setHasElisionSyllabicGroup( const bool value );

            /* _________ Text minOccurs = 1, maxOccurs = 1 _________ */
            TextPtr getText() const;
            void setText( const TextPtr& value );
            
            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            ElisionSyllabicGroupPtr myElisionSyllabicGroup;
            bool myHasElisionSyllabicGroup;
            TextPtr myText;
        };
    }
}
