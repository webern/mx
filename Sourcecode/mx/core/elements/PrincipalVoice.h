// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Strings.h"
#include "mx/core/elements/PrincipalVoiceAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( PrincipalVoiceAttributes )
        MX_FORWARD_DECLARE_ELEMENT( PrincipalVoice )

        inline PrincipalVoicePtr makePrincipalVoice() { return std::make_shared<PrincipalVoice>(); }
		inline PrincipalVoicePtr makePrincipalVoice( const XsString& value ) { return std::make_shared<PrincipalVoice>( value ); }
		inline PrincipalVoicePtr makePrincipalVoice( XsString&& value ) { return std::make_shared<PrincipalVoice>( std::move( value ) ); }

        class PrincipalVoice : public ElementInterface
        {
        public:
            PrincipalVoice();
            PrincipalVoice( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PrincipalVoiceAttributesPtr getAttributes() const;
            void setAttributes( const PrincipalVoiceAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            XsString myValue;
            PrincipalVoiceAttributesPtr myAttributes;
        };
    }
}
