// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/PlayAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( PlayAttributes )
        MX_FORWARD_DECLARE_ELEMENT( Ipa )
        MX_FORWARD_DECLARE_ELEMENT( Mute )
        MX_FORWARD_DECLARE_ELEMENT( OtherPlay )
        MX_FORWARD_DECLARE_ELEMENT( SemiPitched )
        MX_FORWARD_DECLARE_ELEMENT( Play )

        inline PlayPtr makePlay() { return std::make_shared<Play>(); }

        class Play : public ElementInterface
        {
        public:
            enum class Choice
            {
                ipa = 1,
                mute = 2,
                semiPitched = 3,
                otherPlay = 4
            };
            Play();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PlayAttributesPtr getAttributes() const;
            void setAttributes( const PlayAttributesPtr& value );

            /* _________ Choice __________ */
            Play::Choice getChoice() const;
            void setChoice( const Play::Choice value );

            /* _________ Ipa minOccurs = 1, maxOccurs = 1 _________ */
            IpaPtr getIpa() const;
            void setIpa( const IpaPtr& value );

            /* _________ Mute minOccurs = 1, maxOccurs = 1 _________ */
            MutePtr getMute() const;
            void setMute( const MutePtr& value );

            /* _________ SemiPitched minOccurs = 1, maxOccurs = 1 _________ */
            SemiPitchedPtr getSemiPitched() const;
            void setSemiPitched( const SemiPitchedPtr& value );

            /* _________ OtherPlay minOccurs = 1, maxOccurs = 1 _________ */
            OtherPlayPtr getOtherPlay() const;
            void setOtherPlay( const OtherPlayPtr& value );
            

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            PlayAttributesPtr myAttributes;
            Choice myChoice;
            IpaPtr myIpa;
            MutePtr myMute;
            SemiPitchedPtr mySemiPitched;
            OtherPlayPtr myOtherPlay;
        };
    }
}
