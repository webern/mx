// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/XsString.h"
#include "mx/core/elements/OtherPlayAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( OtherPlayAttributes )
        MX_FORWARD_DECLARE_ELEMENT( OtherPlay )

        inline OtherPlayPtr makeOtherPlay() { return std::make_shared<OtherPlay>(); }
		inline OtherPlayPtr makeOtherPlay( const XsString& value ) { return std::make_shared<OtherPlay>( value ); }
		inline OtherPlayPtr makeOtherPlay( XsString&& value ) { return std::make_shared<OtherPlay>( std::move( value ) ); }

        class OtherPlay : public ElementInterface
        {
        public:
            OtherPlay();
            OtherPlay( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            OtherPlayAttributesPtr getAttributes() const;
            void setAttributes( const OtherPlayAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement );

        private:
            XsString myValue;
            OtherPlayAttributesPtr myAttributes;
        };
    }
}
