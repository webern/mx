// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Strings.h"
#include "mx/core/elements/RelationAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( RelationAttributes )
        MX_FORWARD_DECLARE_ELEMENT( Relation )

        inline RelationPtr makeRelation() { return std::make_shared<Relation>(); }
		inline RelationPtr makeRelation( const XsString& value ) { return std::make_shared<Relation>( value ); }
		inline RelationPtr makeRelation( XsString&& value ) { return std::make_shared<Relation>( std::move( value ) ); }

        class Relation : public ElementInterface
        {
        public:
            Relation();
            Relation( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            RelationAttributesPtr getAttributes() const;
            void setAttributes( const RelationAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );

        private:
            XsString myValue;
            RelationAttributesPtr myAttributes;
        };
    }
}
