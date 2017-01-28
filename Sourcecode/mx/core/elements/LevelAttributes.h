// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

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

        MX_FORWARD_DECLARE_ATTRIBUTES( LevelAttributes )

        struct LevelAttributes : public AttributesInterface
        {
        public:
            LevelAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            YesNo reference;
            YesNo parentheses;
            YesNo bracket;
            SymbolSize size;
            bool hasReference;
            bool hasParentheses;
            bool hasBracket;
            bool hasSize;

            bool fromXElement( std::ostream& message, xml::XElement& xelement );
        };
    }
}
