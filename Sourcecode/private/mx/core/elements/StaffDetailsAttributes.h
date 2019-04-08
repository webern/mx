// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/AttributesInterface.h"
#include "mx/core/Enums.h"
#include "mx/core/Integers.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( StaffDetailsAttributes )

        struct StaffDetailsAttributes : public AttributesInterface
        {
        public:
            StaffDetailsAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            StaffNumber number;
            ShowFrets showFrets;
            YesNo printObject;
            YesNo printSpacing;
            bool hasNumber;
            bool hasShowFrets;
            bool hasPrintObject;
            bool hasPrintSpacing;

            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );
        };
    }
}
