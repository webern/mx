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

        MX_FORWARD_DECLARE_ELEMENT( BeatUnit )
        MX_FORWARD_DECLARE_ELEMENT( BeatUnitDot )
        MX_FORWARD_DECLARE_ELEMENT( BeatUnitGroup )

        inline BeatUnitGroupPtr makeBeatUnitGroup() { return std::make_shared<BeatUnitGroup>(); }

        class BeatUnitGroup : public ElementInterface
        {
        public:
            BeatUnitGroup();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ BeatUnit minOccurs = 1, maxOccurs = 1 _________ */
            BeatUnitPtr getBeatUnit() const;
            void setBeatUnit( const BeatUnitPtr& value );

            /* _________ BeatUnitDot minOccurs = 0, maxOccurs = unbounded _________ */
            const BeatUnitDotSet& getBeatUnitDotSet() const;
            void addBeatUnitDot( const BeatUnitDotPtr& value );
            void removeBeatUnitDot( const BeatUnitDotSetIterConst& setIterator );
            void clearBeatUnitDotSet();
            
            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );

        private:
            BeatUnitPtr myBeatUnit;
            BeatUnitDotSet myBeatUnitDotSet;
        };
    }
}
