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

        MX_FORWARD_DECLARE_ELEMENT( NormalNotes )

        inline NormalNotesPtr makeNormalNotes() { return std::make_shared<NormalNotes>(); }
		inline NormalNotesPtr makeNormalNotes( const NonNegativeInteger& value ) { return std::make_shared<NormalNotes>( value ); }
		inline NormalNotesPtr makeNormalNotes( NonNegativeInteger&& value ) { return std::make_shared<NormalNotes>( std::move( value ) ); }

        class NormalNotes : public ElementInterface
        {
        public:
            NormalNotes();
            NormalNotes( const NonNegativeInteger& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            NonNegativeInteger getValue() const;
            void setValue( const NonNegativeInteger& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            NonNegativeInteger myValue;
        };
    }
}
