// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/PageMargins.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/BottomMargin.h"
#include "mx/core/elements/LeftMargin.h"
#include "mx/core/elements/RightMargin.h"
#include "mx/core/elements/TopMargin.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        PageMargins::PageMargins()
        :myAttributes( std::make_shared<PageMarginsAttributes>() )
        ,myLeftMargin( makeLeftMargin() )
        ,myRightMargin( makeRightMargin() )
        ,myTopMargin( makeTopMargin() )
        ,myBottomMargin( makeBottomMargin() )
        {}


        bool PageMargins::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& PageMargins::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& PageMargins::streamName( std::ostream& os ) const
        {
            os << "page-margins";
            return os;
        }


        bool PageMargins::hasContents() const
        {
            return true;
        }


        std::ostream& PageMargins::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            isOneLineOnly = false;
            os << std::endl;
            myLeftMargin->toStream( os, indentLevel+1 );
            os << std::endl;
            myRightMargin->toStream( os, indentLevel+1 );
            os << std::endl;
            myTopMargin->toStream( os, indentLevel+1 );
            os << std::endl;
            myBottomMargin->toStream( os, indentLevel+1 );
            os << std::endl;
            return os;
        }


        PageMarginsAttributesPtr PageMargins::getAttributes() const
        {
            return myAttributes;
        }


        void PageMargins::setAttributes( const PageMarginsAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        LeftMarginPtr PageMargins::getLeftMargin() const
        {
            return myLeftMargin;
        }


        void PageMargins::setLeftMargin( const LeftMarginPtr& value )
        {
            if( value )
            {
                myLeftMargin = value;
            }
        }


        RightMarginPtr PageMargins::getRightMargin() const
        {
            return myRightMargin;
        }


        void PageMargins::setRightMargin( const RightMarginPtr& value )
        {
            if( value )
            {
                myRightMargin = value;
            }
        }


        TopMarginPtr PageMargins::getTopMargin() const
        {
            return myTopMargin;
        }


        void PageMargins::setTopMargin( const TopMarginPtr& value )
        {
            if( value )
            {
                myTopMargin = value;
            }
        }


        BottomMarginPtr PageMargins::getBottomMargin() const
        {
            return myBottomMargin;
        }


        void PageMargins::setBottomMargin( const BottomMarginPtr& value )
        {
            if( value )
            {
                myBottomMargin = value;
            }
        }


        bool PageMargins::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            bool isLeftMarginFound = false;
            bool isRightMarginFound = false;
            bool isTopMarginFound = false;
            bool isBottomMarginFound = false;

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if ( importElement( message, *it, isSuccess, *myLeftMargin, isLeftMarginFound ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myRightMargin, isRightMarginFound ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myTopMargin, isTopMarginFound ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myBottomMargin, isBottomMarginFound ) ) { continue; }
            }

            if( !isLeftMarginFound )
            {
                message << "PageMargins: '" << myLeftMargin->getElementName() << "' is required but was not found" << std::endl;
            }
            if( !isRightMarginFound )
            {
                message << "PageMargins: '" << myRightMargin->getElementName() << "' is required but was not found" << std::endl;
            }
            if( !isTopMarginFound )
            {
                message << "PageMargins: '" << myTopMargin->getElementName() << "' is required but was not found" << std::endl;
            }
            if( !isBottomMarginFound )
            {
                message << "PageMargins: '" << myBottomMargin->getElementName() << "' is required but was not found" << std::endl;
            }
            MX_RETURN_IS_SUCCESS;
        }

    }
}
