// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "mx/core/elements/ScoreInstrumentAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ATTRIBUTES( ScoreInstrumentAttributes )
        MX_FORWARD_DECLARE_ELEMENT( InstrumentAbbreviation )
        MX_FORWARD_DECLARE_ELEMENT( InstrumentName )
        MX_FORWARD_DECLARE_ELEMENT( InstrumentSound )
        MX_FORWARD_DECLARE_ELEMENT( SoloOrEnsembleChoice )
        MX_FORWARD_DECLARE_ELEMENT( VirtualInstrument )
        MX_FORWARD_DECLARE_ELEMENT( ScoreInstrument )

        inline ScoreInstrumentPtr makeScoreInstrument() { return std::make_shared<ScoreInstrument>(); }

        class ScoreInstrument : public ElementInterface
        {
        public:
            ScoreInstrument();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;
            ScoreInstrumentAttributesPtr getAttributes() const;
            void setAttributes( const ScoreInstrumentAttributesPtr& value );

            /* _________ InstrumentName minOccurs = 1, maxOccurs = 1 _________ */
            InstrumentNamePtr getInstrumentName() const;
            void setInstrumentName( const InstrumentNamePtr& value );

            /* _________ InstrumentAbbreviation minOccurs = 0, maxOccurs = 1 _________ */
            InstrumentAbbreviationPtr getInstrumentAbbreviation() const;
            void setInstrumentAbbreviation( const InstrumentAbbreviationPtr& value );
            bool getHasInstrumentAbbreviation() const;
            void setHasInstrumentAbbreviation( const bool value );

            /* _________ SoloOrEnsembleChoice minOccurs = 0, maxOccurs = 1 _________ */
            SoloOrEnsembleChoicePtr getSoloOrEnsembleChoice() const;
            void setSoloOrEnsembleChoice( const SoloOrEnsembleChoicePtr& value );
            bool getHasSoloOrEnsembleChoice() const;
            void setHasSoloOrEnsembleChoice( const bool value );

            /* _________ InstrumentSound minOccurs = 0, maxOccurs = 1 _________ */
            InstrumentSoundPtr getInstrumentSound() const;
            void setInstrumentSound( const InstrumentSoundPtr& value );
            bool getHasInstrumentSound() const;
            void setHasInstrumentSound( const bool value );

            /* _________ VirtualInstrument minOccurs = 0, maxOccurs = 1 _________ */
            VirtualInstrumentPtr getVirtualInstrument() const;
            void setVirtualInstrument( const VirtualInstrumentPtr& value );
            bool getHasVirtualInstrument() const;
            void setHasVirtualInstrument( const bool value );

            private:
            virtual bool fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement );

        private:
            ScoreInstrumentAttributesPtr myAttributes;
            InstrumentNamePtr myInstrumentName;
            InstrumentAbbreviationPtr myInstrumentAbbreviation;
            bool myHasInstrumentAbbreviation;
            InstrumentSoundPtr myInstrumentSound;
            bool myHasInstrumentSound;
            SoloOrEnsembleChoicePtr mySoloOrEnsembleChoice;
            bool myHasSoloOrEnsembleChoice;
            VirtualInstrumentPtr myVirtualInstrument;
            bool myHasVirtualInstrument;
        };
    }
}
