// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Decimals.h"
#include "mx/core/elements/RootAlterAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( RootAlterAttributes )
        MX_FORWARD_DECLARE_ELEMENT( RootAlter )

        inline RootAlterPtr makeRootAlter() { return std::make_shared<RootAlter>(); }
		inline RootAlterPtr makeRootAlter( const Semitones& value ) { return std::make_shared<RootAlter>( value ); }
		inline RootAlterPtr makeRootAlter( Semitones&& value ) { return std::make_shared<RootAlter>( std::move( value ) ); }

        class RootAlter : public ElementInterface
        {
        public:
            RootAlter();
            RootAlter( const Semitones& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            RootAlterAttributesPtr getAttributes() const;
            void setAttributes( const RootAlterAttributesPtr& attributes );
            Semitones getValue() const;
            void setValue( const Semitones& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            Semitones myValue;
            RootAlterAttributesPtr myAttributes;
        };
    }
}
