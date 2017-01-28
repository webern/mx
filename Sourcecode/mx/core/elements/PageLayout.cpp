// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/PageLayout.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/PageHeight.h"
#include "mx/core/elements/PageMargins.h"
#include "mx/core/elements/PageWidth.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        PageLayout::PageLayout()
        :myPageHeight( makePageHeight() )
        ,myPageWidth( makePageWidth() )
        ,myPageMarginsSet()
        {}


        bool PageLayout::hasAttributes() const
        {
            return false;
        }


        std::ostream& PageLayout::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& PageLayout::streamName( std::ostream& os ) const
        {
            os << "page-layout";
            return os;
        }


        bool PageLayout::hasContents() const
        {
            return true;
        }


        std::ostream& PageLayout::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            os << std::endl;
            myPageHeight->toStream( os, indentLevel+1 );
            os << std::endl;
            myPageWidth->toStream( os, indentLevel+1 );
            for ( auto x : myPageMarginsSet )
            {
                os << std::endl;
                x->toStream( os, indentLevel+1 );
            }
            os << std::endl;
            isOneLineOnly = false;
            return os;
        }


        PageHeightPtr PageLayout::getPageHeight() const
        {
            return myPageHeight;
        }


        void PageLayout::setPageHeight( const PageHeightPtr& value )
        {
            if( value )
            {
                myPageHeight = value;
            }
        }


        PageWidthPtr PageLayout::getPageWidth() const
        {
            return myPageWidth;
        }


        void PageLayout::setPageWidth( const PageWidthPtr& value )
        {
            if( value )
            {
                myPageWidth = value;
            }
        }


        const PageMarginsSet& PageLayout::getPageMarginsSet() const
        {
            return myPageMarginsSet;
        }


        void PageLayout::removePageMargins( const PageMarginsSetIterConst& value )
        {
            if ( value != myPageMarginsSet.cend() )
            {
                myPageMarginsSet.erase( value );
            }
        }


        void PageLayout::addPageMargins( const PageMarginsPtr& value )
        {
            if ( value )
            {
                if( myPageMarginsSet.size() < 2 )
                {
                    myPageMarginsSet.push_back( value );
                }
            }
        }


        void PageLayout::clearPageMarginsSet()
        {
            myPageMarginsSet.clear();
        }


        PageMarginsPtr PageLayout::getPageMargins( const PageMarginsSetIterConst& setIterator ) const
        {
            if( setIterator != myPageMarginsSet.cend() )
            {
                return *setIterator;
            }
            return PageMarginsPtr();
        }


        bool PageLayout::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            bool isPageHeightFound = false;
            bool isPageWidthFound = false;

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if ( importElement( message, *it, isSuccess, *myPageHeight, isPageHeightFound ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myPageWidth, isPageWidthFound ) ) { continue; }
                importElementSet( message, it, endIter, isSuccess, "page-margins", myPageMarginsSet );
            }

            if( !isPageHeightFound )
            {
                message << "PageLayout: '" << myPageHeight->getElementName() << "' is required but was not found" << std::endl;
            }
            if( !isPageWidthFound )
            {
                message << "PageLayout: '" << myPageWidth->getElementName() << "' is required but was not found" << std::endl;
            }
            MX_RETURN_IS_SUCCESS;
        }

    }
}
