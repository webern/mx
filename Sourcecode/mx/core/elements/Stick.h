// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/StickAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( StickAttributes )
        MX_FORWARD_DECLARE_ELEMENT( StickMaterial )
        MX_FORWARD_DECLARE_ELEMENT( StickType )
        MX_FORWARD_DECLARE_ELEMENT( Stick )

        inline StickPtr makeStick() { return std::make_shared<Stick>(); }

        class Stick : public ElementInterface
        {
        public:
            Stick();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            StickAttributesPtr getAttributes() const;
            void setAttributes( const StickAttributesPtr& value );

            /* _________ StickType minOccurs = 1, maxOccurs = 1 _________ */
            StickTypePtr getStickType() const;
            void setStickType( const StickTypePtr& value );

            /* _________ StickMaterial minOccurs = 1, maxOccurs = 1 _________ */
            StickMaterialPtr getStickMaterial() const;
            void setStickMaterial( const StickMaterialPtr& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            StickAttributesPtr myAttributes;
            StickTypePtr myStickType;
            StickMaterialPtr myStickMaterial;
        };
    }
}
