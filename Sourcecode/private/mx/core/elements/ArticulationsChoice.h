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

        MX_FORWARD_DECLARE_ELEMENT( Accent )
        MX_FORWARD_DECLARE_ELEMENT( BreathMark )
        MX_FORWARD_DECLARE_ELEMENT( Caesura )
        MX_FORWARD_DECLARE_ELEMENT( DetachedLegato )
        MX_FORWARD_DECLARE_ELEMENT( Doit )
        MX_FORWARD_DECLARE_ELEMENT( Falloff )
        MX_FORWARD_DECLARE_ELEMENT( OtherArticulation )
        MX_FORWARD_DECLARE_ELEMENT( Plop )
        MX_FORWARD_DECLARE_ELEMENT( Scoop )
        MX_FORWARD_DECLARE_ELEMENT( Spiccato )
        MX_FORWARD_DECLARE_ELEMENT( Staccatissimo )
        MX_FORWARD_DECLARE_ELEMENT( Staccato )
        MX_FORWARD_DECLARE_ELEMENT( Stress )
        MX_FORWARD_DECLARE_ELEMENT( StrongAccent )
        MX_FORWARD_DECLARE_ELEMENT( Tenuto )
        MX_FORWARD_DECLARE_ELEMENT( Unstress )
        MX_FORWARD_DECLARE_ELEMENT( ArticulationsChoice )

        inline ArticulationsChoicePtr makeArticulationsChoice() { return std::make_shared<ArticulationsChoice>(); }

        class ArticulationsChoice : public ElementInterface
        {
        public:
            enum class Choice
            {
                accent = 1,
                strongAccent = 2,
                staccato = 3,
                tenuto = 4,
                detachedLegato = 5,
                staccatissimo = 6,
                spiccato = 7,
                scoop = 8,
                plop = 9,
                doit = 10,
                falloff = 11,
                breathMark = 12,
                caesura = 13,
                stress = 14,
                unstress = 15,
                otherArticulation = 16
            };
            ArticulationsChoice();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Choice minOccurs = 1, maxOccurs = 1 _________ */
            ArticulationsChoice::Choice getChoice() const;
            void setChoice( const ArticulationsChoice::Choice value );

            /* _________ Accent minOccurs = 1, maxOccurs = 1 _________ */
            AccentPtr getAccent() const;
            void setAccent( const AccentPtr& value );

            /* _________ StrongAccent minOccurs = 1, maxOccurs = 1 _________ */
            StrongAccentPtr getStrongAccent() const;
            void setStrongAccent( const StrongAccentPtr& value );

            /* _________ Staccato minOccurs = 1, maxOccurs = 1 _________ */
            StaccatoPtr getStaccato() const;
            void setStaccato( const StaccatoPtr& value );

            /* _________ Tenuto minOccurs = 1, maxOccurs = 1 _________ */
            TenutoPtr getTenuto() const;
            void setTenuto( const TenutoPtr& value );

            /* _________ DetachedLegato minOccurs = 1, maxOccurs = 1 _________ */
            DetachedLegatoPtr getDetachedLegato() const;
            void setDetachedLegato( const DetachedLegatoPtr& value );

            /* _________ Staccatissimo minOccurs = 1, maxOccurs = 1 _________ */
            StaccatissimoPtr getStaccatissimo() const;
            void setStaccatissimo( const StaccatissimoPtr& value );

            /* _________ Spiccato minOccurs = 1, maxOccurs = 1 _________ */
            SpiccatoPtr getSpiccato() const;
            void setSpiccato( const SpiccatoPtr& value );

            /* _________ Scoop minOccurs = 1, maxOccurs = 1 _________ */
            ScoopPtr getScoop() const;
            void setScoop( const ScoopPtr& value );

            /* _________ Plop minOccurs = 1, maxOccurs = 1 _________ */
            PlopPtr getPlop() const;
            void setPlop( const PlopPtr& value );

            /* _________ Doit minOccurs = 1, maxOccurs = 1 _________ */
            DoitPtr getDoit() const;
            void setDoit( const DoitPtr& value );

            /* _________ Falloff minOccurs = 1, maxOccurs = 1 _________ */
            FalloffPtr getFalloff() const;
            void setFalloff( const FalloffPtr& value );

            /* _________ BreathMark minOccurs = 1, maxOccurs = 1 _________ */
            BreathMarkPtr getBreathMark() const;
            void setBreathMark( const BreathMarkPtr& value );

            /* _________ Caesura minOccurs = 1, maxOccurs = 1 _________ */
            CaesuraPtr getCaesura() const;
            void setCaesura( const CaesuraPtr& value );

            /* _________ Stress minOccurs = 1, maxOccurs = 1 _________ */
            StressPtr getStress() const;
            void setStress( const StressPtr& value );

            /* _________ Unstress minOccurs = 1, maxOccurs = 1 _________ */
            UnstressPtr getUnstress() const;
            void setUnstress( const UnstressPtr& value );

            /* _________ OtherArticulation minOccurs = 1, maxOccurs = 1 _________ */
            OtherArticulationPtr getOtherArticulation() const;
            void setOtherArticulation( const OtherArticulationPtr& value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, xml::XElement& xelement );

        private:
            Choice myChoice;
            AccentPtr myAccent;
            StrongAccentPtr myStrongAccent;
            StaccatoPtr myStaccato;
            TenutoPtr myTenuto;
            DetachedLegatoPtr myDetachedLegato;
            StaccatissimoPtr myStaccatissimo;
            SpiccatoPtr mySpiccato;
            ScoopPtr myScoop;
            PlopPtr myPlop;
            DoitPtr myDoit;
            FalloffPtr myFalloff;
            BreathMarkPtr myBreathMark;
            CaesuraPtr myCaesura;
            StressPtr myStress;
            UnstressPtr myUnstress;
            OtherArticulationPtr myOtherArticulation;
        };
    }
}
