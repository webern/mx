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

        MX_FORWARD_DECLARE_ELEMENT( Ensemble )
        MX_FORWARD_DECLARE_ELEMENT( Solo )
        MX_FORWARD_DECLARE_ELEMENT( SoloOrEnsembleChoice )

        inline SoloOrEnsembleChoicePtr makeSoloOrEnsembleChoice() { return std::make_shared<SoloOrEnsembleChoice>(); }

        class SoloOrEnsembleChoice : public ElementInterface
        {
        public:
            enum class Choice
            {
                solo = 1,
                ensemble = 2
            };
            SoloOrEnsembleChoice();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Choice  _________ */
            SoloOrEnsembleChoice::Choice getChoice() const;
            void setChoice( const SoloOrEnsembleChoice::Choice value );

            /* _________ Solo minOccurs = 1, maxOccurs = 1 _________ */
            SoloPtr getSolo() const;
            void setSolo( const SoloPtr& value );

            /* _________ Ensemble minOccurs = 1, maxOccurs = 1 _________ */
            EnsemblePtr getEnsemble() const;
            void setEnsemble( const EnsemblePtr& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            Choice myChoice;
            SoloPtr mySolo;
            EnsemblePtr myEnsemble;
        };
    }
}
