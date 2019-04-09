// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Backup.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/Duration.h"
#include "mx/core/elements/EditorialGroup.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Backup::Backup()
        :myDuration( makeDuration() )
        ,myEditorialGroup( makeEditorialGroup() )
        {}


        bool Backup::hasAttributes() const
        {
            return false;
        }


        std::ostream& Backup::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& Backup::streamName( std::ostream& os ) const
        {
            os << "backup";
            return os;
        }


        bool Backup::hasContents() const
        {
            return true;
        }


        std::ostream& Backup::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            os << std::endl;
            myDuration->toStream( os, indentLevel+1 );
            os << std::endl;
            myEditorialGroup->streamContents( os, indentLevel+1, isOneLineOnly );
            if ( myEditorialGroup->hasContents() )
            {
                os << std::endl;
            }
            isOneLineOnly = false;
            return os;
        }


        DurationPtr Backup::getDuration() const
        {
            return myDuration;
        }


        void Backup::setDuration( const DurationPtr& value )
        {
            if( value )
            {
                myDuration = value;
            }
        }


        EditorialGroupPtr Backup::getEditorialGroup() const
        {
            return myEditorialGroup;
        }


        void Backup::setEditorialGroup( const EditorialGroupPtr& value )
        {
            if ( value )
            {
                myEditorialGroup = value;
            }
        }


        bool Backup::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            bool isDurationFound = false;

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if ( importElement( message, *it, isSuccess, *myDuration, isDurationFound ) ) { continue; }
                importGroup( message, it, endIter, isSuccess, myEditorialGroup );
            }

            MX_RETURN_IS_SUCCESS;
        }

    }
}
