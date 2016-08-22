// MusicXML Class Library v0.3.0
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

        MX_FORWARD_DECLARE_ELEMENT( Beater )
        MX_FORWARD_DECLARE_ELEMENT( Effect )
        MX_FORWARD_DECLARE_ELEMENT( Glass )
        MX_FORWARD_DECLARE_ELEMENT( Membrane )
        MX_FORWARD_DECLARE_ELEMENT( Metal )
        MX_FORWARD_DECLARE_ELEMENT( OtherPercussion )
        MX_FORWARD_DECLARE_ELEMENT( Pitched )
        MX_FORWARD_DECLARE_ELEMENT( Stick )
        MX_FORWARD_DECLARE_ELEMENT( StickLocation )
        MX_FORWARD_DECLARE_ELEMENT( StickMaterial )
        MX_FORWARD_DECLARE_ELEMENT( StickType )
        MX_FORWARD_DECLARE_ELEMENT( Timpani )
        MX_FORWARD_DECLARE_ELEMENT( Wood )
        MX_FORWARD_DECLARE_ELEMENT( PercussionChoice )

        inline PercussionChoicePtr makePercussionChoice() { return std::make_shared<PercussionChoice>(); }

        class PercussionChoice : public ElementInterface
        {
        public:
            enum class Choice
            {
                glass = 1,
                metal = 2,
                wood = 3,
                pitched = 4,
                membrane = 5,
                effect = 6,
                timpani = 7,
                beater = 8,
                stick = 9,
                stickType = 10,
                stickMaterial = 11,
                stickLocation = 12,
                otherPercussion = 13
            };
            PercussionChoice();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            PercussionChoice::Choice getChoice() const;
            void setChoice( const PercussionChoice::Choice value );

            /* _________ Glass minOccurs = 1, maxOccurs = 1 _________ */
            GlassPtr getGlass() const;
            void setGlass( const GlassPtr& value );

            /* _________ Metal minOccurs = 1, maxOccurs = 1 _________ */
            MetalPtr getMetal() const;
            void setMetal( const MetalPtr& value );

            /* _________ Wood minOccurs = 1, maxOccurs = 1 _________ */
            WoodPtr getWood() const;
            void setWood( const WoodPtr& value );

            /* _________ Pitched minOccurs = 1, maxOccurs = 1 _________ */
            PitchedPtr getPitched() const;
            void setPitched( const PitchedPtr& value );

            /* _________ Membrane minOccurs = 1, maxOccurs = 1 _________ */
            MembranePtr getMembrane() const;
            void setMembrane( const MembranePtr& value );

            /* _________ Effect minOccurs = 1, maxOccurs = 1 _________ */
            EffectPtr getEffect() const;
            void setEffect( const EffectPtr& value );

            /* _________ Timpani minOccurs = 1, maxOccurs = 1 _________ */
            TimpaniPtr getTimpani() const;
            void setTimpani( const TimpaniPtr& value );

            /* _________ Beater minOccurs = 1, maxOccurs = 1 _________ */
            BeaterPtr getBeater() const;
            void setBeater( const BeaterPtr& value );

            /* _________ Stick minOccurs = 1, maxOccurs = 1 _________ */
            StickPtr getStick() const;
            void setStick( const StickPtr& value );

            /* _________ StickType minOccurs = 1, maxOccurs = 1 _________ */
            StickTypePtr getStickType() const;
            void setStickType( const StickTypePtr& value );

            /* _________ StickMaterial minOccurs = 1, maxOccurs = 1 _________ */
            StickMaterialPtr getStickMaterial() const;
            void setStickMaterial( const StickMaterialPtr& value );

            /* _________ StickLocation minOccurs = 1, maxOccurs = 1 _________ */
            StickLocationPtr getStickLocation() const;
            void setStickLocation( const StickLocationPtr& value );

            /* _________ OtherPercussion minOccurs = 1, maxOccurs = 1 _________ */
            OtherPercussionPtr getOtherPercussion() const;
            void setOtherPercussion( const OtherPercussionPtr& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            Choice myChoice;
            GlassPtr myGlass;
            MetalPtr myMetal;
            WoodPtr myWood;
            PitchedPtr myPitched;
            MembranePtr myMembrane;
            EffectPtr myEffect;
            TimpaniPtr myTimpani;
            BeaterPtr myBeater;
            StickPtr myStick;
            StickTypePtr myStickType;
            StickMaterialPtr myStickMaterial;
            StickLocationPtr myStickLocation;
            OtherPercussionPtr myOtherPercussion;
        };
    }
}
