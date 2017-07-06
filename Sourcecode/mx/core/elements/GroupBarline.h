// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Enums.h"
#include "mx/core/elements/GroupBarlineAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( GroupBarlineAttributes )
        MX_FORWARD_DECLARE_ELEMENT( GroupBarline )

        inline GroupBarlinePtr makeGroupBarline() { return std::make_shared<GroupBarline>(); }
		inline GroupBarlinePtr makeGroupBarline( const GroupBarlineValue& value ) { return std::make_shared<GroupBarline>( value ); }
		inline GroupBarlinePtr makeGroupBarline( GroupBarlineValue&& value ) { return std::make_shared<GroupBarline>( std::move( value ) ); }

        class GroupBarline : public ElementInterface
        {
        public:
            GroupBarline();
            GroupBarline( const GroupBarlineValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            GroupBarlineAttributesPtr getAttributes() const;
            void setAttributes( const GroupBarlineAttributesPtr& attributes );
            GroupBarlineValue getValue() const;
            void setValue( const GroupBarlineValue& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );

        private:
            GroupBarlineValue myValue;
            GroupBarlineAttributesPtr myAttributes;
        };
    }
}
