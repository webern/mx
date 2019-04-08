// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/SystemDividers.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/LeftDivider.h"
#include "mx/core/elements/RightDivider.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        SystemDividers::SystemDividers()
        :myLeftDivider( makeLeftDivider() )
        ,myRightDivider( makeRightDivider() )
        {}


        bool SystemDividers::hasAttributes() const
        {
            return false;
        }


        std::ostream& SystemDividers::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& SystemDividers::streamName( std::ostream& os ) const
        {
            os << "system-dividers";
            return os;
        }


        bool SystemDividers::hasContents() const
        {
            return true;
        }


        std::ostream& SystemDividers::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            isOneLineOnly = false;
            os << std::endl;
            myLeftDivider->toStream( os, indentLevel+1 );
            os << std::endl;
            myRightDivider->toStream( os, indentLevel+1 );
            os << std::endl;
            return os;
        }


        LeftDividerPtr SystemDividers::getLeftDivider() const
        {
            return myLeftDivider;
        }


        void SystemDividers::setLeftDivider( const LeftDividerPtr& value )
        {
            if( value )
            {
                myLeftDivider = value;
            }
        }


        RightDividerPtr SystemDividers::getRightDivider() const
        {
            return myRightDivider;
        }


        void SystemDividers::setRightDivider( const RightDividerPtr& value )
        {
            if( value )
            {
                myRightDivider = value;
            }
        }


        bool SystemDividers::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            bool isLeftDividerFound = false;
            bool isRightDividerFound = false;

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if ( importElement( message, *it, isSuccess, *myLeftDivider, isLeftDividerFound ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myRightDivider, isRightDividerFound ) ) { continue; }
            }

            if( !isLeftDividerFound )
            {
                message << "SystemDividers: '" << myLeftDivider->getElementName() << "' is required but was not found" << std::endl;
            }
            if( !isRightDividerFound )
            {
                message << "SystemDividers: '" << myRightDivider->getElementName() << "' is required but was not found" << std::endl;
            }
            MX_RETURN_IS_SUCCESS;
        }

    }
}
