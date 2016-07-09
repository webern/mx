// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/HarmonyChordGroup.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/Bass.h"
#include "mx/core/elements/Degree.h"
#include "mx/core/elements/Function.h"
#include "mx/core/elements/Inversion.h"
#include "mx/core/elements/Kind.h"
#include "mx/core/elements/Root.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        HarmonyChordGroup::HarmonyChordGroup()
        :myChoice( Choice::root )
        ,myRoot( makeRoot() )
        ,myFunction( makeFunction() )
        ,myKind( makeKind() )
        ,myInversion( makeInversion() )
        ,myHasInversion( false )
        ,myBass( makeBass() )
        ,myHasBass( false )
        ,myDegreeSet()
        {}


        bool HarmonyChordGroup::hasAttributes() const
        {
            return false;
        }


        std::ostream& HarmonyChordGroup::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& HarmonyChordGroup::streamName( std::ostream& os ) const
        {
            return os;
        }


        bool HarmonyChordGroup::hasContents() const
        {
            return true;
        }


        std::ostream& HarmonyChordGroup::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            switch ( myChoice )
            {
                case Choice::root:
                    myRoot->toStream( os, indentLevel );
                    break;
                case Choice::function:
                    myFunction->toStream( os, indentLevel );
                    break;
                default:
                    break;
            }
            os << std::endl;
            myKind->toStream( os, indentLevel );
            if ( myHasInversion )
            {
                os << std::endl;
                myInversion->toStream( os, indentLevel );
            }
            if ( myHasBass )
            {
                os << std::endl;
                myBass->toStream( os, indentLevel );
            }
            for ( auto x : myDegreeSet )
            {
                os << std::endl;
                x->toStream( os, indentLevel );
            }
            isOneLineOnly = false;
            return os;
        }


        HarmonyChordGroup::Choice HarmonyChordGroup::getChoice() const
        {
            return myChoice;
        }


        void HarmonyChordGroup::setChoice( const HarmonyChordGroup::Choice value )
        {
            myChoice = value;
        }


        RootPtr HarmonyChordGroup::getRoot() const
        {
            return myRoot;
        }


        void HarmonyChordGroup::setRoot( const RootPtr& value )
        {
            if ( value )
            {
                myRoot = value;
            }
        }


        FunctionPtr HarmonyChordGroup::getFunction() const
        {
            return myFunction;
        }


        void HarmonyChordGroup::setFunction( const FunctionPtr& value )
        {
            if ( value )
            {
                myFunction = value;
            }
        }


        KindPtr HarmonyChordGroup::getKind() const
        {
            return myKind;
        }


        void HarmonyChordGroup::setKind( const KindPtr& value )
        {
            if ( value )
            {
                myKind = value;
            }
        }


        InversionPtr HarmonyChordGroup::getInversion() const
        {
            return myInversion;
        }


        void HarmonyChordGroup::setInversion( const InversionPtr& value )
        {
            if ( value )
            {
                myInversion = value;
            }
        }


        bool HarmonyChordGroup::getHasInversion() const
        {
            return myHasInversion;
        }


        void HarmonyChordGroup::setHasInversion( const bool value )
        {
            myHasInversion = value;
        }


        BassPtr HarmonyChordGroup::getBass() const
        {
            return myBass;
        }


        void HarmonyChordGroup::setBass( const BassPtr& value )
        {
            if ( value )
            {
                myBass = value;
            }
        }


        bool HarmonyChordGroup::getHasBass() const
        {
            return myHasBass;
        }


        void HarmonyChordGroup::setHasBass( const bool value )
        {
            myHasBass = value;
        }


        const DegreeSet& HarmonyChordGroup::getDegreeSet() const
        {
            return myDegreeSet;
        }


        void HarmonyChordGroup::addDegree( const DegreePtr& value )
        {
            if ( value )
            {
                myDegreeSet.push_back( value );
            }
        }


        void HarmonyChordGroup::removeDegree( const DegreeSetIterConst& value )
        {
            if ( value != myDegreeSet.cend() )
            {
                myDegreeSet.erase( value );
            }
        }


        void HarmonyChordGroup::clearDegreeSet()
        {
            myDegreeSet.clear();
        }


        DegreePtr HarmonyChordGroup::getDegree( const DegreeSetIterConst& setIterator ) const
        {
            if( setIterator != myDegreeSet.cend() )
            {
                return *setIterator;
            }
            return DegreePtr();
        }


        MX_FROM_XELEMENT_UNUSED( HarmonyChordGroup );

    }
}
