// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Strings.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ELEMENT( Group )

        inline GroupPtr makeGroup() { return std::make_shared<Group>(); }
		inline GroupPtr makeGroup( const XsString& value ) { return std::make_shared<Group>( value ); }
		inline GroupPtr makeGroup( XsString&& value ) { return std::make_shared<Group>( std::move( value ) ); }

        class Group : public ElementInterface
        {
        public:
            Group();
            Group( const XsString& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            XsString getValue() const;
            void setValue( const XsString& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );

        private:
            XsString myValue;
        };
    }
}
