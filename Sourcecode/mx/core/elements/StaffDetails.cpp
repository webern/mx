// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/StaffDetails.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/Capo.h"
#include "mx/core/elements/StaffLines.h"
#include "mx/core/elements/StaffSize.h"
#include "mx/core/elements/StaffTuning.h"
#include "mx/core/elements/StaffType.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        StaffDetails::StaffDetails()
        :myAttributes( std::make_shared<StaffDetailsAttributes>() )
        ,myStaffType( makeStaffType() )
        ,myHasStaffType( false )
        ,myStaffLines( makeStaffLines( NonNegativeInteger( 5 ) ) )
        ,myHasStaffLines( false )
        ,myStaffTuningSet()
        ,myCapo( makeCapo() )
        ,myHasCapo( false )
        ,myStaffSize( makeStaffSize() )
        ,myHasStaffSize( false )
        {}


        bool StaffDetails::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& StaffDetails::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& StaffDetails::streamName( std::ostream& os ) const
        {
            os << "staff-details";
            return os;
        }


        bool StaffDetails::hasContents() const
        {
            return myHasStaffType
            || myHasStaffLines
            || myStaffTuningSet.size() > 0
            || myHasCapo
            || myHasStaffSize;
        }


        std::ostream& StaffDetails::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            isOneLineOnly = !(   myHasStaffType
                              || myHasStaffLines
                              || myStaffTuningSet.size() > 0
                              || myHasCapo
                              || myHasStaffSize);
            if ( myHasStaffType )
            {
                os << std::endl;
                myStaffType->toStream( os, indentLevel+1 );
            }
            if ( myHasStaffLines )
            {
                os << std::endl;
                myStaffLines->toStream( os, indentLevel+1 );
            }
            for ( auto x : myStaffTuningSet )
            {
                os << std::endl;
                x->toStream( os, indentLevel+1 );
            }
            if ( myHasCapo )
            {
                os << std::endl;
                myCapo->toStream( os, indentLevel+1 );
            }
            if ( myHasStaffSize )
            {
                os << std::endl;
                myStaffSize->toStream( os, indentLevel+1 );
            }
            if( hasContents() )
            {
                os << std::endl;
            }
            return os;
        }


        StaffDetailsAttributesPtr StaffDetails::getAttributes() const
        {
            return myAttributes;
        }


        void StaffDetails::setAttributes( const StaffDetailsAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        StaffTypePtr StaffDetails::getStaffType() const
        {
            return myStaffType;
        }


        void StaffDetails::setStaffType( const StaffTypePtr& value )
        {
            if( value )
            {
                myStaffType = value;
            }
        }


        bool StaffDetails::getHasStaffType() const
        {
            return myHasStaffType;
        }


        void StaffDetails::setHasStaffType( const bool value )
        {
            myHasStaffType = value;
        }


        StaffLinesPtr StaffDetails::getStaffLines() const
        {
            return myStaffLines;
        }


        void StaffDetails::setStaffLines( const StaffLinesPtr& value )
        {
            if( value )
            {
                myStaffLines = value;
            }
        }


        bool StaffDetails::getHasStaffLines() const
        {
            return myHasStaffLines;
        }


        void StaffDetails::setHasStaffLines( const bool value )
        {
            myHasStaffLines = value;
        }


        const StaffTuningSet& StaffDetails::getStaffTuningSet() const
        {
            return myStaffTuningSet;
        }


        void StaffDetails::removeStaffTuning( const StaffTuningSetIterConst& value )
        {
            if ( value != myStaffTuningSet.cend() )
            {
                myStaffTuningSet.erase( value );
            }
        }


        void StaffDetails::addStaffTuning( const StaffTuningPtr& value )
        {
            if ( value )
            {
                myStaffTuningSet.push_back( value );
            }
        }


        void StaffDetails::clearStaffTuningSet()
        {
            myStaffTuningSet.clear();
        }


        CapoPtr StaffDetails::getCapo() const
        {
            return myCapo;
        }


        void StaffDetails::setCapo( const CapoPtr& value )
        {
            if( value )
            {
                myCapo = value;
            }
        }


        bool StaffDetails::getHasCapo() const
        {
            return myHasCapo;
        }


        void StaffDetails::setHasCapo( const bool value )
        {
            myHasCapo = value;
        }


        StaffSizePtr StaffDetails::getStaffSize() const
        {
            return myStaffSize;
        }


        void StaffDetails::setStaffSize( const StaffSizePtr& value )
        {
            if( value )
            {
                myStaffSize = value;
            }
        }


        bool StaffDetails::getHasStaffSize() const
        {
            return myHasStaffSize;
        }


        void StaffDetails::setHasStaffSize( const bool value )
        {
            myHasStaffSize = value;
        }


        bool StaffDetails::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if ( importElement( message, *it, isSuccess, *myStaffType, myHasStaffType ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myStaffLines, myHasStaffLines ) ) { continue; }
                importElementSet( message, it, endIter, isSuccess, "staff-tuning", myStaffTuningSet );
                if ( importElement( message, *it, isSuccess, *myCapo, myHasCapo ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myStaffSize, myHasStaffSize ) ) { continue; }
            }

            MX_RETURN_IS_SUCCESS;
        }

    }
}
