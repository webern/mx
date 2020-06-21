// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Tuplet.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/TupletActual.h"
#include "mx/core/elements/TupletNormal.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Tuplet::Tuplet()
        :myAttributes( std::make_shared<TupletAttributes>() )
        ,myTupletActual( makeTupletActual() )
        ,myHasTupletActual( false )
        ,myTupletNormal( makeTupletNormal() )
        ,myHasTupletNormal( false )
        {}


        bool Tuplet::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& Tuplet::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& Tuplet::streamName( std::ostream& os ) const
        {
            os << "tuplet";
            return os;
        }


        bool Tuplet::hasContents() const
        {
            return myHasTupletActual || myHasTupletNormal;
        }


        std::ostream& Tuplet::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            if ( myHasTupletActual )
            {
                os << std::endl;
                myTupletActual->toStream( os, indentLevel+1 );
            }
            if ( myHasTupletNormal )
            {
                os << std::endl;
                myTupletNormal->toStream( os, indentLevel+1 );
            }
            isOneLineOnly = ! hasContents();
            if ( ! isOneLineOnly )
            {
                os << std::endl;
            }
            return os;
        }


        TupletAttributesPtr Tuplet::getAttributes() const
        {
            return myAttributes;
        }


        void Tuplet::setAttributes( const TupletAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        TupletActualPtr Tuplet::getTupletActual() const
        {
            return myTupletActual;
        }


        void Tuplet::setTupletActual( const TupletActualPtr& value )
        {
            if( value )
            {
                myTupletActual = value;
            }
        }


        bool Tuplet::getHasTupletActual() const
        {
            return myHasTupletActual;
        }


        void Tuplet::setHasTupletActual( const bool value )
        {
            myHasTupletActual = value;
        }


        TupletNormalPtr Tuplet::getTupletNormal() const
        {
            return myTupletNormal;
        }


        void Tuplet::setTupletNormal( const TupletNormalPtr& value )
        {
            if( value )
            {
                myTupletNormal = value;
            }
        }


        bool Tuplet::getHasTupletNormal() const
        {
            return myHasTupletNormal;
        }


        void Tuplet::setHasTupletNormal( const bool value )
        {
            myHasTupletNormal = value;
        }


        bool Tuplet::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if ( importElement( message, *it, isSuccess, *myTupletActual, myHasTupletActual ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myTupletNormal, myHasTupletNormal ) ) { continue; }
            }

            return isSuccess;
        }

    }
}
