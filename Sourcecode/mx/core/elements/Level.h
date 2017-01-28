// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Strings.h"
#include "mx/core/elements/LevelAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( LevelAttributes )
        MX_FORWARD_DECLARE_ELEMENT( Level )

        inline LevelPtr makeLevel() { return std::make_shared<Level>(); }
		inline LevelPtr makeLevel( const XsString& value ) { return std::make_shared<Level>( value ); }
		inline LevelPtr makeLevel( XsString&& value ) { return std::make_shared<Level>( std::move( value ) ); }

        class Level : public ElementInterface
        {
        public:
            Level();
            Level( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            LevelAttributesPtr getAttributes() const;
            void setAttributes( const LevelAttributesPtr& attributes );
            XsString getValue() const;
            void setValue( const XsString& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            XsString myValue;
            LevelAttributesPtr myAttributes;
        };
    }
}
