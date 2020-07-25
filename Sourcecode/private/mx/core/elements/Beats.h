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

        MX_FORWARD_DECLARE_ELEMENT( Beats )

        inline BeatsPtr makeBeats() { return std::make_shared<Beats>(); }
		inline BeatsPtr makeBeats( const XsString& value ) { return std::make_shared<Beats>( value ); }
		inline BeatsPtr makeBeats( XsString&& value ) { return std::make_shared<Beats>( std::move( value ) ); }

        class Beats : public ElementInterface
        {
        public:
            Beats();
            Beats( const XsString& value );

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
