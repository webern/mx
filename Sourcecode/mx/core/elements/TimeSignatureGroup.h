// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

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

        MX_FORWARD_DECLARE_ELEMENT( BeatType )
        MX_FORWARD_DECLARE_ELEMENT( Beats )
        MX_FORWARD_DECLARE_ELEMENT( Interchangeable )
        MX_FORWARD_DECLARE_ELEMENT( TimeSignatureGroup )

        inline TimeSignatureGroupPtr makeTimeSignatureGroup() { return std::make_shared<TimeSignatureGroup>(); }

        class TimeSignatureGroup : public ElementInterface
        {
        public:
            TimeSignatureGroup();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            BeatsPtr getBeats() const;
            void setBeats( const BeatsPtr& value );
            BeatTypePtr getBeatType() const;
            void setBeatType( const BeatTypePtr& value );
            InterchangeablePtr getInterchangeable() const;
            void setInterchangeable( const InterchangeablePtr& value );
            bool getHasInterchangeable() const;
            void setHasInterchangeable( const bool value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            BeatsPtr myBeats;
            BeatTypePtr myBeatType;
            InterchangeablePtr myInterchangeable;
            bool myHasInterchangeable;
        };
    }
}
