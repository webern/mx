// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/AttributesInterface.h"
#include "mx/core/Decimals.h"
#include "mx/core/Enums.h"
#include "mx/core/Strings.h"
#include "mx/core/YesNoNumber.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( SoundAttributes )

        struct SoundAttributes : public AttributesInterface
        {
        public:
            SoundAttributes();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            NonNegativeDecimal tempo;
            NonNegativeDecimal dynamics;
            YesNo dacapo;
            XsToken segno;
            XsToken dalsegno;
            XsToken coda;
            XsToken tocoda;
            DivisionsValue divisions;
            YesNo forwardRepeat;
            XsToken fine;
            TimeOnly timeOnly;
            YesNo pizzicato;
            RotationDegrees pan;
            RotationDegrees elevation;
            YesNoNumber damperPedal;
            YesNoNumber softPedal;
            YesNoNumber sostenutoPedal;
            bool hasTempo;
            bool hasDynamics;
            bool hasDacapo;
            bool hasSegno;
            bool hasDalsegno;
            bool hasCoda;
            bool hasTocoda;
            bool hasDivisions;
            bool hasForwardRepeat;
            bool hasFine;
            bool hasTimeOnly;
            bool hasPizzicato;
            bool hasPan;
            bool hasElevation;
            bool hasDamperPedal;
            bool hasSoftPedal;
            bool hasSostenutoPedal;

            bool fromXElement( std::ostream& message, xml::XElement& xelement );
        };
    }
}
