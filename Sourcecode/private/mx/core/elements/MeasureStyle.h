// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/MeasureStyleAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( MeasureStyleAttributes )
        MX_FORWARD_DECLARE_ELEMENT( MeasureStyleChoice )
        MX_FORWARD_DECLARE_ELEMENT( MeasureStyle )

        inline MeasureStylePtr makeMeasureStyle() { return std::make_shared<MeasureStyle>(); }

        class MeasureStyle : public ElementInterface
        {
        public:
            MeasureStyle();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            MeasureStyleAttributesPtr getAttributes() const;
            void setAttributes( const MeasureStyleAttributesPtr& value );
            MeasureStyleChoicePtr getMeasureStyleChoice() const;
            void setMeasureStyleChoice( const MeasureStyleChoicePtr& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );

        private:
            MeasureStyleAttributesPtr myAttributes;
            MeasureStyleChoicePtr myChoice;
        };
    }
}
