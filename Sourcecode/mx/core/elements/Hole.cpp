// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/Hole.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/HoleClosed.h"
#include "mx/core/elements/HoleShape.h"
#include "mx/core/elements/HoleType.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Hole::Hole()
        :myAttributes( std::make_shared<HoleAttributes>() )
        ,myHoleType( makeHoleType() )
        ,myHasHoleType( false )
        ,myHoleClosed( makeHoleClosed() )
        ,myHoleShape( makeHoleShape() )
        ,myHasHoleShape( false )
        {}


        bool Hole::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& Hole::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& Hole::streamName( std::ostream& os ) const
        {
            os << "hole";
            return os;
        }


        bool Hole::hasContents() const
        {
            return true;
        }


        std::ostream& Hole::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            if ( myHasHoleType )
            {
                os << std::endl;
                myHoleType->toStream( os, indentLevel+1 );
            }
            os << std::endl;
            myHoleClosed->toStream( os, indentLevel+1 );
            if ( myHasHoleShape )
            {
                os << std::endl;
                myHoleShape->toStream( os, indentLevel+1 );
            }
            os << std::endl;
            isOneLineOnly = false;
            return os;
        }


        HoleAttributesPtr Hole::getAttributes() const
        {
            return myAttributes;
        }


        void Hole::setAttributes( const HoleAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        HoleTypePtr Hole::getHoleType() const
        {
            return myHoleType;
        }


        void Hole::setHoleType( const HoleTypePtr& value )
        {
            if( value )
            {
                myHoleType = value;
            }
        }


        bool Hole::getHasHoleType() const
        {
            return myHasHoleType;
        }


        void Hole::setHasHoleType( const bool value )
        {
            myHasHoleType = value;
        }


        HoleClosedPtr Hole::getHoleClosed() const
        {
            return myHoleClosed;
        }


        void Hole::setHoleClosed( const HoleClosedPtr& value )
        {
            if( value )
            {
                myHoleClosed = value;
            }
        }


        HoleShapePtr Hole::getHoleShape() const
        {
            return myHoleShape;
        }


        void Hole::setHoleShape( const HoleShapePtr& value )
        {
            if( value )
            {
                myHoleShape = value;
            }
        }


        bool Hole::getHasHoleShape() const
        {
            return myHasHoleShape;
        }


        void Hole::setHasHoleShape( const bool value )
        {
            myHasHoleShape = value;
        }


        bool Hole::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            bool isHoleClosedFound = false;

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if ( importElement( message, *it, isSuccess, *myHoleType, myHasHoleType ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myHoleClosed, isHoleClosedFound ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myHoleShape, myHasHoleShape ) ) { continue; }
            }

            if( !isHoleClosedFound )
            {
                message << "Hole: '" << myHoleClosed->getElementName() << "' is required but was not found" << std::endl;
            }
            MX_RETURN_IS_SUCCESS;
        }

    }
}
