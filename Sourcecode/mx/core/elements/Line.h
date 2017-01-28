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

        MX_FORWARD_DECLARE_ELEMENT( Line )

        inline LinePtr makeLine() { return std::make_shared<Line>(); }
		inline LinePtr makeLine( const StaffLine& value ) { return std::make_shared<Line>( value ); }
		inline LinePtr makeLine( StaffLine&& value ) { return std::make_shared<Line>( std::move( value ) ); }

        class Line : public ElementInterface
        {
        public:
            Line();
            Line( const StaffLine& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            StaffLine getValue() const;
            void setValue( const StaffLine& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            StaffLine myValue;
        };
    }
}
