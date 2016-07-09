// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/Work.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/Opus.h"
#include "mx/core/elements/WorkNumber.h"
#include "mx/core/elements/WorkTitle.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Work::Work()
        :myWorkNumber( makeWorkNumber() )
        ,myHasWorkNumber( false )
        ,myWorkTitle( makeWorkTitle() )
        ,myHasWorkTitle( false )
        ,myOpus( makeOpus() )
        ,myHasOpus( false )
        {}


        bool Work::hasAttributes() const
        {
            return false;
        }


        std::ostream& Work::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& Work::streamName( std::ostream& os ) const
        {
            os << "work";
            return os;
        }


        bool Work::hasContents() const
        {
            return myHasWorkNumber || myHasWorkTitle || myHasOpus;
        }


        std::ostream& Work::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            if ( myHasWorkNumber )
            {
                os << std::endl;
                myWorkNumber->toStream( os, indentLevel+1 );
            }
            if ( myHasWorkTitle )
            {
                os << std::endl;
                myWorkTitle->toStream( os, indentLevel+1 );
            }
            if ( myHasOpus )
            {
                os << std::endl;
                myOpus->toStream( os, indentLevel+1 );
            }
            isOneLineOnly = ! hasContents();
            if ( ! isOneLineOnly )
            {
                os << std::endl;
            }
            return os;
        }


        WorkNumberPtr Work::getWorkNumber() const
        {
            return myWorkNumber;
        }


        void Work::setWorkNumber( const WorkNumberPtr& value )
        {
            if( value )
            {
                myWorkNumber = value;
            }
        }


        bool Work::getHasWorkNumber() const
        {
            return myHasWorkNumber;
        }


        void Work::setHasWorkNumber( const bool value )
        {
            myHasWorkNumber = value;
        }


        WorkTitlePtr Work::getWorkTitle() const
        {
            return myWorkTitle;
        }


        void Work::setWorkTitle( const WorkTitlePtr& value )
        {
            if( value )
            {
                myWorkTitle = value;
            }
        }


        bool Work::getHasWorkTitle() const
        {
            return myHasWorkTitle;
        }


        void Work::setHasWorkTitle( const bool value )
        {
            myHasWorkTitle = value;
        }


        OpusPtr Work::getOpus() const
        {
            return myOpus;
        }


        void Work::setOpus( const OpusPtr& value )
        {
            if( value )
            {
                myOpus = value;
            }
        }


        bool Work::getHasOpus() const
        {
            return myHasOpus;
        }


        void Work::setHasOpus( const bool value )
        {
            myHasOpus = value;
        }


        bool Work::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if ( importElement( message, *it, isSuccess, *myWorkNumber, myHasWorkNumber ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myWorkTitle, myHasWorkTitle ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myOpus, myHasOpus ) ) { continue; }
            }

            MX_RETURN_IS_SUCCESS;
        }

    }
}
