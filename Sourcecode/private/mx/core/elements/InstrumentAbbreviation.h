// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/XsString.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ELEMENT( InstrumentAbbreviation )

        inline InstrumentAbbreviationPtr makeInstrumentAbbreviation() { return std::make_shared<InstrumentAbbreviation>(); }
		inline InstrumentAbbreviationPtr makeInstrumentAbbreviation( const XsString& value ) { return std::make_shared<InstrumentAbbreviation>( value ); }
		inline InstrumentAbbreviationPtr makeInstrumentAbbreviation( XsString&& value ) { return std::make_shared<InstrumentAbbreviation>( std::move( value ) ); }

        class InstrumentAbbreviation : public ElementInterface
        {
        public:
            InstrumentAbbreviation();
            InstrumentAbbreviation( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            XsString getValue() const;
            void setValue( const XsString& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement );

        private:
            XsString myValue;
        };
    }
}
