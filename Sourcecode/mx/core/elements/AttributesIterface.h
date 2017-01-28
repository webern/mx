// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/AttributesInterface.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( AttributesIterface )

        struct AttributesIterface : public AttributesInterface
        {
        public:
            AttributesIterface();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
        };
    }
}
