// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/ScoreInstrument.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/Ensemble.h"
#include "mx/core/elements/InstrumentAbbreviation.h"
#include "mx/core/elements/InstrumentName.h"
#include "mx/core/elements/InstrumentSound.h"
#include "mx/core/elements/SoloOrEnsembleChoice.h"
#include "mx/core/elements/Solo.h"
#include "mx/core/elements/VirtualInstrument.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        ScoreInstrument::ScoreInstrument()
        :myAttributes( std::make_shared<ScoreInstrumentAttributes>() )
        ,myInstrumentName( makeInstrumentName() )
        ,myInstrumentAbbreviation( makeInstrumentAbbreviation() )
        ,myHasInstrumentAbbreviation( false )
        ,myInstrumentSound( makeInstrumentSound() )
        ,myHasInstrumentSound( false )
        ,mySoloOrEnsembleChoice( makeSoloOrEnsembleChoice() )
        ,myHasSoloOrEnsembleChoice( false )
        ,myVirtualInstrument( makeVirtualInstrument() )
        ,myHasVirtualInstrument( false )
        {}


        bool ScoreInstrument::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& ScoreInstrument::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& ScoreInstrument::streamName( std::ostream& os ) const
        {
            os << "score-instrument";
            return os;
        }


        bool ScoreInstrument::hasContents() const
        {
            return true;
        }


        std::ostream& ScoreInstrument::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            os << std::endl;
            myInstrumentName->toStream( os, indentLevel+1 );
            if ( myHasInstrumentAbbreviation )
            {
                os << std::endl;
                myInstrumentAbbreviation->toStream( os, indentLevel+1 );
            }
            if ( myHasInstrumentSound )
            {
                os << std::endl;
                myInstrumentSound->toStream( os, indentLevel+1 );
            }
            if ( myHasSoloOrEnsembleChoice )
            {
                os << std::endl;
                mySoloOrEnsembleChoice->streamContents( os, indentLevel+1, isOneLineOnly );
            }
            if ( myHasVirtualInstrument )
            {
                os << std::endl;
                myVirtualInstrument->toStream( os, indentLevel+1 );
            }
            os << std::endl;
            isOneLineOnly = false;
            return os;
        }


        ScoreInstrumentAttributesPtr ScoreInstrument::getAttributes() const
        {
            return myAttributes;
        }


        void ScoreInstrument::setAttributes( const ScoreInstrumentAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        InstrumentNamePtr ScoreInstrument::getInstrumentName() const
        {
            return myInstrumentName;
        }


        void ScoreInstrument::setInstrumentName( const InstrumentNamePtr& value )
        {
            if( value )
            {
                myInstrumentName = value;
            }
        }


        InstrumentAbbreviationPtr ScoreInstrument::getInstrumentAbbreviation() const
        {
            return myInstrumentAbbreviation;
        }


        void ScoreInstrument::setInstrumentAbbreviation( const InstrumentAbbreviationPtr& value )
        {
            if( value )
            {
                myInstrumentAbbreviation = value;
            }
        }


        bool ScoreInstrument::getHasInstrumentAbbreviation() const
        {
            return myHasInstrumentAbbreviation;
        }


        void ScoreInstrument::setHasInstrumentAbbreviation( const bool value )
        {
            myHasInstrumentAbbreviation = value;
        }


        InstrumentSoundPtr ScoreInstrument::getInstrumentSound() const
        {
            return myInstrumentSound;
        }


        void ScoreInstrument::setInstrumentSound( const InstrumentSoundPtr& value )
        {
            if( value )
            {
                myInstrumentSound = value;
            }
        }


        bool ScoreInstrument::getHasInstrumentSound() const
        {
            return myHasInstrumentSound;
        }


        void ScoreInstrument::setHasInstrumentSound( const bool value )
        {
            myHasInstrumentSound = value;
        }


        SoloOrEnsembleChoicePtr ScoreInstrument::getSoloOrEnsembleChoice() const
        {
            return mySoloOrEnsembleChoice;
        }


        void ScoreInstrument::setSoloOrEnsembleChoice( const SoloOrEnsembleChoicePtr& value )
        {
            if ( value )
            {
                mySoloOrEnsembleChoice = value;
            }
        }


        bool ScoreInstrument::getHasSoloOrEnsembleChoice() const
        {
            return myHasSoloOrEnsembleChoice;
        }


        void ScoreInstrument::setHasSoloOrEnsembleChoice( const bool value )
        {
            myHasSoloOrEnsembleChoice = value;
        }


        VirtualInstrumentPtr ScoreInstrument::getVirtualInstrument() const
        {
            return myVirtualInstrument;
        }


        void ScoreInstrument::setVirtualInstrument( const VirtualInstrumentPtr& value )
        {
            if( value )
            {
                myVirtualInstrument = value;
            }
        }


        bool ScoreInstrument::getHasVirtualInstrument() const
        {
            return myHasVirtualInstrument;
        }


        void ScoreInstrument::setHasVirtualInstrument( const bool value )
        {
            myHasVirtualInstrument = value;
        }


        bool ScoreInstrument::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            bool isInstrumentNameFound = false;

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if ( importElement( message, *it, isSuccess, *myInstrumentName, isInstrumentNameFound ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myInstrumentAbbreviation, myHasInstrumentAbbreviation ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myInstrumentSound, myHasInstrumentSound ) ) { continue; }
                
                if( checkSetChoiceMember(
                    message, *it, isSuccess, mySoloOrEnsembleChoice,
                    "solo", &SoloOrEnsembleChoice::getSolo,
                    static_cast<int>( SoloOrEnsembleChoice::Choice::solo ) ) )
                { myHasSoloOrEnsembleChoice = true; continue; }
                
                if( checkSetChoiceMember(
                    message, *it, isSuccess, mySoloOrEnsembleChoice,
                    "ensemble", &SoloOrEnsembleChoice::getEnsemble,
                    static_cast<int>( SoloOrEnsembleChoice::Choice::ensemble ) ) )
                { myHasSoloOrEnsembleChoice = true; continue; }

                if ( importElement( message, *it, isSuccess, *myVirtualInstrument, myHasVirtualInstrument ) ) { continue; }
            }
            
            if( !isInstrumentNameFound )
            {
                message << "ScoreInstrument: 'instrument-name' not found" << std::endl;
                isSuccess = false;
            }

            return isSuccess;
        }

    }
}
