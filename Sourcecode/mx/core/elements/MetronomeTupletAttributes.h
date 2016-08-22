// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/AttributesInterface.h"
#include "mx/core/Enums.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( MetronomeTupletAttributes )

        struct MetronomeTupletAttributes : public AttributesInterface
        {
        public:
            MetronomeTupletAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            StartStop type;
            YesNo bracket;
            ShowTuplet showNumber;
            const 	bool hasType;
            bool hasBracket;
            bool hasShowNumber;

            bool fromXElement( std::ostream& message, xml::XElement& xelement );
        };
    }
}
