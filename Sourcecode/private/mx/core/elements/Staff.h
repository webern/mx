// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

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

        MX_FORWARD_DECLARE_ELEMENT( Staff )

        inline StaffPtr makeStaff() { return std::make_shared<Staff>(); }
		inline StaffPtr makeStaff( const PositiveInteger& value ) { return std::make_shared<Staff>( value ); }
		inline StaffPtr makeStaff( PositiveInteger&& value ) { return std::make_shared<Staff>( std::move( value ) ); }

        class Staff : public ElementInterface
        {
        public:
            Staff();
            Staff( const PositiveInteger& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PositiveInteger getValue() const;
            void setValue( const PositiveInteger& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );

        private:
            PositiveInteger myValue;
        };
    }
}
