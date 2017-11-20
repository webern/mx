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

        MX_FORWARD_DECLARE_ELEMENT( BeatUnitGroup )
        MX_FORWARD_DECLARE_ELEMENT( PerMinuteOrBeatUnitChoice )
        MX_FORWARD_DECLARE_ELEMENT( BeatUnitPer )

        inline BeatUnitPerPtr makeBeatUnitPer() { return std::make_shared<BeatUnitPer>(); }

        class BeatUnitPer : public ElementInterface
        {
        public:
            BeatUnitPer();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            BeatUnitGroupPtr getBeatUnitGroup() const;
            void setBeatUnitGroup( const BeatUnitGroupPtr& value );
            PerMinuteOrBeatUnitChoicePtr getPerMinuteOrBeatUnitChoice() const;
            void setPerMinuteOtBeatUnitChoice( const PerMinuteOrBeatUnitChoicePtr& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );

        private:
            BeatUnitGroupPtr myBeatUnitGroup;
            PerMinuteOrBeatUnitChoicePtr myPerMinuteOrBeatUnitChoice;
        };
    }
}
