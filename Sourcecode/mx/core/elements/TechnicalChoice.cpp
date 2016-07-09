// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/TechnicalChoice.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/Arrow.h"
#include "mx/core/elements/Bend.h"
#include "mx/core/elements/DoubleTongue.h"
#include "mx/core/elements/DownBow.h"
#include "mx/core/elements/Fingering.h"
#include "mx/core/elements/Fingernails.h"
#include "mx/core/elements/Fret.h"
#include "mx/core/elements/HammerOn.h"
#include "mx/core/elements/Handbell.h"
#include "mx/core/elements/Harmonic.h"
#include "mx/core/elements/Heel.h"
#include "mx/core/elements/Hole.h"
#include "mx/core/elements/OpenString.h"
#include "mx/core/elements/OtherTechnical.h"
#include "mx/core/elements/Pluck.h"
#include "mx/core/elements/PullOff.h"
#include "mx/core/elements/SnapPizzicato.h"
#include "mx/core/elements/Stopped.h"
#include "mx/core/elements/String.h"
#include "mx/core/elements/Tap.h"
#include "mx/core/elements/ThumbPosition.h"
#include "mx/core/elements/Toe.h"
#include "mx/core/elements/TripleTongue.h"
#include "mx/core/elements/UpBow.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        TechnicalChoice::TechnicalChoice()
        :myChoice( Choice::upBow )
        ,myUpBow( makeUpBow() )
        ,myDownBow( makeDownBow() )
        ,myHarmonic( makeHarmonic() )
        ,myOpenString( makeOpenString() )
        ,myThumbPosition( makeThumbPosition() )
        ,myFingering( makeFingering() )
        ,myPluck( makePluck() )
        ,myDoubleTongue( makeDoubleTongue() )
        ,myTripleTongue( makeTripleTongue() )
        ,myStopped( makeStopped() )
        ,mySnapPizzicato( makeSnapPizzicato() )
        ,myFret( makeFret() )
        ,myString( makeString() )
        ,myHammerOn( makeHammerOn() )
        ,myPullOff( makePullOff() )
        ,myBend( makeBend() )
        ,myTap( makeTap() )
        ,myHeel( makeHeel() )
        ,myToe( makeToe() )
        ,myFingernails( makeFingernails() )
        ,myHole( makeHole() )
        ,myArrow( makeArrow() )
        ,myHandbell( makeHandbell() )
        ,myOtherTechnical( makeOtherTechnical() )
        {}


        bool TechnicalChoice::hasAttributes() const
        {
            return false;
        }


        std::ostream& TechnicalChoice::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& TechnicalChoice::streamName( std::ostream& os ) const
        {
            os << "technical";
            return os;
        }


        bool TechnicalChoice::hasContents() const
        {
            return true;
        }


        std::ostream& TechnicalChoice::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            switch ( myChoice )
            {
                case Choice::upBow:
                {
                    myUpBow->toStream( os, indentLevel );
                }
                    break;
                case Choice::downBow:
                {
                    myDownBow->toStream( os, indentLevel );
                }
                    break;
                case Choice::harmonic:
                {
                    myHarmonic->toStream( os, indentLevel );
                }
                    break;
                case Choice::openString:
                {
                    myOpenString->toStream( os, indentLevel );
                }
                    break;
                case Choice::thumbPosition:
                {
                    myThumbPosition->toStream( os, indentLevel );
                }
                    break;
                case Choice::fingering:
                {
                    myFingering->toStream( os, indentLevel );
                }
                    break;
                case Choice::pluck:
                {
                    myPluck->toStream( os, indentLevel );
                }
                    break;
                case Choice::doubleTongue:
                {
                    myDoubleTongue->toStream( os, indentLevel );
                }
                    break;
                case Choice::tripleTongue:
                {
                    myTripleTongue->toStream( os, indentLevel );
                }
                    break;
                case Choice::stopped:
                {
                    myStopped->toStream( os, indentLevel );
                }
                    break;
                case Choice::snapPizzicato:
                {
                    mySnapPizzicato->toStream( os, indentLevel );
                }
                    break;
                case Choice::fret:
                {
                    myFret->toStream( os, indentLevel );
                }
                    break;
                case Choice::string_:
                {
                    myString->toStream( os, indentLevel );
                }
                    break;
                case Choice::hammerOn:
                {
                    myHammerOn->toStream( os, indentLevel );
                }
                    break;
                case Choice::pullOff:
                {
                    myPullOff->toStream( os, indentLevel );
                }
                    break;
                case Choice::bend:
                {
                    myBend->toStream( os, indentLevel );
                }
                    break;
                case Choice::tap:
                {
                    myTap->toStream( os, indentLevel );
                }
                    break;
                case Choice::heel:
                {
                    myHeel->toStream( os, indentLevel );
                }
                    break;
                case Choice::toe:
                {
                    myToe->toStream( os, indentLevel );
                }
                    break;
                case Choice::fingernails:
                {
                    myFingernails->toStream( os, indentLevel );
                }
                    break;
                case Choice::hole:
                {
                    myHole->toStream( os, indentLevel );
                }
                    break;
                case Choice::arrow:
                {
                    myArrow->toStream( os, indentLevel );
                }
                    break;
                case Choice::handbell:
                {
                    myHandbell->toStream( os, indentLevel );
                }
                    break;
                case Choice::otherTechnical:
                {
                    myOtherTechnical->toStream( os, indentLevel );
                }
                    break;
                default:
                    break;
            }
            isOneLineOnly = false;
            return os;
        }


        TechnicalChoice::Choice TechnicalChoice::getChoice() const
        {
            return myChoice;
        }


        void TechnicalChoice::setChoice( const TechnicalChoice::Choice value )
        {
            myChoice = value;
        }


        UpBowPtr TechnicalChoice::getUpBow() const
        {
            return myUpBow;
        }


        void TechnicalChoice::setUpBow( const UpBowPtr& value )
        {
            if( value )
            {
                myUpBow = value;
            }
        }


        DownBowPtr TechnicalChoice::getDownBow() const
        {
            return myDownBow;
        }


        void TechnicalChoice::setDownBow( const DownBowPtr& value )
        {
            if( value )
            {
                myDownBow = value;
            }
        }


        HarmonicPtr TechnicalChoice::getHarmonic() const
        {
            return myHarmonic;
        }


        void TechnicalChoice::setHarmonic( const HarmonicPtr& value )
        {
            if( value )
            {
                myHarmonic = value;
            }
        }


        OpenStringPtr TechnicalChoice::getOpenString() const
        {
            return myOpenString;
        }


        void TechnicalChoice::setOpenString( const OpenStringPtr& value )
        {
            if( value )
            {
                myOpenString = value;
            }
        }


        ThumbPositionPtr TechnicalChoice::getThumbPosition() const
        {
            return myThumbPosition;
        }


        void TechnicalChoice::setThumbPosition( const ThumbPositionPtr& value )
        {
            if( value )
            {
                myThumbPosition = value;
            }
        }


        FingeringPtr TechnicalChoice::getFingering() const
        {
            return myFingering;
        }


        void TechnicalChoice::setFingering( const FingeringPtr& value )
        {
            if( value )
            {
                myFingering = value;
            }
        }


        PluckPtr TechnicalChoice::getPluck() const
        {
            return myPluck;
        }


        void TechnicalChoice::setPluck( const PluckPtr& value )
        {
            if( value )
            {
                myPluck = value;
            }
        }


        DoubleTonguePtr TechnicalChoice::getDoubleTongue() const
        {
            return myDoubleTongue;
        }


        void TechnicalChoice::setDoubleTongue( const DoubleTonguePtr& value )
        {
            if( value )
            {
                myDoubleTongue = value;
            }
        }


        TripleTonguePtr TechnicalChoice::getTripleTongue() const
        {
            return myTripleTongue;
        }


        void TechnicalChoice::setTripleTongue( const TripleTonguePtr& value )
        {
            if( value )
            {
                myTripleTongue = value;
            }
        }


        StoppedPtr TechnicalChoice::getStopped() const
        {
            return myStopped;
        }


        void TechnicalChoice::setStopped( const StoppedPtr& value )
        {
            if( value )
            {
                myStopped = value;
            }
        }


        SnapPizzicatoPtr TechnicalChoice::getSnapPizzicato() const
        {
            return mySnapPizzicato;
        }


        void TechnicalChoice::setSnapPizzicato( const SnapPizzicatoPtr& value )
        {
            if( value )
            {
                mySnapPizzicato = value;
            }
        }


        FretPtr TechnicalChoice::getFret() const
        {
            return myFret;
        }


        void TechnicalChoice::setFret( const FretPtr& value )
        {
            if( value )
            {
                myFret = value;
            }
        }


        StringPtr TechnicalChoice::getString() const
        {
            return myString;
        }


        void TechnicalChoice::setString( const StringPtr& value )
        {
            if( value )
            {
                myString = value;
            }
        }


        HammerOnPtr TechnicalChoice::getHammerOn() const
        {
            return myHammerOn;
        }


        void TechnicalChoice::setHammerOn( const HammerOnPtr& value )
        {
            if( value )
            {
                myHammerOn = value;
            }
        }


        PullOffPtr TechnicalChoice::getPullOff() const
        {
            return myPullOff;
        }


        void TechnicalChoice::setPullOff( const PullOffPtr& value )
        {
            if( value )
            {
                myPullOff = value;
            }
        }


        BendPtr TechnicalChoice::getBend() const
        {
            return myBend;
        }


        void TechnicalChoice::setBend( const BendPtr& value )
        {
            if( value )
            {
                myBend = value;
            }
        }


        TapPtr TechnicalChoice::getTap() const
        {
            return myTap;
        }


        void TechnicalChoice::setTap( const TapPtr& value )
        {
            if( value )
            {
                myTap = value;
            }
        }


        HeelPtr TechnicalChoice::getHeel() const
        {
            return myHeel;
        }


        void TechnicalChoice::setHeel( const HeelPtr& value )
        {
            if( value )
            {
                myHeel = value;
            }
        }


        ToePtr TechnicalChoice::getToe() const
        {
            return myToe;
        }


        void TechnicalChoice::setToe( const ToePtr& value )
        {
            if( value )
            {
                myToe = value;
            }
        }


        FingernailsPtr TechnicalChoice::getFingernails() const
        {
            return myFingernails;
        }


        void TechnicalChoice::setFingernails( const FingernailsPtr& value )
        {
            if( value )
            {
                myFingernails = value;
            }
        }


        HolePtr TechnicalChoice::getHole() const
        {
            return myHole;
        }


        void TechnicalChoice::setHole( const HolePtr& value )
        {
            if( value )
            {
                myHole = value;
            }
        }


        ArrowPtr TechnicalChoice::getArrow() const
        {
            return myArrow;
        }


        void TechnicalChoice::setArrow( const ArrowPtr& value )
        {
            if( value )
            {
                myArrow = value;
            }
        }


        HandbellPtr TechnicalChoice::getHandbell() const
        {
            return myHandbell;
        }


        void TechnicalChoice::setHandbell( const HandbellPtr& value )
        {
            if( value )
            {
                myHandbell = value;
            }
        }


        OtherTechnicalPtr TechnicalChoice::getOtherTechnical() const
        {
            return myOtherTechnical;
        }


        void TechnicalChoice::setOtherTechnical( const OtherTechnicalPtr& value )
        {
            if( value )
            {
                myOtherTechnical = value;
            }
        }

        MX_FROM_XELEMENT_UNUSED( TechnicalChoice );
    }
}
