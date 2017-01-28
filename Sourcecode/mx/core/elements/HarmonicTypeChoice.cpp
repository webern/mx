// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/HarmonicTypeChoice.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/Artificial.h"
#include "mx/core/elements/Natural.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        HarmonicTypeChoice::HarmonicTypeChoice()
        :myChoice( Choice::natural )
        ,myNatural( makeNatural() )
        ,myArtificial( makeArtificial() )
        {}


        bool HarmonicTypeChoice::hasAttributes() const
        {
            return false;
        }


        std::ostream& HarmonicTypeChoice::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& HarmonicTypeChoice::streamName( std::ostream& os ) const
        {
            return os;
        }


        bool HarmonicTypeChoice::hasContents() const
        {
            return true;
        }


        std::ostream& HarmonicTypeChoice::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            switch ( myChoice )
            {
                case Choice::natural:
                    myNatural->toStream( os, indentLevel );
                    break;
                case Choice::artificial:
                    myArtificial->toStream( os, indentLevel );
                    break;
                default:
                    break;
            }
            isOneLineOnly = false;
            return os;
        }


        HarmonicTypeChoice::Choice HarmonicTypeChoice::getChoice() const
        {
            return myChoice;
        }


        void HarmonicTypeChoice::setChoice( const HarmonicTypeChoice::Choice value )
        {
            myChoice = value;
        }


        NaturalPtr HarmonicTypeChoice::getNatural() const
        {
            return myNatural;
        }


        void HarmonicTypeChoice::setNatural( const NaturalPtr& value )
        {
            if ( value )
            {
                myNatural = value;
            }
        }


        ArtificialPtr HarmonicTypeChoice::getArtificial() const
        {
            return myArtificial;
        }


        void HarmonicTypeChoice::setArtificial( const ArtificialPtr& value )
        {
            if ( value )
            {
                myArtificial = value;
            }
        }
        
        
        MX_FROM_XELEMENT_UNUSED( HarmonicTypeChoice );

    }
}
