// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/MetronomeNote.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/MetronomeBeam.h"
#include "mx/core/elements/MetronomeDot.h"
#include "mx/core/elements/MetronomeTuplet.h"
#include "mx/core/elements/MetronomeType.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        MetronomeNote::MetronomeNote()
        :myMetronomeType( makeMetronomeType() )
        ,myMetronomeDotSet()
        ,myMetronomeBeamSet()
        ,myMetronomeTuplet( makeMetronomeTuplet() )
        ,myHasMetronomeTuplet( false )
        {}


        bool MetronomeNote::hasAttributes() const
        {
            return false;
        }


        std::ostream& MetronomeNote::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& MetronomeNote::streamName( std::ostream& os ) const
        {
            os << "metronome-note";
            return os;
        }


        bool MetronomeNote::hasContents() const
        {
            return true;
        }


        std::ostream& MetronomeNote::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            isOneLineOnly = false;
            os << std::endl;
            myMetronomeType->toStream( os, indentLevel+1 );
            for ( auto x : myMetronomeDotSet )
            {
                os << std::endl;
                x->toStream( os, indentLevel+1 );
            }
            for ( auto x : myMetronomeBeamSet )
            {
                os << std::endl;
                x->toStream( os, indentLevel+1 );
            }
            if ( myHasMetronomeTuplet )
            {
                os << std::endl;
                myMetronomeTuplet->toStream( os, indentLevel+1 );
            }
            os << std::endl;
            return os;
        }


        MetronomeTypePtr MetronomeNote::getMetronomeType() const
        {
            return myMetronomeType;
        }


        void MetronomeNote::setMetronomeType( const MetronomeTypePtr& value )
        {
            if( value )
            {
                myMetronomeType = value;
            }
        }


        const MetronomeDotSet& MetronomeNote::getMetronomeDotSet() const
        {
            return myMetronomeDotSet;
        }


        void MetronomeNote::removeMetronomeDot( const MetronomeDotSetIterConst& value )
        {
            if ( value != myMetronomeDotSet.cend() )
            {
                myMetronomeDotSet.erase( value );
            }
        }


        void MetronomeNote::addMetronomeDot( const MetronomeDotPtr& value )
        {
            if ( value )
            {
                myMetronomeDotSet.push_back( value );
            }
        }


        void MetronomeNote::clearMetronomeDotSet()
        {
            myMetronomeDotSet.clear();
        }


        MetronomeDotPtr MetronomeNote::getMetronomeDot( const MetronomeDotSetIterConst& setIterator ) const
        {
            if( setIterator != myMetronomeDotSet.cend() )
            {
                return *setIterator;
            }
            return MetronomeDotPtr();
        }


        const MetronomeBeamSet& MetronomeNote::getMetronomeBeamSet() const
        {
            return myMetronomeBeamSet;
        }


        void MetronomeNote::removeMetronomeBeam( const MetronomeBeamSetIterConst& value )
        {
            if ( value != myMetronomeBeamSet.cend() )
            {
                myMetronomeBeamSet.erase( value );
            }
        }


        void MetronomeNote::addMetronomeBeam( const MetronomeBeamPtr& value )
        {
            if ( value )
            {
                myMetronomeBeamSet.push_back( value );
            }
        }


        void MetronomeNote::clearMetronomeBeamSet()
        {
            myMetronomeBeamSet.clear();
        }


        MetronomeBeamPtr MetronomeNote::getMetronomeBeam( const MetronomeBeamSetIterConst& setIterator ) const
        {
            if( setIterator != myMetronomeBeamSet.cend() )
            {
                return *setIterator;
            }
            return MetronomeBeamPtr();
        }


        MetronomeTupletPtr MetronomeNote::getMetronomeTuplet() const
        {
            return myMetronomeTuplet;
        }


        void MetronomeNote::setMetronomeTuplet( const MetronomeTupletPtr& value )
        {
            if( value )
            {
                myMetronomeTuplet = value;
            }
        }


        bool MetronomeNote::getHasMetronomeTuplet() const
        {
            return myHasMetronomeTuplet;
        }


        void MetronomeNote::setHasMetronomeTuplet( const bool value )
        {
            myHasMetronomeTuplet = value;
        }


        bool MetronomeNote::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            bool isMetronomeTypeFound = false;

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if ( importElement( message, *it, isSuccess, *myMetronomeType, isMetronomeTypeFound ) ) { continue; }
                importElementSet( message, it, endIter, isSuccess, "metronome-dot", myMetronomeDotSet );
                importElementSet( message, it, endIter, isSuccess, "metronome-beam", myMetronomeBeamSet );
                if ( importElement( message, *it, isSuccess, *myMetronomeTuplet, myHasMetronomeTuplet ) ) { continue; }
            }

            if( !isMetronomeTypeFound )
            {
                message << "MetronomeNote: '" << myMetronomeType->getElementName() << "' is required but was not found" << std::endl;
            }
            MX_RETURN_IS_SUCCESS;
        }

    }
}
