// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Strings.h"
#include "mx/core/elements/CreditWordsAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( CreditWordsAttributes )
        MX_FORWARD_DECLARE_ELEMENT( CreditWords )

        inline CreditWordsPtr makeCreditWords() { return std::make_shared<CreditWords>(); }
		inline CreditWordsPtr makeCreditWords( const XsString& value ) { return std::make_shared<CreditWords>( value ); }
		inline CreditWordsPtr makeCreditWords( XsString&& value ) { return std::make_shared<CreditWords>( std::move( value ) ); }

        class CreditWords : public ElementInterface
        {
        public:
            CreditWords();
            CreditWords( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            CreditWordsAttributesPtr getAttributes() const;
            void setAttributes( const CreditWordsAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );

        private:
            XsString myValue;
            CreditWordsAttributesPtr myAttributes;
        };
    }
}
