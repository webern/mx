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

        MX_FORWARD_DECLARE_ELEMENT( FrameStrings )

        inline FrameStringsPtr makeFrameStrings() { return std::make_shared<FrameStrings>(); }
		inline FrameStringsPtr makeFrameStrings( const PositiveInteger& value ) { return std::make_shared<FrameStrings>( value ); }
		inline FrameStringsPtr makeFrameStrings( PositiveInteger&& value ) { return std::make_shared<FrameStrings>( std::move( value ) ); }

        class FrameStrings : public ElementInterface
        {
        public:
            FrameStrings();
            FrameStrings( const PositiveInteger& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PositiveInteger getValue() const;
            void setValue( const PositiveInteger& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            PositiveInteger myValue;
        };
    }
}
