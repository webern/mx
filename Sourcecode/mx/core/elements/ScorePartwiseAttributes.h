// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/AttributesInterface.h"
#include "mx/core/Strings.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( ScorePartwiseAttributes )

        struct ScorePartwiseAttributes : public AttributesInterface
        {
        public:
            ScorePartwiseAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            XsToken version;
            bool hasVersion;

            bool fromXElement( std::ostream& message, xml::XElement& xelement );
        };
    }
}
