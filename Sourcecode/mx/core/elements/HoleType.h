// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Strings.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ELEMENT( HoleType )

        inline HoleTypePtr makeHoleType() { return std::make_shared<HoleType>(); }
		inline HoleTypePtr makeHoleType( const XsString& value ) { return std::make_shared<HoleType>( value ); }
		inline HoleTypePtr makeHoleType( XsString&& value ) { return std::make_shared<HoleType>( std::move( value ) ); }

        class HoleType : public ElementInterface
        {
        public:
            HoleType();
            HoleType( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            XsString getValue() const;
            void setValue( const XsString& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            XsString myValue;
        };
    }
}
