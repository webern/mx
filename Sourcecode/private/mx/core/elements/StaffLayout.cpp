// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/StaffLayout.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/StaffDistance.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        StaffLayout::StaffLayout()
        :myAttributes( std::make_shared<StaffLayoutAttributes>() )
        ,myStaffDistance( makeStaffDistance() )
        ,myHasStaffDistance( false )
        {}


        bool StaffLayout::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& StaffLayout::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& StaffLayout::streamName( std::ostream& os ) const
        {
            os << "staff-layout";
            return os;
        }


        bool StaffLayout::hasContents() const
        {
            return myHasStaffDistance;
        }


        std::ostream& StaffLayout::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            if ( myHasStaffDistance )
            {
                os << std::endl;
                myStaffDistance->toStream( os, indentLevel+1 );
            }
            isOneLineOnly = ! hasContents();
            if ( ! isOneLineOnly )
            {
                os << std::endl;
            }
            return os;
        }


        StaffLayoutAttributesPtr StaffLayout::getAttributes() const
        {
            return myAttributes;
        }


        void StaffLayout::setAttributes( const StaffLayoutAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        StaffDistancePtr StaffLayout::getStaffDistance() const
        {
            return myStaffDistance;
        }


        void StaffLayout::setStaffDistance( const StaffDistancePtr& value )
        {
            if( value )
            {
                myStaffDistance = value;
            }
        }


        bool StaffLayout::getHasStaffDistance() const
        {
            return myHasStaffDistance;
        }


        void StaffLayout::setHasStaffDistance( const bool value )
        {
            myHasStaffDistance = value;
        }


        bool StaffLayout::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if ( importElement( message, *it, isSuccess, *myStaffDistance, myHasStaffDistance ) ) { continue; }
            }

            MX_RETURN_IS_SUCCESS;
        }

    }
}
