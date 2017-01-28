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

        MX_FORWARD_DECLARE_ELEMENT( BasePitch )
        MX_FORWARD_DECLARE_ELEMENT( SoundingPitch )
        MX_FORWARD_DECLARE_ELEMENT( TouchingPitch )
        MX_FORWARD_DECLARE_ELEMENT( HarmonicInfoChoice )

        inline HarmonicInfoChoicePtr makeHarmonicInfoChoice() { return std::make_shared<HarmonicInfoChoice>(); }

        class HarmonicInfoChoice : public ElementInterface
        {
        public:
            enum class Choice
            {
                basePitch = 1,
                touchingPitch = 2,
                soundingPitch = 3
            };
            HarmonicInfoChoice();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Choice minOccurs = 1, maxOccurs = 1 _________ */
            HarmonicInfoChoice::Choice getChoice() const;
            void setChoice( const HarmonicInfoChoice::Choice value );

            /* _________ BasePitch minOccurs = 1, maxOccurs = 1 _________ */
            BasePitchPtr getBasePitch() const;
            void setBasePitch( const BasePitchPtr& value );

            /* _________ TouchingPitch minOccurs = 1, maxOccurs = 1 _________ */
            TouchingPitchPtr getTouchingPitch() const;
            void setTouchingPitch( const TouchingPitchPtr& value );

            /* _________ SoundingPitch minOccurs = 1, maxOccurs = 1 _________ */
            SoundingPitchPtr getSoundingPitch() const;
            void setSoundingPitch( const SoundingPitchPtr& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );
            
        private:
            Choice myChoice;
            BasePitchPtr myBasePitch;
            TouchingPitchPtr myTouchingPitch;
            SoundingPitchPtr mySoundingPitch;
        };
    }
}
