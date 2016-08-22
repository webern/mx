// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/CreditWordsGroup.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/Bookmark.h"
#include "mx/core/elements/CreditWords.h"
#include "mx/core/elements/Link.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        CreditWordsGroup::CreditWordsGroup()
        :myLinkSet()
        ,myBookmarkSet()
        ,myCreditWords( makeCreditWords() )
        {}


        bool CreditWordsGroup::hasAttributes() const
        {
            return false;
        }


        std::ostream& CreditWordsGroup::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& CreditWordsGroup::streamName( std::ostream& os ) const
        {
            return os;
        }


        bool CreditWordsGroup::hasContents() const
        {
            return true;
        }


        std::ostream& CreditWordsGroup::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            bool isFirst = true;
            for ( auto x : myLinkSet )
            {
                if ( !isFirst )
                {
                    os << std::endl;
                }
                x->toStream( os, indentLevel );
                isFirst = false;
            }
            for ( auto x : myBookmarkSet )
            {
                if ( !isFirst )
                {
                    os << std::endl;
                }
                x->toStream( os, indentLevel );
                isFirst = false;
            }
            if ( !isFirst )
            {
                os << std::endl;
            }
            myCreditWords->toStream( os, indentLevel );
            isOneLineOnly = false;
            return os;
        }


        const LinkSet& CreditWordsGroup::getLinkSet() const
        {
            return myLinkSet;
        }


        void CreditWordsGroup::addLink( const LinkPtr& value )
        {
            if ( value )
            {
                myLinkSet.push_back( value );
            }
        }


        void CreditWordsGroup::removeLink( const LinkSetIterConst& value )
        {
            if ( value != myLinkSet.cend() )
            {
                myLinkSet.erase( value );
            }
        }


        void CreditWordsGroup::clearLinkSet()
        {
            myLinkSet.clear();
        }


        LinkPtr CreditWordsGroup::getLink( const LinkSetIterConst& setIterator ) const
        {
            if( setIterator != myLinkSet.cend() )
            {
                return *setIterator;
            }
            return LinkPtr();
        }


        const BookmarkSet& CreditWordsGroup::getBookmarkSet() const
        {
            return myBookmarkSet;
        }


        void CreditWordsGroup::addBookmark( const BookmarkPtr& value )
        {
            if ( value )
            {
                myBookmarkSet.push_back( value );
            }
        }


        void CreditWordsGroup::removeBookmark( const BookmarkSetIterConst& value )
        {
            if ( value != myBookmarkSet.cend() )
            {
                myBookmarkSet.erase( value );
            }
        }


        void CreditWordsGroup::clearBookmarkSet()
        {
            myBookmarkSet.clear();
        }


        BookmarkPtr CreditWordsGroup::getBookmark( const BookmarkSetIterConst& setIterator ) const
        {
            if( setIterator != myBookmarkSet.cend() )
            {
                return *setIterator;
            }
            return BookmarkPtr();
        }


        CreditWordsPtr CreditWordsGroup::getCreditWords() const
        {
            return myCreditWords;
        }


        void CreditWordsGroup::setCreditWords( const CreditWordsPtr& value )
        {
            if ( value )
            {
                myCreditWords = value;
            }
        }
        
        
        MX_FROM_XELEMENT_UNUSED( CreditWordsGroup );

    }
}
