// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Scaling.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/Millimeters.h"
#include "mx/core/elements/Tenths.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Scaling::Scaling()
        :myMillimeters( makeMillimeters( MillimetersValue( 7 ) ) )
        ,myTenths( makeTenths( TenthsValue( 40 ) ) )
        {}


        bool Scaling::hasAttributes() const
        {
            return false;
        }


        std::ostream& Scaling::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& Scaling::streamName( std::ostream& os ) const
        {
            os << "scaling";
            return os;
        }


        bool Scaling::hasContents() const
        {
            return true;
        }


        std::ostream& Scaling::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            os << std::endl;
            myMillimeters->toStream( os, indentLevel+1 );
            os << std::endl;
            myTenths->toStream( os, indentLevel+1 );
            os << std::endl;
            isOneLineOnly = false;
            return os;
        }


        MillimetersPtr Scaling::getMillimeters() const
        {
            return myMillimeters;
        }


        void Scaling::setMillimeters( const MillimetersPtr& value )
        {
            if( value )
            {
                myMillimeters = value;
            }
        }


        TenthsPtr Scaling::getTenths() const
        {
            return myTenths;
        }


        void Scaling::setTenths( const TenthsPtr& value )
        {
            if( value )
            {
                myTenths = value;
            }
        }


        bool Scaling::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            bool isSuccess = true;
            bool isMillimetersFound = false;
            bool isTenthsFound = false;

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if ( importElement( message, *it, isSuccess, *myMillimeters, isMillimetersFound ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myTenths, isTenthsFound ) ) { continue; }
            }

            if( !isMillimetersFound )
            {
                message << "Scaling: '" << myMillimeters->getElementName() << "' is required but was not found" << std::endl;
            }
            if( !isTenthsFound )
            {
                message << "Scaling: '" << myTenths->getElementName() << "' is required but was not found" << std::endl;
            }
            MX_RETURN_IS_SUCCESS;
        }

    }
}
