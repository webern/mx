// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/SoloOrEnsembleChoice.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/Ensemble.h"
#include "mx/core/elements/Solo.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        SoloOrEnsembleChoice::SoloOrEnsembleChoice()
        :myChoice( Choice::solo )
        ,mySolo( makeSolo() )
        ,myEnsemble( makeEnsemble() )
        {}


        bool SoloOrEnsembleChoice::hasAttributes() const
        {
            return false;
        }


        std::ostream& SoloOrEnsembleChoice::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& SoloOrEnsembleChoice::streamName( std::ostream& os ) const
        {
            return os;
        }


        bool SoloOrEnsembleChoice::hasContents() const
        {
            return true;
        }


        std::ostream& SoloOrEnsembleChoice::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            switch ( myChoice )
            {
                case Choice::solo:
                {
                    mySolo->toStream( os, indentLevel );
                }
                    break;
                case Choice::ensemble:
                {
                    myEnsemble->toStream( os, indentLevel );
                }
                    break;
                default:
                    break;
            }
            isOneLineOnly = true;
            return os;
        }


        SoloOrEnsembleChoice::Choice SoloOrEnsembleChoice::getChoice() const
        {
            return myChoice;
        }


        void SoloOrEnsembleChoice::setChoice( const SoloOrEnsembleChoice::Choice value )
        {
            myChoice = value;
        }


        SoloPtr SoloOrEnsembleChoice::getSolo() const
        {
            return mySolo;
        }


        void SoloOrEnsembleChoice::setSolo( const SoloPtr& value )
        {
            if ( value )
            {
                mySolo = value;
            }
        }


        EnsemblePtr SoloOrEnsembleChoice::getEnsemble() const
        {
            return myEnsemble;
        }


        void SoloOrEnsembleChoice::setEnsemble( const EnsemblePtr& value )
        {
            if ( value )
            {
                myEnsemble = value;
            }
        }


        MX_FROM_XELEMENT_UNUSED( SoloOrEnsembleChoice );

    }
}
