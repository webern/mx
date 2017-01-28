// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/StaffLayoutAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( StaffLayoutAttributes )
        MX_FORWARD_DECLARE_ELEMENT( StaffDistance )
        MX_FORWARD_DECLARE_ELEMENT( StaffLayout )

        inline StaffLayoutPtr makeStaffLayout() { return std::make_shared<StaffLayout>(); }

        class StaffLayout : public ElementInterface
        {
        public:
            StaffLayout();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            StaffLayoutAttributesPtr getAttributes() const;
            void setAttributes( const StaffLayoutAttributesPtr& value );

            /* _________ StaffDistance minOccurs = 0, maxOccurs = 1 _________ */
            StaffDistancePtr getStaffDistance() const;
            void setStaffDistance( const StaffDistancePtr& value );
            bool getHasStaffDistance() const;
            void setHasStaffDistance( const bool value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            StaffLayoutAttributesPtr myAttributes;
            StaffDistancePtr myStaffDistance;
            bool myHasStaffDistance;
        };
    }
}
