// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ELEMENT( PedalAlter )
        MX_FORWARD_DECLARE_ELEMENT( PedalStep )
        MX_FORWARD_DECLARE_ELEMENT( PedalTuning )

        inline PedalTuningPtr makePedalTuning() { return std::make_shared<PedalTuning>(); }

        class PedalTuning : public ElementInterface
        {
        public:
            PedalTuning();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ PedalStep minOccurs = 1, maxOccurs = 1 _________ */
            PedalStepPtr getPedalStep() const;
            void setPedalStep( const PedalStepPtr& value );

            /* _________ PedalAlter minOccurs = 1, maxOccurs = 1 _________ */
            PedalAlterPtr getPedalAlter() const;
            void setPedalAlter( const PedalAlterPtr& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            PedalStepPtr myPedalStep;
            PedalAlterPtr myPedalAlter;
        };
    }
}
