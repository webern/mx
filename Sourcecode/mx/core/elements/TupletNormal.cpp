// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/TupletNormal.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/TupletDot.h"
#include "mx/core/elements/TupletNumber.h"
#include "mx/core/elements/TupletType.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        TupletNormal::TupletNormal()
        :myTupletNumber( makeTupletNumber() )
        ,myHasTupletNumber( false )
        ,myTupletType( makeTupletType() )
        ,myHasTupletType( false )
        ,myTupletDotSet()
        {}


        bool TupletNormal::hasAttributes() const
        {
            return false;
        }


        std::ostream& TupletNormal::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& TupletNormal::streamName( std::ostream& os ) const
        {
            os << "tuplet-normal";
            return os;
        }


        bool TupletNormal::hasContents() const
        {
            return myHasTupletNumber || myHasTupletType || myTupletDotSet.size() > 0;
        }


        std::ostream& TupletNormal::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
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


        TupletNumberPtr TupletNormal::getTupletNumber() const
        {
            return myTupletNumber;
        }


        void TupletNormal::setTupletNumber( const TupletNumberPtr& value )
        {
            if( value )
            {
                myTupletNumber = value;
            }
        }


        bool TupletNormal::getHasTupletNumber() const
        {
            return myHasTupletNumber;
        }


        void TupletNormal::setHasTupletNumber( const bool value )
        {
            myHasTupletNumber = value;
        }


        TupletTypePtr TupletNormal::getTupletType() const
        {
            return myTupletType;
        }


        void TupletNormal::setTupletType( const TupletTypePtr& value )
        {
            if( value )
            {
                myTupletType = value;
            }
        }


        bool TupletNormal::getHasTupletType() const
        {
            return myHasTupletType;
        }


        void TupletNormal::setHasTupletType( const bool value )
        {
            myHasTupletType = value;
        }


        const TupletDotSet& TupletNormal::getTupletDotSet() const
        {
            return myTupletDotSet;
        }


        void TupletNormal::removeTupletDot( const TupletDotSetIterConst& value )
        {
            if ( value != myTupletDotSet.cend() )
            {
                myTupletDotSet.erase( value );
            }
        }


        void TupletNormal::addTupletDot( const TupletDotPtr& value )
        {
            if ( value )
            {
                myTupletDotSet.push_back( value );
            }
        }


        void TupletNormal::clearTupletDotSet()
        {
            myTupletDotSet.clear();
        }


        TupletDotPtr TupletNormal::getTupletDot( const TupletDotSetIterConst& setIterator ) const
        {
            if( setIterator != myTupletDotSet.cend() )
            {
                return *setIterator;
            }
            return TupletDotPtr();
        }


        bool TupletNormal::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
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
