// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/TupletActual.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/TupletDot.h"
#include "mx/core/elements/TupletNumber.h"
#include "mx/core/elements/TupletType.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        TupletActual::TupletActual()
        :myTupletNumber( makeTupletNumber() )
        ,myHasTupletNumber( false )
        ,myTupletType( makeTupletType() )
        ,myHasTupletType( false )
        ,myTupletDotSet()
        {}


        bool TupletActual::hasAttributes() const
        {
            return false;
        }


        std::ostream& TupletActual::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& TupletActual::streamName( std::ostream& os ) const
        {
            os << "tuplet-actual";
            return os;
        }


        bool TupletActual::hasContents() const
        {
            return myHasTupletNumber || myHasTupletType || myTupletDotSet.size() > 0;
        }


        std::ostream& TupletActual::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            if ( myHasTupletNumber )
            {
                os << std::endl;
                myTupletNumber->toStream( os, indentLevel+1 );
            }
            if ( myHasTupletType )
            {
                os << std::endl;
                myTupletType->toStream( os, indentLevel+1 );
            }
            for ( auto x : myTupletDotSet )
            {
                os << std::endl;
                x->toStream( os, indentLevel+1 );
            }
            if ( hasContents() )
            {
                isOneLineOnly = false;
                os << std::endl;
            }
            else
            {
                isOneLineOnly = true;
            }
            return os;
        }


        TupletNumberPtr TupletActual::getTupletNumber() const
        {
            return myTupletNumber;
        }


        void TupletActual::setTupletNumber( const TupletNumberPtr& value )
        {
            if( value )
            {
                myTupletNumber = value;
            }
        }


        bool TupletActual::getHasTupletNumber() const
        {
            return myHasTupletNumber;
        }


        void TupletActual::setHasTupletNumber( const bool value )
        {
            myHasTupletNumber = value;
        }


        TupletTypePtr TupletActual::getTupletType() const
        {
            return myTupletType;
        }


        void TupletActual::setTupletType( const TupletTypePtr& value )
        {
            if( value )
            {
                myTupletType = value;
            }
        }


        bool TupletActual::getHasTupletType() const
        {
            return myHasTupletType;
        }


        void TupletActual::setHasTupletType( const bool value )
        {
            myHasTupletType = value;
        }


        const TupletDotSet& TupletActual::getTupletDotSet() const
        {
            return myTupletDotSet;
        }


        void TupletActual::removeTupletDot( const TupletDotSetIterConst& value )
        {
            if ( value != myTupletDotSet.cend() )
            {
                myTupletDotSet.erase( value );
            }
        }


        void TupletActual::addTupletDot( const TupletDotPtr& value )
        {
            if ( value )
            {
                myTupletDotSet.push_back( value );
            }
        }


        void TupletActual::clearTupletDotSet()
        {
            myTupletDotSet.clear();
        }


        TupletDotPtr TupletActual::getTupletDot( const TupletDotSetIterConst& setIterator ) const
        {
            if( setIterator != myTupletDotSet.cend() )
            {
                return *setIterator;
            }
            return TupletDotPtr();
        }


        bool TupletActual::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if ( importElement( message, *it, isSuccess, *myTupletNumber, myHasTupletNumber ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myTupletType, myHasTupletType ) ) { continue; }
                importElementSet( message, it, endIter, isSuccess, "tuplet-dot", myTupletDotSet );
            }

            MX_RETURN_IS_SUCCESS;
        }

    }
}
