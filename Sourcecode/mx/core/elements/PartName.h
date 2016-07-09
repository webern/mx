// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Strings.h"
#include "mx/core/elements/PartNameAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( PartNameAttributes )
        MX_FORWARD_DECLARE_ELEMENT( PartName )

        inline PartNamePtr makePartName() { return std::make_shared<PartName>(); }
		inline PartNamePtr makePartName( const XsString& value ) { return std::make_shared<PartName>( value ); }
		inline PartNamePtr makePartName( XsString&& value ) { return std::make_shared<PartName>( std::move( value ) ); }

        class PartName : public ElementInterface
        {
        public:
            PartName();
            PartName( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PartNameAttributesPtr getAttributes() const;
            void setAttributes( const PartNameAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            XsString myValue;
            PartNameAttributesPtr myAttributes;
        };
    }
}
