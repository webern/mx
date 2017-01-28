// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Strings.h"
#include "mx/core/elements/SuffixAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( SuffixAttributes )
        MX_FORWARD_DECLARE_ELEMENT( Suffix )

        inline SuffixPtr makeSuffix() { return std::make_shared<Suffix>(); }
		inline SuffixPtr makeSuffix( const XsString& value ) { return std::make_shared<Suffix>( value ); }
		inline SuffixPtr makeSuffix( XsString&& value ) { return std::make_shared<Suffix>( std::move( value ) ); }

        class Suffix : public ElementInterface
        {
        public:
            Suffix();
            Suffix( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            SuffixAttributesPtr getAttributes() const;
            void setAttributes( const SuffixAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            XsString myValue;
            SuffixAttributesPtr myAttributes;
        };
    }
}
