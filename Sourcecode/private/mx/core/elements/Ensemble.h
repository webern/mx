// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/PositiveIntegerOrEmpty.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ELEMENT( Ensemble )

        inline EnsemblePtr makeEnsemble() { return std::make_shared<Ensemble>(); }
		inline EnsemblePtr makeEnsemble( const PositiveIntegerOrEmpty& value ) { return std::make_shared<Ensemble>( value ); }
		inline EnsemblePtr makeEnsemble( PositiveIntegerOrEmpty&& value ) { return std::make_shared<Ensemble>( std::move( value ) ); }

        class Ensemble : public ElementInterface
        {
        public:
            Ensemble();
            Ensemble( const PositiveIntegerOrEmpty& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PositiveIntegerOrEmpty getValue() const;
            void setValue( const PositiveIntegerOrEmpty& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement );

        private:
            PositiveIntegerOrEmpty myValue;
        };
    }
}
