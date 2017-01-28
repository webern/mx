// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/TimeAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( TimeAttributes )
        MX_FORWARD_DECLARE_ELEMENT( TimeChoice )
        MX_FORWARD_DECLARE_ELEMENT( Time )

        inline TimePtr makeTime() { return std::make_shared<Time>(); }

        class Time : public ElementInterface
        {
        public:
            Time();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            TimeAttributesPtr getAttributes() const;
            void setAttributes( const TimeAttributesPtr& value );
            TimeChoicePtr getTimeChoice() const;
            void setTimeChoice( const TimeChoicePtr& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            TimeAttributesPtr myAttributes;
            TimeChoicePtr myChoice;
        };
    }
}
