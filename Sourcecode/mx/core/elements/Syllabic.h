// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Enums.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ELEMENT( Syllabic )

        inline SyllabicPtr makeSyllabic() { return std::make_shared<Syllabic>(); }
		inline SyllabicPtr makeSyllabic( const SyllabicEnum& value ) { return std::make_shared<Syllabic>( value ); }
		inline SyllabicPtr makeSyllabic( SyllabicEnum&& value ) { return std::make_shared<Syllabic>( std::move( value ) ); }

        class Syllabic : public ElementInterface
        {
        public:
            Syllabic();
            Syllabic( const SyllabicEnum& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            SyllabicEnum getValue() const;
            void setValue( const SyllabicEnum& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            SyllabicEnum myValue;
        };
    }
}
