// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/HoleAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( HoleAttributes )
        MX_FORWARD_DECLARE_ELEMENT( HoleClosed )
        MX_FORWARD_DECLARE_ELEMENT( HoleShape )
        MX_FORWARD_DECLARE_ELEMENT( HoleType )
        MX_FORWARD_DECLARE_ELEMENT( Hole )

        inline HolePtr makeHole() { return std::make_shared<Hole>(); }

        class Hole : public ElementInterface
        {
        public:
            Hole();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            HoleAttributesPtr getAttributes() const;
            void setAttributes( const HoleAttributesPtr& value );

            /* _________ HoleType minOccurs = 0, maxOccurs = 1 _________ */
            HoleTypePtr getHoleType() const;
            void setHoleType( const HoleTypePtr& value );
            bool getHasHoleType() const;
            void setHasHoleType( const bool value );

            /* _________ HoleClosed minOccurs = 1, maxOccurs = 1 _________ */
            HoleClosedPtr getHoleClosed() const;
            void setHoleClosed( const HoleClosedPtr& value );

            /* _________ HoleShape minOccurs = 0, maxOccurs = 1 _________ */
            HoleShapePtr getHoleShape() const;
            void setHoleShape( const HoleShapePtr& value );
            bool getHasHoleShape() const;
            void setHasHoleShape( const bool value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );

        private:
            HoleAttributesPtr myAttributes;
            HoleTypePtr myHoleType;
            bool myHasHoleType;
            HoleClosedPtr myHoleClosed;
            HoleShapePtr myHoleShape;
            bool myHasHoleShape;
        };
    }
}
