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

        MX_FORWARD_DECLARE_ELEMENT( Diatonic )

        inline DiatonicPtr makeDiatonic() { return std::make_shared<Diatonic>(); }
		inline DiatonicPtr makeDiatonic( const Integer& value ) { return std::make_shared<Diatonic>( value ); }
		inline DiatonicPtr makeDiatonic( Integer&& value ) { return std::make_shared<Diatonic>( std::move( value ) ); }

        class Diatonic : public ElementInterface
        {
        public:
            Diatonic();
            Diatonic( const Integer& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            Integer getValue() const;
            void setValue( const Integer& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            Integer myValue;
        };
    }
}
