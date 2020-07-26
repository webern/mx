// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/XsString.h"
#include "mx/core/elements/PullOffAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( PullOffAttributes )
        MX_FORWARD_DECLARE_ELEMENT( PullOff )

        inline PullOffPtr makePullOff() { return std::make_shared<PullOff>(); }
		inline PullOffPtr makePullOff( const XsString& value ) { return std::make_shared<PullOff>( value ); }
		inline PullOffPtr makePullOff( XsString&& value ) { return std::make_shared<PullOff>( std::move( value ) ); }

        class PullOff : public ElementInterface
        {
        public:
            PullOff();
            PullOff( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PullOffAttributesPtr getAttributes() const;
            void setAttributes( const PullOffAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement );

        private:
            XsString myValue;
            PullOffAttributesPtr myAttributes;
        };
    }
}
