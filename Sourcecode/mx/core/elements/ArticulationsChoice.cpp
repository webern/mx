// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/ArticulationsChoice.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/Accent.h"
#include "mx/core/elements/BreathMark.h"
#include "mx/core/elements/Caesura.h"
#include "mx/core/elements/DetachedLegato.h"
#include "mx/core/elements/Doit.h"
#include "mx/core/elements/Falloff.h"
#include "mx/core/elements/OtherArticulation.h"
#include "mx/core/elements/Plop.h"
#include "mx/core/elements/Scoop.h"
#include "mx/core/elements/Spiccato.h"
#include "mx/core/elements/Staccatissimo.h"
#include "mx/core/elements/Staccato.h"
#include "mx/core/elements/Stress.h"
#include "mx/core/elements/StrongAccent.h"
#include "mx/core/elements/Tenuto.h"
#include "mx/core/elements/Unstress.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        ArticulationsChoice::ArticulationsChoice()
        :myChoice( Choice::accent )
        ,myAccent( makeAccent() )
        ,myStrongAccent( makeStrongAccent() )
        ,myStaccato( makeStaccato() )
        ,myTenuto( makeTenuto() )
        ,myDetachedLegato( makeDetachedLegato() )
        ,myStaccatissimo( makeStaccatissimo() )
        ,mySpiccato( makeSpiccato() )
        ,myScoop( makeScoop() )
        ,myPlop( makePlop() )
        ,myDoit( makeDoit() )
        ,myFalloff( makeFalloff() )
        ,myBreathMark( makeBreathMark() )
        ,myCaesura( makeCaesura() )
        ,myStress( makeStress() )
        ,myUnstress( makeUnstress() )
        ,myOtherArticulation( makeOtherArticulation() )
        {}


        bool ArticulationsChoice::hasAttributes() const
        {
            return false;
        }


        std::ostream& ArticulationsChoice::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& ArticulationsChoice::streamName( std::ostream& os ) const
        {
            os << "articulations";
            return os;
        }


        bool ArticulationsChoice::hasContents() const
        {
            return true;
        }


        std::ostream& ArticulationsChoice::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( isOneLineOnly );
            
            switch ( myChoice )
            {
                case Choice::accent:
                {
                    myAccent->toStream( os, indentLevel );
                }
                    break;
                case Choice::strongAccent:
                {
                    myStrongAccent->toStream( os, indentLevel );
                }
                    break;
                case Choice::staccato:
                {
                    myStaccato->toStream( os, indentLevel );
                }
                    break;
                case Choice::tenuto:
                {
                    myTenuto->toStream( os, indentLevel );
                }
                    break;
                case Choice::detachedLegato:
                {
                    myDetachedLegato->toStream( os, indentLevel );
                }
                    break;
                case Choice::staccatissimo:
                {
                    myStaccatissimo->toStream( os, indentLevel );
                }
                    break;
                case Choice::spiccato:
                {
                    mySpiccato->toStream( os, indentLevel );
                }
                    break;
                case Choice::scoop:
                {
                    myScoop->toStream( os, indentLevel );
                }
                    break;
                case Choice::plop:
                {
                    myPlop->toStream( os, indentLevel );
                }
                    break;
                case Choice::doit:
                {
                    myDoit->toStream( os, indentLevel );
                }
                    break;
                case Choice::falloff:
                {
                    myFalloff->toStream( os, indentLevel );
                }
                    break;
                case Choice::breathMark:
                {
                    myBreathMark->toStream( os, indentLevel );
                }
                    break;
                case Choice::caesura:
                {
                    myCaesura->toStream( os, indentLevel );
                }
                    break;
                case Choice::stress:
                {
                    myStress->toStream( os, indentLevel );
                }
                    break;
                case Choice::unstress:
                {
                    myUnstress->toStream( os, indentLevel );
                }
                    break;
                case Choice::otherArticulation:
                {
                    myOtherArticulation->toStream( os, indentLevel );
                }
                    break;
                default:
                    break;
            }
            return os;
        }


        ArticulationsChoice::Choice ArticulationsChoice::getChoice() const
        {
            return myChoice;
        }


        void ArticulationsChoice::setChoice( const ArticulationsChoice::Choice value )
        {
            myChoice = value;
        }


        AccentPtr ArticulationsChoice::getAccent() const
        {
            return myAccent;
        }


        void ArticulationsChoice::setAccent( const AccentPtr& value )
        {
            if( value )
            {
                myAccent = value;
            }
        }


        StrongAccentPtr ArticulationsChoice::getStrongAccent() const
        {
            return myStrongAccent;
        }


        void ArticulationsChoice::setStrongAccent( const StrongAccentPtr& value )
        {
            if( value )
            {
                myStrongAccent = value;
            }
        }


        StaccatoPtr ArticulationsChoice::getStaccato() const
        {
            return myStaccato;
        }


        void ArticulationsChoice::setStaccato( const StaccatoPtr& value )
        {
            if( value )
            {
                myStaccato = value;
            }
        }


        TenutoPtr ArticulationsChoice::getTenuto() const
        {
            return myTenuto;
        }


        void ArticulationsChoice::setTenuto( const TenutoPtr& value )
        {
            if( value )
            {
                myTenuto = value;
            }
        }


        DetachedLegatoPtr ArticulationsChoice::getDetachedLegato() const
        {
            return myDetachedLegato;
        }


        void ArticulationsChoice::setDetachedLegato( const DetachedLegatoPtr& value )
        {
            if( value )
            {
                myDetachedLegato = value;
            }
        }


        StaccatissimoPtr ArticulationsChoice::getStaccatissimo() const
        {
            return myStaccatissimo;
        }


        void ArticulationsChoice::setStaccatissimo( const StaccatissimoPtr& value )
        {
            if( value )
            {
                myStaccatissimo = value;
            }
        }


        SpiccatoPtr ArticulationsChoice::getSpiccato() const
        {
            return mySpiccato;
        }


        void ArticulationsChoice::setSpiccato( const SpiccatoPtr& value )
        {
            if( value )
            {
                mySpiccato = value;
            }
        }


        ScoopPtr ArticulationsChoice::getScoop() const
        {
            return myScoop;
        }


        void ArticulationsChoice::setScoop( const ScoopPtr& value )
        {
            if( value )
            {
                myScoop = value;
            }
        }


        PlopPtr ArticulationsChoice::getPlop() const
        {
            return myPlop;
        }


        void ArticulationsChoice::setPlop( const PlopPtr& value )
        {
            if( value )
            {
                myPlop = value;
            }
        }


        DoitPtr ArticulationsChoice::getDoit() const
        {
            return myDoit;
        }


        void ArticulationsChoice::setDoit( const DoitPtr& value )
        {
            if( value )
            {
                myDoit = value;
            }
        }


        FalloffPtr ArticulationsChoice::getFalloff() const
        {
            return myFalloff;
        }


        void ArticulationsChoice::setFalloff( const FalloffPtr& value )
        {
            if( value )
            {
                myFalloff = value;
            }
        }


        BreathMarkPtr ArticulationsChoice::getBreathMark() const
        {
            return myBreathMark;
        }


        void ArticulationsChoice::setBreathMark( const BreathMarkPtr& value )
        {
            if( value )
            {
                myBreathMark = value;
            }
        }


        CaesuraPtr ArticulationsChoice::getCaesura() const
        {
            return myCaesura;
        }


        void ArticulationsChoice::setCaesura( const CaesuraPtr& value )
        {
            if( value )
            {
                myCaesura = value;
            }
        }


        StressPtr ArticulationsChoice::getStress() const
        {
            return myStress;
        }


        void ArticulationsChoice::setStress( const StressPtr& value )
        {
            if( value )
            {
                myStress = value;
            }
        }


        UnstressPtr ArticulationsChoice::getUnstress() const
        {
            return myUnstress;
        }


        void ArticulationsChoice::setUnstress( const UnstressPtr& value )
        {
            if( value )
            {
                myUnstress = value;
            }
        }


        OtherArticulationPtr ArticulationsChoice::getOtherArticulation() const
        {
            return myOtherArticulation;
        }


        void ArticulationsChoice::setOtherArticulation( const OtherArticulationPtr& value )
        {
            if( value )
            {
                myOtherArticulation = value;
            }
        }


        bool ArticulationsChoice::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            if( xelement.getName() == "accent" )
            {
                myChoice = Choice::accent;
                return getAccent()->fromXElement( message, xelement );
            }
            
            if( xelement.getName() == "strong-accent" )
            {
                myChoice = Choice::strongAccent;
                return getStrongAccent()->fromXElement( message, xelement );
            }
            
            if( xelement.getName() == "staccato" )
            {
                myChoice = Choice::staccato;
                return getStaccato()->fromXElement( message, xelement );
            }
            
            if( xelement.getName() == "tenuto" )
            {
                myChoice = Choice::tenuto;
                return getTenuto()->fromXElement( message, xelement );
            }
            
            if( xelement.getName() == "detached-legato" )
            {
                myChoice = Choice::detachedLegato;
                return getDetachedLegato()->fromXElement( message, xelement );
            }
            
            if( xelement.getName() == "staccatissimo" )
            {
                myChoice = Choice::staccatissimo;
                return getStaccatissimo()->fromXElement( message, xelement );
            }
            
            if( xelement.getName() == "spiccato" )
            {
                myChoice = Choice::spiccato;
                return getSpiccato()->fromXElement( message, xelement );
            }
            
            if( xelement.getName() == "scoop" )
            {
                myChoice = Choice::scoop;
                return getScoop()->fromXElement( message, xelement );
            }
            
            if( xelement.getName() == "plop" )
            {
                myChoice = Choice::plop;
                return getPlop()->fromXElement( message, xelement );
            }
            
            if( xelement.getName() == "doit" )
            {
                myChoice = Choice::doit;
                return getDoit()->fromXElement( message, xelement );
            }
            
            if( xelement.getName() == "falloff" )
            {
                myChoice = Choice::falloff;
                return getFalloff()->fromXElement( message, xelement );
            }
            
            if( xelement.getName() == "breath-mark" )
            {
                myChoice = Choice::breathMark;
                return getBreathMark()->fromXElement( message, xelement );
            }
            
            if( xelement.getName() == "caesura" )
            {
                myChoice = Choice::caesura;
                return getCaesura()->fromXElement( message, xelement );
            }
            
            if( xelement.getName() == "stress" )
            {
                myChoice = Choice::stress;
                return getStress()->fromXElement( message, xelement );
            }
            
            if( xelement.getName() == "unstress" )
            {
                myChoice = Choice::unstress;
                return getUnstress()->fromXElement( message, xelement );
            }
            
            if( xelement.getName() == "other-articulation" )
            {
                myChoice = Choice::otherArticulation;
                return getOtherArticulation()->fromXElement( message, xelement );
            }
            
            message << "ArticulationsChoice: '" << xelement.getName() << "' is not allowed" << std::endl;
            return false;
        }

    }
}
