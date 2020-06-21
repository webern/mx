// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/SystemMargins.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/LeftMargin.h"
#include "mx/core/elements/RightMargin.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        SystemMargins::SystemMargins()
        :myLeftMargin( makeLeftMargin() )
        ,myRightMargin( makeRightMargin() )
        {}


        bool SystemMargins::hasAttributes() const
        {
            return false;
        }


        std::ostream& SystemMargins::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& SystemMargins::streamName( std::ostream& os ) const
        {
            os << "system-margins";
            return os;
        }


        bool SystemMargins::hasContents() const
        {
            return true;
        }


        std::ostream& SystemMargins::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            isOneLineOnly = false;
            os << std::endl;
            myLeftMargin->toStream( os, indentLevel+1 );
            os << std::endl;
            myRightMargin->toStream( os, indentLevel+1 );
            os << std::endl;
            return os;
        }


        LeftMarginPtr SystemMargins::getLeftMargin() const
        {
            return myLeftMargin;
        }


        void SystemMargins::setLeftMargin( const LeftMarginPtr& value )
        {
            if( value )
            {
                myLeftMargin = value;
            }
        }


        RightMarginPtr SystemMargins::getRightMargin() const
        {
            return myRightMargin;
        }


        void SystemMargins::setRightMargin( const RightMarginPtr& value )
        {
            if( value )
            {
                myRightMargin = value;
            }
        }


        bool SystemMargins::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            bool isSuccess = true;
            bool isLeftMarginFound = false;
            bool isRightMarginFound = false;

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if ( importElement( message, *it, isSuccess, *myLeftMargin, isLeftMarginFound ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myRightMargin, isRightMarginFound ) ) { continue; }
            }

            if( !isLeftMarginFound )
            {
                message << "SystemMargins: '" << myLeftMargin->getElementName() << "' is required but was not found" << std::endl;
            }
            if( !isRightMarginFound )
            {
                message << "SystemMargins: '" << myRightMargin->getElementName() << "' is required but was not found" << std::endl;
            }
            return isSuccess;
        }

    }
}
