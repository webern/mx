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

        MX_FORWARD_DECLARE_ATTRIBUTES( ScoreInstrumentAttributes )

        struct ScoreInstrumentAttributes : public AttributesInterface
        {
        public:
            ScoreInstrumentAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            XsID id;
            const 	bool hasId;

            bool fromXElement( std::ostream& message, xml::XElement& xelement );
        };
    }
}
