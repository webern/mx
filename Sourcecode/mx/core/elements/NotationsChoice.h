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

        MX_FORWARD_DECLARE_ELEMENT( AccidentalMark )
        MX_FORWARD_DECLARE_ELEMENT( Arpeggiate )
        MX_FORWARD_DECLARE_ELEMENT( Articulations )
        MX_FORWARD_DECLARE_ELEMENT( Dynamics )
        MX_FORWARD_DECLARE_ELEMENT( Fermata )
        MX_FORWARD_DECLARE_ELEMENT( Glissando )
        MX_FORWARD_DECLARE_ELEMENT( NonArpeggiate )
        MX_FORWARD_DECLARE_ELEMENT( Ornaments )
        MX_FORWARD_DECLARE_ELEMENT( OtherNotation )
        MX_FORWARD_DECLARE_ELEMENT( Slide )
        MX_FORWARD_DECLARE_ELEMENT( Slur )
        MX_FORWARD_DECLARE_ELEMENT( Technical )
        MX_FORWARD_DECLARE_ELEMENT( Tied )
        MX_FORWARD_DECLARE_ELEMENT( Tuplet )
        MX_FORWARD_DECLARE_ELEMENT( NotationsChoice )

        inline NotationsChoicePtr makeNotationsChoice() { return std::make_shared<NotationsChoice>(); }

        class NotationsChoice : public ElementInterface
        {
        public:
            enum class Choice
            {
                tied = 1,
                slur = 2,
                tuplet = 3,
                glissando = 4,
                slide = 5,
                ornaments = 6,
                technical = 7,
                articulations = 8,
                dynamics = 9,
                fermata = 10,
                arpeggiate = 11,
                nonArpeggiate = 12,
                accidentalMark = 13,
                otherNotation = 14
            };
            NotationsChoice();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Choice minOccurs = 1, maxOccurs = 1 _________ */
            NotationsChoice::Choice getChoice() const;
            void setChoice( const NotationsChoice::Choice value );

            /* _________ Tied minOccurs = 1, maxOccurs = 1 _________ */
            TiedPtr getTied() const;
            void setTied( const TiedPtr& value );

            /* _________ Slur minOccurs = 1, maxOccurs = 1 _________ */
            SlurPtr getSlur() const;
            void setSlur( const SlurPtr& value );

            /* _________ Tuplet minOccurs = 1, maxOccurs = 1 _________ */
            TupletPtr getTuplet() const;
            void setTuplet( const TupletPtr& value );

            /* _________ Glissando minOccurs = 1, maxOccurs = 1 _________ */
            GlissandoPtr getGlissando() const;
            void setGlissando( const GlissandoPtr& value );

            /* _________ Slide minOccurs = 1, maxOccurs = 1 _________ */
            SlidePtr getSlide() const;
            void setSlide( const SlidePtr& value );

            /* _________ Ornaments minOccurs = 1, maxOccurs = 1 _________ */
            OrnamentsPtr getOrnaments() const;
            void setOrnaments( const OrnamentsPtr& value );

            /* _________ Technical minOccurs = 1, maxOccurs = 1 _________ */
            TechnicalPtr getTechnical() const;
            void setTechnical( const TechnicalPtr& value );

            /* _________ Articulations minOccurs = 1, maxOccurs = 1 _________ */
            ArticulationsPtr getArticulations() const;
            void setArticulations( const ArticulationsPtr& value );

            /* _________ Dynamics minOccurs = 1, maxOccurs = 1 _________ */
            DynamicsPtr getDynamics() const;
            void setDynamics( const DynamicsPtr& value );

            /* _________ Fermata minOccurs = 1, maxOccurs = 1 _________ */
            FermataPtr getFermata() const;
            void setFermata( const FermataPtr& value );

            /* _________ Arpeggiate minOccurs = 1, maxOccurs = 1 _________ */
            ArpeggiatePtr getArpeggiate() const;
            void setArpeggiate( const ArpeggiatePtr& value );

            /* _________ NonArpeggiate minOccurs = 1, maxOccurs = 1 _________ */
            NonArpeggiatePtr getNonArpeggiate() const;
            void setNonArpeggiate( const NonArpeggiatePtr& value );

            /* _________ AccidentalMark minOccurs = 1, maxOccurs = 1 _________ */
            AccidentalMarkPtr getAccidentalMark() const;
            void setAccidentalMark( const AccidentalMarkPtr& value );

            /* _________ OtherNotation minOccurs = 1, maxOccurs = 1 _________ */
            OtherNotationPtr getOtherNotation() const;
            void setOtherNotation( const OtherNotationPtr& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            Choice myChoice;
            TiedPtr myTied;
            SlurPtr mySlur;
            TupletPtr myTuplet;
            GlissandoPtr myGlissando;
            SlidePtr mySlide;
            OrnamentsPtr myOrnaments;
            TechnicalPtr myTechnical;
            ArticulationsPtr myArticulations;
            DynamicsPtr myDynamics;
            FermataPtr myFermata;
            ArpeggiatePtr myArpeggiate;
            NonArpeggiatePtr myNonArpeggiate;
            AccidentalMarkPtr myAccidentalMark;
            OtherNotationPtr myOtherNotation;
        };
    }
}
