// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Enums.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ELEMENT( ArrowDirection )

        inline ArrowDirectionPtr makeArrowDirection() { return std::make_shared<ArrowDirection>(); }
		inline ArrowDirectionPtr makeArrowDirection( const ArrowDirectionEnum& value ) { return std::make_shared<ArrowDirection>( value ); }
		inline ArrowDirectionPtr makeArrowDirection( ArrowDirectionEnum&& value ) { return std::make_shared<ArrowDirection>( std::move( value ) ); }

        class ArrowDirection : public ElementInterface
        {
        public:
            ArrowDirection();
            ArrowDirection( const ArrowDirectionEnum& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            ArrowDirectionEnum getValue() const;
            void setValue( const ArrowDirectionEnum& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );

        private:
            ArrowDirectionEnum myValue;
        };
    }
}
