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
        MX_FORWARD_DECLARE_ELEMENT( PerMinute )
        MX_FORWARD_DECLARE_ELEMENT( PerMinuteOrBeatUnitChoice )

        inline PerMinuteOrBeatUnitChoicePtr makePerMinuteOrBeatUnitChoice() { return std::make_shared<PerMinuteOrBeatUnitChoice>(); }

        class PerMinuteOrBeatUnitChoice : public ElementInterface
        {
        public:
            enum class Choice
            {
                perMinute = 0,
                beatUnitGroup = 1
            };
            PerMinuteOrBeatUnitChoice();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PerMinuteOrBeatUnitChoice::Choice getChoice() const;
            void setChoice( const PerMinuteOrBeatUnitChoice::Choice value );
            PerMinutePtr getPerMinute() const;
            void setPerMinute( const PerMinutePtr& value );
            BeatUnitGroupPtr getBeatUnitGroup() const;
            void setBeatUnitGroup( const BeatUnitGroupPtr& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );

        private:
            Choice myChoice;
            PerMinutePtr myPerMinute;
            BeatUnitGroupPtr myBeatUnitGroup;
        };
    }
}
