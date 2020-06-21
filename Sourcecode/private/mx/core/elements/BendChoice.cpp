// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/BendChoice.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/PreBend.h"
#include "mx/core/elements/Release.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        BendChoice::BendChoice()
        :myChoice( Choice::preBend )
        ,myPreBend( makePreBend() )
        ,myRelease( makeRelease() )
        {}


        bool BendChoice::hasAttributes() const
        {
            return false;
        }


        std::ostream& BendChoice::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& BendChoice::streamName( std::ostream& os ) const
        {
            return os;
        }


        bool BendChoice::hasContents() const
        {
            return true;
        }


        std::ostream& BendChoice::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( isOneLineOnly );
            switch ( myChoice )
            {
                case Choice::preBend:
                {
                    myPreBend->toStream( os, indentLevel );
                }
                    break;
                case Choice::release:
                {
                    myRelease->toStream( os, indentLevel );
                }
                    break;
                default:
                    break;
            }
            return os;
        }


        BendChoice::Choice BendChoice::getChoice() const
        {
            return myChoice;
        }


        void BendChoice::setChoice( BendChoice::Choice value )
        {
            myChoice = value;
        }


        PreBendPtr BendChoice::getPreBend() const
        {
            return myPreBend;
        }


        void BendChoice::setPreBend( const PreBendPtr& value )
        {
            if ( value )
            {
                myPreBend = value;
            }
        }


        ReleasePtr BendChoice::getRelease() const
        {
            return myRelease;
        }


        void BendChoice::setRelease( const ReleasePtr& value )
        {
            if ( value )
            {
                myRelease = value;
            }
        }


        MX_FROM_XELEMENT_UNUSED( BendChoice );

    }
}
