// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Strings.h"
#include "mx/core/elements/GroupAbbreviationAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( GroupAbbreviationAttributes )
        MX_FORWARD_DECLARE_ELEMENT( GroupAbbreviation )

        inline GroupAbbreviationPtr makeGroupAbbreviation() { return std::make_shared<GroupAbbreviation>(); }
		inline GroupAbbreviationPtr makeGroupAbbreviation( const XsString& value ) { return std::make_shared<GroupAbbreviation>( value ); }
		inline GroupAbbreviationPtr makeGroupAbbreviation( XsString&& value ) { return std::make_shared<GroupAbbreviation>( std::move( value ) ); }

        class GroupAbbreviation : public ElementInterface
        {
        public:
            GroupAbbreviation();
            GroupAbbreviation( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            GroupAbbreviationAttributesPtr getAttributes() const;
            void setAttributes( const GroupAbbreviationAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            XsString myValue;
            GroupAbbreviationAttributesPtr myAttributes;
        };
    }
}
