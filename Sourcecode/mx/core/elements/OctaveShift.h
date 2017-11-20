// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/OctaveShiftAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( OctaveShiftAttributes )
        MX_FORWARD_DECLARE_ELEMENT( OctaveShift )

        inline OctaveShiftPtr makeOctaveShift() { return std::make_shared<OctaveShift>(); }

        class OctaveShift : public ElementInterface
        {
        public:
            OctaveShift();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            OctaveShiftAttributesPtr getAttributes() const;
            void setAttributes( const OctaveShiftAttributesPtr& attributes );

            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );

        private:
            OctaveShiftAttributesPtr myAttributes;
        };
    }
}
