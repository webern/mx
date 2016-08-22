// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/ArrowAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( ArrowAttributes )
        MX_FORWARD_DECLARE_ELEMENT( ArrowGroup )
        MX_FORWARD_DECLARE_ELEMENT( CircularArrow )
        MX_FORWARD_DECLARE_ELEMENT( Arrow )

        inline ArrowPtr makeArrow() { return std::make_shared<Arrow>(); }

        class Arrow : public ElementInterface
        {
        public:
            enum class Choice
            {
                arrowGroup = 1,
                circularArrow = 2
            };
            Arrow();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            ArrowAttributesPtr getAttributes() const;
            void setAttributes( const ArrowAttributesPtr& value );

            /* _________ Choice _________ */
            Arrow::Choice getChoice() const;
            void setChoice( const Arrow::Choice value );

            /* _________ ArrowGroup minOccurs = 1, maxOccurs = 1 _________ */
            ArrowGroupPtr getArrowGroup() const;
            void setArrowGroup( const ArrowGroupPtr& value );

            /* _________ CircularArrow minOccurs = 1, maxOccurs = 1 _________ */
            CircularArrowPtr getCircularArrow() const;
            void setCircularArrow( const CircularArrowPtr& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            Choice myChoice;
            ArrowAttributesPtr myAttributes;
            ArrowGroupPtr myArrowGroup;
            CircularArrowPtr myCircularArrow;
        };
    }
}
