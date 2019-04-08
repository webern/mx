// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

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

        MX_FORWARD_DECLARE_ELEMENT( DisplayStep )

        inline DisplayStepPtr makeDisplayStep() { return std::make_shared<DisplayStep>(); }
		inline DisplayStepPtr makeDisplayStep( const StepEnum& value ) { return std::make_shared<DisplayStep>( value ); }
		inline DisplayStepPtr makeDisplayStep( StepEnum&& value ) { return std::make_shared<DisplayStep>( std::move( value ) ); }

        class DisplayStep : public ElementInterface
        {
        public:
            DisplayStep();
            DisplayStep( const StepEnum& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            StepEnum getValue() const;
            void setValue( const StepEnum& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );

        private:
            StepEnum myValue;
        };
    }
}
