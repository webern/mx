// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Forward.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/Duration.h"
#include "mx/core/elements/EditorialVoiceGroup.h"
#include "mx/core/elements/Staff.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Forward::Forward()
        :myDuration( makeDuration() )
        ,myEditorialVoiceGroup( makeEditorialVoiceGroup() )
        ,myStaff( makeStaff() )
        ,myHasStaff( false )
        {}


        bool Forward::hasAttributes() const
        {
            return false;
        }


        std::ostream& Forward::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& Forward::streamName( std::ostream& os ) const
        {
            os << "forward";
            return os;
        }


        bool Forward::hasContents() const
        {
            return true;
        }


        std::ostream& Forward::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            os << std::endl;
            myDuration->toStream( os, indentLevel+1 );
            if ( myEditorialVoiceGroup->hasContents() )
            {
                os << std::endl;
                myEditorialVoiceGroup->streamContents(os, indentLevel+1, isOneLineOnly );
            }
            if ( myHasStaff )
            {
                os << std::endl;
                myStaff->toStream( os, indentLevel+1 );
            }
            isOneLineOnly = false;
            os << std::endl;
            return os;
        }


        DurationPtr Forward::getDuration() const
        {
            return myDuration;
        }


        void Forward::setDuration( const DurationPtr& value )
        {
            if( value )
            {
                myDuration = value;
            }
        }


        EditorialVoiceGroupPtr Forward::getEditorialVoiceGroup() const
        {
            return myEditorialVoiceGroup;
        }


        void Forward::setEditorialVoiceGroup( const EditorialVoiceGroupPtr& value )
        {
            if ( value )
            {
                myEditorialVoiceGroup = value;
            }
        }


        StaffPtr Forward::getStaff() const
        {
            return myStaff;
        }


        void Forward::setStaff( const StaffPtr& value )
        {
            if( value )
            {
                myStaff = value;
            }
        }


        bool Forward::getHasStaff() const
        {
            return myHasStaff;
        }


        void Forward::setHasStaff( const bool value )
        {
            myHasStaff = value;
        }


        bool Forward::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            bool isDurationFound = false;

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if ( importElement( message, *it, isSuccess, *myDuration, isDurationFound ) ) { continue; }
                importGroup( message, it, endIter, isSuccess, myEditorialVoiceGroup );
                if ( importElement( message, *it, isSuccess, *myStaff, myHasStaff ) ) { continue; }
            }

            MX_RETURN_IS_SUCCESS;
        }

    }
}
