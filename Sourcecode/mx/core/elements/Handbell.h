// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Enums.h"
#include "mx/core/elements/HandbellAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( HandbellAttributes )
        MX_FORWARD_DECLARE_ELEMENT( Handbell )

        inline HandbellPtr makeHandbell() { return std::make_shared<Handbell>(); }
		inline HandbellPtr makeHandbell( const HandbellValue& value ) { return std::make_shared<Handbell>( value ); }
		inline HandbellPtr makeHandbell( HandbellValue&& value ) { return std::make_shared<Handbell>( std::move( value ) ); }

        class Handbell : public ElementInterface
        {
        public:
            Handbell();
            Handbell( const HandbellValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            HandbellAttributesPtr getAttributes() const;
            void setAttributes( const HandbellAttributesPtr& attributes );
            HandbellValue getValue() const;
            void setValue( const HandbellValue& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );

        private:
            HandbellValue myValue;
            HandbellAttributesPtr myAttributes;
        };
    }
}
