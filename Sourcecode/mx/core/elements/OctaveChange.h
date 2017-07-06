// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Integers.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ELEMENT( OctaveChange )

        inline OctaveChangePtr makeOctaveChange() { return std::make_shared<OctaveChange>(); }
		inline OctaveChangePtr makeOctaveChange( const Integer& value ) { return std::make_shared<OctaveChange>( value ); }
		inline OctaveChangePtr makeOctaveChange( Integer&& value ) { return std::make_shared<OctaveChange>( std::move( value ) ); }

        class OctaveChange : public ElementInterface
        {
        public:
            OctaveChange();
            OctaveChange( const Integer& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            Integer getValue() const;
            void setValue( const Integer& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );

        private:
            Integer myValue;
        };
    }
}
