// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Integers.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ELEMENT( AccordionMiddle )

        inline AccordionMiddlePtr makeAccordionMiddle() { return std::make_shared<AccordionMiddle>(); }
		inline AccordionMiddlePtr makeAccordionMiddle( const AccordionMiddleValue& value ) { return std::make_shared<AccordionMiddle>( value ); }
		inline AccordionMiddlePtr makeAccordionMiddle( AccordionMiddleValue&& value ) { return std::make_shared<AccordionMiddle>( std::move( value ) ); }

        class AccordionMiddle : public ElementInterface
        {
        public:
            AccordionMiddle();
            AccordionMiddle( const AccordionMiddleValue& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            AccordionMiddleValue getValue() const;
            void setValue( const AccordionMiddleValue& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            AccordionMiddleValue myValue;
        };
    }
}
