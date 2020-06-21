// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/EmptyPrintObjectStyleAlignAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ELEMENT( Eyeglasses )

        inline EyeglassesPtr makeEyeglasses() { return std::make_shared<Eyeglasses>(); }

        class Eyeglasses : public ElementInterface
        {
        public:
            Eyeglasses();

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            EmptyPrintObjectStyleAlignAttributesPtr getAttributes() const;
            void setAttributes( const EmptyPrintObjectStyleAlignAttributesPtr& attributes );

            private:
            virtual bool fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement );

        private:
            EmptyPrintObjectStyleAlignAttributesPtr myAttributes;
        };
    }
}
