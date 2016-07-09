// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Strings.h"
#include "mx/core/elements/PartAbbreviationAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( PartAbbreviationAttributes )
        MX_FORWARD_DECLARE_ELEMENT( PartAbbreviation )

        inline PartAbbreviationPtr makePartAbbreviation() { return std::make_shared<PartAbbreviation>(); }
		inline PartAbbreviationPtr makePartAbbreviation( const XsString& value ) { return std::make_shared<PartAbbreviation>( value ); }
		inline PartAbbreviationPtr makePartAbbreviation( XsString&& value ) { return std::make_shared<PartAbbreviation>( std::move( value ) ); }

        class PartAbbreviation : public ElementInterface
        {
        public:
            PartAbbreviation();
            PartAbbreviation( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PartAbbreviationAttributesPtr getAttributes() const;
            void setAttributes( const PartAbbreviationAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            XsString myValue;
            PartAbbreviationAttributesPtr myAttributes;
        };
    }
}
