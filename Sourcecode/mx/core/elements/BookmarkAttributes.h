// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/AttributesInterface.h"
#include "mx/core/Integers.h"
#include "mx/core/Strings.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( BookmarkAttributes )

        struct BookmarkAttributes : public AttributesInterface
        {
        public:
            BookmarkAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            XsID id;
            XsToken name;
            XsNMToken element;
            PositiveInteger position;
            const   bool hasId;
            bool hasName;
            bool hasElement;
            bool hasPosition;

            bool fromXElement( std::ostream& message, xml::XElement& xelement );
        };
    }
}
