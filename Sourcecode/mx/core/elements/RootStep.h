// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/Enums.h"
#include "mx/core/elements/RootStepAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( RootStepAttributes )
        MX_FORWARD_DECLARE_ELEMENT( RootStep )

        inline RootStepPtr makeRootStep() { return std::make_shared<RootStep>(); }
		inline RootStepPtr makeRootStep( const StepEnum& value ) { return std::make_shared<RootStep>( value ); }
		inline RootStepPtr makeRootStep( StepEnum&& value ) { return std::make_shared<RootStep>( std::move( value ) ); }

        class RootStep : public ElementInterface
        {
        public:
            RootStep();
            RootStep( const StepEnum& value );

            virtual bool hasAttributes() const;
            virtual bool hasContents() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            RootStepAttributesPtr getAttributes() const;
            void setAttributes( const RootStepAttributesPtr& attributes );
            StepEnum getValue() const;
            void setValue( const StepEnum& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            StepEnum myValue;
            RootStepAttributesPtr myAttributes;
        };
    }
}
