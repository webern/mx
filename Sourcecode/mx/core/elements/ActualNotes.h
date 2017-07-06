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

        MX_FORWARD_DECLARE_ELEMENT( ActualNotes )

        inline ActualNotesPtr makeActualNotes() { return std::make_shared<ActualNotes>(); }
		inline ActualNotesPtr makeActualNotes( const NonNegativeInteger& value ) { return std::make_shared<ActualNotes>( value ); }
		inline ActualNotesPtr makeActualNotes( NonNegativeInteger&& value ) { return std::make_shared<ActualNotes>( std::move( value ) ); }

        class ActualNotes : public ElementInterface
        {
        public:
            ActualNotes();
            ActualNotes( const NonNegativeInteger& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            NonNegativeInteger getValue() const;
            void setValue( const NonNegativeInteger& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );

        private:
            NonNegativeInteger myValue;
        };
    }
}
