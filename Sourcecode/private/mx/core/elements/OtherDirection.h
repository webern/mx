// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Strings.h"
#include "mx/core/elements/OtherDirectionAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( OtherDirectionAttributes )
        MX_FORWARD_DECLARE_ELEMENT( OtherDirection )

        inline OtherDirectionPtr makeOtherDirection() { return std::make_shared<OtherDirection>(); }
		inline OtherDirectionPtr makeOtherDirection( const XsString& value ) { return std::make_shared<OtherDirection>( value ); }
		inline OtherDirectionPtr makeOtherDirection( XsString&& value ) { return std::make_shared<OtherDirection>( std::move( value ) ); }

        class OtherDirection : public ElementInterface
        {
        public:
            OtherDirection();
            OtherDirection( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            OtherDirectionAttributesPtr getAttributes() const;
            void setAttributes( const OtherDirectionAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement );

        private:
            XsString myValue;
            OtherDirectionAttributesPtr myAttributes;
        };
    }
}
