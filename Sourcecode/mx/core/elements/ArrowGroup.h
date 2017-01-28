// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ELEMENT( ArrowDirection )
        MX_FORWARD_DECLARE_ELEMENT( ArrowStyle )
        MX_FORWARD_DECLARE_ELEMENT( ArrowGroup )

        inline ArrowGroupPtr makeArrowGroup() { return std::make_shared<ArrowGroup>(); }

        class ArrowGroup : public ElementInterface
        {
        public:
            ArrowGroup();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ ArrowDirection minOccurs = 1, maxOccurs = 1 _________ */
            ArrowDirectionPtr getArrowDirection() const;
            void setArrowDirection( const ArrowDirectionPtr& value );

            /* _________ ArrowStyle minOccurs = 0, maxOccurs = 1 _________ */
            ArrowStylePtr getArrowStyle() const;
            void setArrowStyle( const ArrowStylePtr& value );
            bool getHasArrowStyle() const;
            void setHasArrowStyle( const bool value );
            
            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            ArrowDirectionPtr myArrowDirection;
            ArrowStylePtr myArrowStyle;
            bool myHasArrowStyle;
        };
    }
}
