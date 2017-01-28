// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Strings.h"
#include "mx/core/elements/FingeringAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( FingeringAttributes )
        MX_FORWARD_DECLARE_ELEMENT( Fingering )

        inline FingeringPtr makeFingering() { return std::make_shared<Fingering>(); }
		inline FingeringPtr makeFingering( const XsString& value ) { return std::make_shared<Fingering>( value ); }
		inline FingeringPtr makeFingering( XsString&& value ) { return std::make_shared<Fingering>( std::move( value ) ); }

        class Fingering : public ElementInterface
        {
        public:
            Fingering();
            Fingering( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            FingeringAttributesPtr getAttributes() const;
            void setAttributes( const FingeringAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            XsString myValue;
            FingeringAttributesPtr myAttributes;
        };
    }
}
