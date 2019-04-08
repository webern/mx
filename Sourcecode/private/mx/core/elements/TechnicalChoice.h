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

        MX_FORWARD_DECLARE_ELEMENT( Arrow )
        MX_FORWARD_DECLARE_ELEMENT( Bend )
        MX_FORWARD_DECLARE_ELEMENT( DoubleTongue )
        MX_FORWARD_DECLARE_ELEMENT( DownBow )
        MX_FORWARD_DECLARE_ELEMENT( Fingering )
        MX_FORWARD_DECLARE_ELEMENT( Fingernails )
        MX_FORWARD_DECLARE_ELEMENT( Fret )
        MX_FORWARD_DECLARE_ELEMENT( HammerOn )
        MX_FORWARD_DECLARE_ELEMENT( Handbell )
        MX_FORWARD_DECLARE_ELEMENT( Harmonic )
        MX_FORWARD_DECLARE_ELEMENT( Heel )
        MX_FORWARD_DECLARE_ELEMENT( Hole )
        MX_FORWARD_DECLARE_ELEMENT( OpenString )
        MX_FORWARD_DECLARE_ELEMENT( OtherTechnical )
        MX_FORWARD_DECLARE_ELEMENT( Pluck )
        MX_FORWARD_DECLARE_ELEMENT( PullOff )
        MX_FORWARD_DECLARE_ELEMENT( SnapPizzicato )
        MX_FORWARD_DECLARE_ELEMENT( Stopped )
        MX_FORWARD_DECLARE_ELEMENT( String )
        MX_FORWARD_DECLARE_ELEMENT( Tap )
        MX_FORWARD_DECLARE_ELEMENT( ThumbPosition )
        MX_FORWARD_DECLARE_ELEMENT( Toe )
        MX_FORWARD_DECLARE_ELEMENT( TripleTongue )
        MX_FORWARD_DECLARE_ELEMENT( UpBow )
        MX_FORWARD_DECLARE_ELEMENT( TechnicalChoice )

        inline TechnicalChoicePtr makeTechnicalChoice() { return std::make_shared<TechnicalChoice>(); }

        class TechnicalChoice : public ElementInterface
        {
        public:
            enum class Choice
            {
                upBow = 1,
                downBow = 2,
                harmonic = 3,
                openString = 4,
                thumbPosition = 5,
                fingering = 6,
                pluck = 7,
                doubleTongue = 8,
                tripleTongue = 9,
                stopped = 10,
                snapPizzicato = 11,
                fret = 12,
                string_ = 13,
                hammerOn = 14,
                pullOff = 15,
                bend = 16,
                tap = 17,
                heel = 18,
                toe = 19,
                fingernails = 20,
                hole = 21,
                arrow = 22,
                handbell = 23,
                otherTechnical = 24
            };
            TechnicalChoice();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Choice minOccurs = 1, maxOccurs = 1 _________ */
            TechnicalChoice::Choice getChoice() const;
            void setChoice( const TechnicalChoice::Choice value );

            /* _________ UpBow minOccurs = 1, maxOccurs = 1 _________ */
            UpBowPtr getUpBow() const;
            void setUpBow( const UpBowPtr& value );

            /* _________ DownBow minOccurs = 1, maxOccurs = 1 _________ */
            DownBowPtr getDownBow() const;
            void setDownBow( const DownBowPtr& value );

            /* _________ Harmonic minOccurs = 1, maxOccurs = 1 _________ */
            HarmonicPtr getHarmonic() const;
            void setHarmonic( const HarmonicPtr& value );

            /* _________ OpenString minOccurs = 1, maxOccurs = 1 _________ */
            OpenStringPtr getOpenString() const;
            void setOpenString( const OpenStringPtr& value );

            /* _________ ThumbPosition minOccurs = 1, maxOccurs = 1 _________ */
            ThumbPositionPtr getThumbPosition() const;
            void setThumbPosition( const ThumbPositionPtr& value );

            /* _________ Fingering minOccurs = 1, maxOccurs = 1 _________ */
            FingeringPtr getFingering() const;
            void setFingering( const FingeringPtr& value );

            /* _________ Pluck minOccurs = 1, maxOccurs = 1 _________ */
            PluckPtr getPluck() const;
            void setPluck( const PluckPtr& value );

            /* _________ DoubleTongue minOccurs = 1, maxOccurs = 1 _________ */
            DoubleTonguePtr getDoubleTongue() const;
            void setDoubleTongue( const DoubleTonguePtr& value );

            /* _________ TripleTongue minOccurs = 1, maxOccurs = 1 _________ */
            TripleTonguePtr getTripleTongue() const;
            void setTripleTongue( const TripleTonguePtr& value );

            /* _________ Stopped minOccurs = 1, maxOccurs = 1 _________ */
            StoppedPtr getStopped() const;
            void setStopped( const StoppedPtr& value );

            /* _________ SnapPizzicato minOccurs = 1, maxOccurs = 1 _________ */
            SnapPizzicatoPtr getSnapPizzicato() const;
            void setSnapPizzicato( const SnapPizzicatoPtr& value );

            /* _________ Fret minOccurs = 1, maxOccurs = 1 _________ */
            FretPtr getFret() const;
            void setFret( const FretPtr& value );

            /* _________ String minOccurs = 1, maxOccurs = 1 _________ */
            StringPtr getString() const;
            void setString( const StringPtr& value );

            /* _________ HammerOn minOccurs = 1, maxOccurs = 1 _________ */
            HammerOnPtr getHammerOn() const;
            void setHammerOn( const HammerOnPtr& value );

            /* _________ PullOff minOccurs = 1, maxOccurs = 1 _________ */
            PullOffPtr getPullOff() const;
            void setPullOff( const PullOffPtr& value );

            /* _________ Bend minOccurs = 1, maxOccurs = 1 _________ */
            BendPtr getBend() const;
            void setBend( const BendPtr& value );

            /* _________ Tap minOccurs = 1, maxOccurs = 1 _________ */
            TapPtr getTap() const;
            void setTap( const TapPtr& value );

            /* _________ Heel minOccurs = 1, maxOccurs = 1 _________ */
            HeelPtr getHeel() const;
            void setHeel( const HeelPtr& value );

            /* _________ Toe minOccurs = 1, maxOccurs = 1 _________ */
            ToePtr getToe() const;
            void setToe( const ToePtr& value );

            /* _________ Fingernails minOccurs = 1, maxOccurs = 1 _________ */
            FingernailsPtr getFingernails() const;
            void setFingernails( const FingernailsPtr& value );

            /* _________ Hole minOccurs = 1, maxOccurs = 1 _________ */
            HolePtr getHole() const;
            void setHole( const HolePtr& value );

            /* _________ Arrow minOccurs = 1, maxOccurs = 1 _________ */
            ArrowPtr getArrow() const;
            void setArrow( const ArrowPtr& value );

            /* _________ Handbell minOccurs = 1, maxOccurs = 1 _________ */
            HandbellPtr getHandbell() const;
            void setHandbell( const HandbellPtr& value );

            /* _________ OtherTechnical minOccurs = 1, maxOccurs = 1 _________ */
            OtherTechnicalPtr getOtherTechnical() const;
            void setOtherTechnical( const OtherTechnicalPtr& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );
            
        private:
            Choice myChoice;
            UpBowPtr myUpBow;
            DownBowPtr myDownBow;
            HarmonicPtr myHarmonic;
            OpenStringPtr myOpenString;
            ThumbPositionPtr myThumbPosition;
            FingeringPtr myFingering;
            PluckPtr myPluck;
            DoubleTonguePtr myDoubleTongue;
            TripleTonguePtr myTripleTongue;
            StoppedPtr myStopped;
            SnapPizzicatoPtr mySnapPizzicato;
            FretPtr myFret;
            StringPtr myString;
            HammerOnPtr myHammerOn;
            PullOffPtr myPullOff;
            BendPtr myBend;
            TapPtr myTap;
            HeelPtr myHeel;
            ToePtr myToe;
            FingernailsPtr myFingernails;
            HolePtr myHole;
            ArrowPtr myArrow;
            HandbellPtr myHandbell;
            OtherTechnicalPtr myOtherTechnical;
        };
    }
}
