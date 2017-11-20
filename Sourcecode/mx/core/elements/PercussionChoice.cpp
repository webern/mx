// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/PercussionChoice.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/Beater.h"
#include "mx/core/elements/Effect.h"
#include "mx/core/elements/Glass.h"
#include "mx/core/elements/Membrane.h"
#include "mx/core/elements/Metal.h"
#include "mx/core/elements/OtherPercussion.h"
#include "mx/core/elements/Pitched.h"
#include "mx/core/elements/Stick.h"
#include "mx/core/elements/StickLocation.h"
#include "mx/core/elements/StickMaterial.h"
#include "mx/core/elements/StickType.h"
#include "mx/core/elements/Timpani.h"
#include "mx/core/elements/Wood.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        PercussionChoice::PercussionChoice()


        :myChoice( PercussionChoice::Choice::glass )
        ,myGlass( makeGlass() )
        ,myMetal( makeMetal() )
        ,myWood( makeWood() )
        ,myPitched( makePitched() )
        ,myMembrane( makeMembrane() )
        ,myEffect( makeEffect() )
        ,myTimpani( makeTimpani() )
        ,myBeater( makeBeater() )
        ,myStick( makeStick() )
        ,myStickType( makeStickType() )
        ,myStickMaterial( makeStickMaterial() )
        ,myStickLocation( makeStickLocation() )
        ,myOtherPercussion( makeOtherPercussion() )
        {}


        bool PercussionChoice::hasAttributes() const
        {
            return false;
        }


        std::ostream& PercussionChoice::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& PercussionChoice::streamName( std::ostream& os ) const
        {
            return os;
        }


        bool PercussionChoice::hasContents() const
        {
            return true;
        }


        std::ostream& PercussionChoice::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            switch ( myChoice )
            {
                case Choice::glass:
                {
                    myGlass->toStream( os, indentLevel );
                }
                    break;
                case Choice::metal:
                {
                    myMetal->toStream( os, indentLevel );
                }
                    break;
                case Choice::wood:
                {
                    myWood->toStream( os, indentLevel );
                }
                    break;
                case Choice::pitched:
                {
                    myPitched->toStream( os, indentLevel );
                }
                    break;
                case Choice::membrane:
                {
                    myMembrane->toStream( os, indentLevel );
                }
                    break;
                case Choice::effect:
                {
                    myEffect->toStream( os, indentLevel );
                }
                    break;
                case Choice::timpani:
                {
                    myTimpani->toStream( os, indentLevel );
                }
                    break;
                case Choice::beater:
                {
                    myBeater->toStream( os, indentLevel );
                }
                    break;
                case Choice::stick:
                {
                    myStick->toStream( os, indentLevel );
                }
                    break;
                case Choice::stickType:
                {
                    myStickType->toStream( os, indentLevel );
                }
                    break;
                case Choice::stickMaterial:
                {
                    myStickMaterial->toStream( os, indentLevel );
                }
                    break;
                case Choice::stickLocation:
                {
                    myStickLocation->toStream( os, indentLevel );
                }
                    break;
                case Choice::otherPercussion:
                {
                    myOtherPercussion->toStream( os, indentLevel );
                }
                    break;
                default:
                    break;
            }
            isOneLineOnly = false;
            return os;
        }


        PercussionChoice::Choice PercussionChoice::getChoice() const
        {
            return myChoice;
        }


        void PercussionChoice::setChoice( const PercussionChoice::Choice value )
        {
            myChoice = value;
        }


        GlassPtr PercussionChoice::getGlass() const
        {
            return myGlass;
        }


        void PercussionChoice::setGlass( const GlassPtr& value )
        {
            if( value )
            {
                myGlass = value;
            }
        }


        MetalPtr PercussionChoice::getMetal() const
        {
            return myMetal;
        }


        void PercussionChoice::setMetal( const MetalPtr& value )
        {
            if( value )
            {
                myMetal = value;
            }
        }


        WoodPtr PercussionChoice::getWood() const
        {
            return myWood;
        }


        void PercussionChoice::setWood( const WoodPtr& value )
        {
            if( value )
            {
                myWood = value;
            }
        }


        PitchedPtr PercussionChoice::getPitched() const
        {
            return myPitched;
        }


        void PercussionChoice::setPitched( const PitchedPtr& value )
        {
            if( value )
            {
                myPitched = value;
            }
        }


        MembranePtr PercussionChoice::getMembrane() const
        {
            return myMembrane;
        }


        void PercussionChoice::setMembrane( const MembranePtr& value )
        {
            if( value )
            {
                myMembrane = value;
            }
        }


        EffectPtr PercussionChoice::getEffect() const
        {
            return myEffect;
        }


        void PercussionChoice::setEffect( const EffectPtr& value )
        {
            if( value )
            {
                myEffect = value;
            }
        }


        TimpaniPtr PercussionChoice::getTimpani() const
        {
            return myTimpani;
        }


        void PercussionChoice::setTimpani( const TimpaniPtr& value )
        {
            if( value )
            {
                myTimpani = value;
            }
        }


        BeaterPtr PercussionChoice::getBeater() const
        {
            return myBeater;
        }


        void PercussionChoice::setBeater( const BeaterPtr& value )
        {
            if( value )
            {
                myBeater = value;
            }
        }


        StickPtr PercussionChoice::getStick() const
        {
            return myStick;
        }


        void PercussionChoice::setStick( const StickPtr& value )
        {
            if( value )
            {
                myStick = value;
            }
        }


        StickTypePtr PercussionChoice::getStickType() const
        {
            return myStickType;
        }


        void PercussionChoice::setStickType( const StickTypePtr& value )
        {
            if( value )
            {
                myStickType = value;
            }
        }


        StickMaterialPtr PercussionChoice::getStickMaterial() const
        {
            return myStickMaterial;
        }


        void PercussionChoice::setStickMaterial( const StickMaterialPtr& value )
        {
            if( value )
            {
                myStickMaterial = value;
            }
        }


        StickLocationPtr PercussionChoice::getStickLocation() const
        {
            return myStickLocation;
        }


        void PercussionChoice::setStickLocation( const StickLocationPtr& value )
        {
            if( value )
            {
                myStickLocation = value;
            }
        }


        OtherPercussionPtr PercussionChoice::getOtherPercussion() const
        {
            return myOtherPercussion;
        }


        void PercussionChoice::setOtherPercussion( const OtherPercussionPtr& value )
        {
            if( value )
            {
                myOtherPercussion = value;
            }
        }


        bool PercussionChoice::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            if( xelement.getName() == "glass" )
            {
                myChoice = Choice::glass;
                return getGlass()->fromXElement( message, xelement );
            }
            
            if( xelement.getName() == "metal" )
            {
                myChoice = Choice::metal;
                return getMetal()->fromXElement( message, xelement );
            }
            
            if( xelement.getName() == "wood" )
            {
                myChoice = Choice::wood;
                return getWood()->fromXElement( message, xelement );
            }
            
            if( xelement.getName() == "pitched" )
            {
                myChoice = Choice::pitched;
                return getPitched()->fromXElement( message, xelement );
            }
            
            if( xelement.getName() == "membrane" )
            {
                myChoice = Choice::membrane;
                return getMembrane()->fromXElement( message, xelement );
            }
            
            if( xelement.getName() == "effect" )
            {
                myChoice = Choice::effect;
                return getEffect()->fromXElement( message, xelement );
            }
            
            if( xelement.getName() == "timpani" )
            {
                myChoice = Choice::timpani;
                return getTimpani()->fromXElement( message, xelement );
            }
            
            if( xelement.getName() == "beater" )
            {
                myChoice = Choice::beater;
                return getBeater()->fromXElement( message, xelement );
            }
            
            if( xelement.getName() == "stick" )
            {
                myChoice = Choice::stick;
                return getStick()->fromXElement( message, xelement );
            }
            
            if( xelement.getName() == "stick-type" )
            {
                myChoice = Choice::stickType;
                return getStickType()->fromXElement( message, xelement );
            }
            
            if( xelement.getName() == "stick-material" )
            {
                myChoice = Choice::stickMaterial;
                return getStickMaterial()->fromXElement( message, xelement );
            }
            
            if( xelement.getName() == "stick-location" )
            {
                myChoice = Choice::stickLocation;
                return getStickLocation()->fromXElement( message, xelement );
            }
            
            if( xelement.getName() == "other-percussion" )
            {
                myChoice = Choice::otherPercussion;
                return getOtherPercussion()->fromXElement( message, xelement );
            }
            
            MX_BAD_ELEMENT_FAILURE( PercussionChoice );
        }

    }
}
