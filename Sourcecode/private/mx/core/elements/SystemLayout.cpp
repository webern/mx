// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/SystemLayout.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/SystemDistance.h"
#include "mx/core/elements/SystemDividers.h"
#include "mx/core/elements/SystemMargins.h"
#include "mx/core/elements/TopSystemDistance.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        SystemLayout::SystemLayout()
        :mySystemMargins( makeSystemMargins() )
        ,myHasSystemMargins( false )
        ,mySystemDistance( makeSystemDistance() )
        ,myHasSystemDistance( false )
        ,myTopSystemDistance( makeTopSystemDistance() )
        ,myHasTopSystemDistance( false )
        ,mySystemDividers( makeSystemDividers() )
        ,myHasSystemDividers( false )
        {}


        bool SystemLayout::hasAttributes() const
        {
            return false;
        }


        std::ostream& SystemLayout::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& SystemLayout::streamName( std::ostream& os ) const
        {
            os << "system-layout";
            return os;
        }


        bool SystemLayout::hasContents() const
        {
            return myHasSystemMargins
            || myHasSystemDistance
            || myHasTopSystemDistance
            || myHasSystemDividers;
        }


        std::ostream& SystemLayout::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            if ( myHasSystemMargins )
            {
                os << std::endl;
                mySystemMargins->toStream( os, indentLevel+1 );
            }
            if ( myHasSystemDistance )
            {
                os << std::endl;
                mySystemDistance->toStream( os, indentLevel+1 );
            }
            if ( myHasTopSystemDistance )
            {
                os << std::endl;
                myTopSystemDistance->toStream( os, indentLevel+1 );
            }
            if ( myHasSystemDividers )
            {
                os << std::endl;
                mySystemDividers->toStream( os, indentLevel+1 );
            }
            isOneLineOnly = ! hasContents();
            if ( ! isOneLineOnly )
            {
                os << std::endl;
            }
            return os;
        }


        SystemMarginsPtr SystemLayout::getSystemMargins() const
        {
            return mySystemMargins;
        }


        void SystemLayout::setSystemMargins( const SystemMarginsPtr& value )
        {
            if( value )
            {
                mySystemMargins = value;
            }
        }


        bool SystemLayout::getHasSystemMargins() const
        {
            return myHasSystemMargins;
        }


        void SystemLayout::setHasSystemMargins( const bool value )
        {
            myHasSystemMargins = value;
        }


        SystemDistancePtr SystemLayout::getSystemDistance() const
        {
            return mySystemDistance;
        }


        void SystemLayout::setSystemDistance( const SystemDistancePtr& value )
        {
            if( value )
            {
                mySystemDistance = value;
            }
        }


        bool SystemLayout::getHasSystemDistance() const
        {
            return myHasSystemDistance;
        }


        void SystemLayout::setHasSystemDistance( const bool value )
        {
            myHasSystemDistance = value;
        }


        TopSystemDistancePtr SystemLayout::getTopSystemDistance() const
        {
            return myTopSystemDistance;
        }


        void SystemLayout::setTopSystemDistance( const TopSystemDistancePtr& value )
        {
            if( value )
            {
                myTopSystemDistance = value;
            }
        }


        bool SystemLayout::getHasTopSystemDistance() const
        {
            return myHasTopSystemDistance;
        }


        void SystemLayout::setHasTopSystemDistance( const bool value )
        {
            myHasTopSystemDistance = value;
        }


        SystemDividersPtr SystemLayout::getSystemDividers() const
        {
            return mySystemDividers;
        }


        void SystemLayout::setSystemDividers( const SystemDividersPtr& value )
        {
            if( value )
            {
                mySystemDividers = value;
            }
        }


        bool SystemLayout::getHasSystemDividers() const
        {
            return myHasSystemDividers;
        }


        void SystemLayout::setHasSystemDividers( const bool value )
        {
            myHasSystemDividers = value;
        }


        bool SystemLayout::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if ( importElement( message, *it, isSuccess, *mySystemMargins, myHasSystemMargins ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *mySystemDistance, myHasSystemDistance ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myTopSystemDistance, myHasTopSystemDistance ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *mySystemDividers, myHasSystemDividers ) ) { continue; }
            }

            MX_RETURN_IS_SUCCESS;
        }

    }
}
