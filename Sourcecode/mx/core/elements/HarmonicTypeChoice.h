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

        MX_FORWARD_DECLARE_ELEMENT( Artificial )
        MX_FORWARD_DECLARE_ELEMENT( Natural )
        MX_FORWARD_DECLARE_ELEMENT( HarmonicTypeChoice )

        inline HarmonicTypeChoicePtr makeHarmonicTypeChoice() { return std::make_shared<HarmonicTypeChoice>(); }

        class HarmonicTypeChoice : public ElementInterface
        {
        public:
            enum class Choice
            {
                natural = 1,
                artificial = 2
            };
            HarmonicTypeChoice();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Choice minOccurs = 1, maxOccurs = 1 _________ */
            HarmonicTypeChoice::Choice getChoice() const;
            void setChoice( const HarmonicTypeChoice::Choice value );

            /* _________ Natural minOccurs = 1, maxOccurs = 1 _________ */
            NaturalPtr getNatural() const;
            void setNatural( const NaturalPtr& value );

            /* _________ Artificial minOccurs = 1, maxOccurs = 1 _________ */
            ArtificialPtr getArtificial() const;
            void setArtificial( const ArtificialPtr& value );
            
            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            Choice myChoice;
            NaturalPtr myNatural;
            ArtificialPtr myArtificial;
        };
    }
}
