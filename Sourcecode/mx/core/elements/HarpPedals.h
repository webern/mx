// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/HarpPedalsAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( HarpPedalsAttributes )
        MX_FORWARD_DECLARE_ELEMENT( PedalTuning )
        MX_FORWARD_DECLARE_ELEMENT( HarpPedals )

        inline HarpPedalsPtr makeHarpPedals() { return std::make_shared<HarpPedals>(); }

        class HarpPedals : public ElementInterface
        {
        public:
            HarpPedals();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            HarpPedalsAttributesPtr getAttributes() const;
            void setAttributes( const HarpPedalsAttributesPtr& value );

            /* _________ PedalTuning minOccurs = 1, maxOccurs = unbounded _________ */
            const PedalTuningSet& getPedalTuningSet() const;
            void addPedalTuning( const PedalTuningPtr& value );
            void removePedalTuning( const PedalTuningSetIterConst& value );
            void clearPedalTuningSet();
            PedalTuningPtr getPedalTuning( const PedalTuningSetIterConst& setIterator ) const;

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            HarpPedalsAttributesPtr myAttributes;
            PedalTuningSet myPedalTuningSet;
        };
    }
}
