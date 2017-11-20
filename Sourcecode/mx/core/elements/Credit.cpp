// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Credit.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/Bookmark.h"
#include "mx/core/elements/CreditChoice.h"
#include "mx/core/elements/CreditImage.h"
#include "mx/core/elements/CreditType.h"
#include "mx/core/elements/CreditWords.h"
#include "mx/core/elements/CreditWordsGroup.h"
#include "mx/core/elements/Link.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Credit::Credit()
        :myAttributes( std::make_shared<CreditAttributes>() )
        ,myCreditTypeSet()
        ,myLinkSet()
        ,myBookmarkSet()
        ,myCreditChoice( makeCreditChoice() )
        {}


        bool Credit::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& Credit::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& Credit::streamName( std::ostream& os ) const
        {
            os << "credit";
            return os;
        }


        bool Credit::hasContents() const
        {
            return true;
        }


        std::ostream& Credit::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            for( auto x : myCreditTypeSet )
            {
                os << std::endl;
                x->toStream( os, indentLevel+1 );
            }
            for( auto x : myLinkSet )
            {
                os << std::endl;
                x->toStream( os, indentLevel+1 );
            }
            for( auto x : myBookmarkSet )
            {
                os << std::endl;
                x->toStream( os, indentLevel+1 );
            }
            os << std::endl;
            myCreditChoice->streamContents( os, indentLevel+1, isOneLineOnly );
            os << std::endl;
            isOneLineOnly = false;
            return os;
        }


        CreditAttributesPtr Credit::getAttributes() const
        {
            return myAttributes;
        }


        void Credit::setAttributes( const CreditAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        const CreditTypeSet& Credit::getCreditTypeSet() const
        {
            return myCreditTypeSet;
        }


        void Credit::removeCreditType( const CreditTypeSetIterConst& value )
        {
            if ( value != myCreditTypeSet.cend() )
            {
                myCreditTypeSet.erase( value );
            }
        }


        void Credit::addCreditType( const CreditTypePtr& value )
        {
            if ( value )
            {
                myCreditTypeSet.push_back( value );
            }
        }


        void Credit::clearCreditTypeSet()
        {
            myCreditTypeSet.clear();
        }


        CreditTypePtr Credit::getCreditType( const CreditTypeSetIterConst& setIterator ) const
        {
            if( setIterator != myCreditTypeSet.cend() )
            {
                return *setIterator;
            }
            return CreditTypePtr();
        }


        const LinkSet& Credit::getLinkSet() const
        {
            return myLinkSet;
        }


        void Credit::removeLink( const LinkSetIterConst& value )
        {
            if ( value != myLinkSet.cend() )
            {
                myLinkSet.erase( value );
            }
        }


        void Credit::addLink( const LinkPtr& value )
        {
            if ( value )
            {
                myLinkSet.push_back( value );
            }
        }


        void Credit::clearLinkSet()
        {
            myLinkSet.clear();
        }


        LinkPtr Credit::getLink( const LinkSetIterConst& setIterator ) const
        {
            if( setIterator != myLinkSet.cend() )
            {
                return *setIterator;
            }
            return LinkPtr();
        }


        const BookmarkSet& Credit::getBookmarkSet() const
        {
            return myBookmarkSet;
        }


        void Credit::removeBookmark( const BookmarkSetIterConst& value )
        {
            if ( value != myBookmarkSet.cend() )
            {
                myBookmarkSet.erase( value );
            }
        }


        void Credit::addBookmark( const BookmarkPtr& value )
        {
            if ( value )
            {
                myBookmarkSet.push_back( value );
            }
        }


        void Credit::clearBookmarkSet()
        {
            myBookmarkSet.clear();
        }


        BookmarkPtr Credit::getBookmark( const BookmarkSetIterConst& setIterator ) const
        {
            if( setIterator != myBookmarkSet.cend() )
            {
                return *setIterator;
            }
            return BookmarkPtr();
        }


        CreditChoicePtr Credit::getCreditChoice() const
        {
            return myCreditChoice;
        }


        void Credit::setCreditChoice( const CreditChoicePtr& value )
        {
            if( value )
            {
                myCreditChoice = value;
            }
        }
        
#ifndef MX_CREDIT_RETURN_IF_END
#define MX_CREDIT_RETURN_IF_END \
        if( i == endIter ) \
        { \
            if( !isCreditImageOrCreditWordsFound ) \
            { \
                message << "Credit: neither 'credit-words' nor 'credit-image' was present - one of these is required" << std::endl; \
                isSuccess = false; \
            } \
            return isSuccess; \
        }
#endif
        
        bool Credit::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            bool isCreditImageOrCreditWordsFound = false;
            
            xml::XElementIterator i = xelement.begin();
            xml::XElementIterator endIter = xelement.end();
            MX_CREDIT_RETURN_IF_END;
            
            if( i->getName() == "credit-type" )
            {
                while( i != endIter && i->getName() == "credit-type" )
                {
                    auto item = makeCreditType();
                    isSuccess &= item->fromXElement( message, *i );
                    addCreditType( item );
                    ++i;
                }
            }
            MX_CREDIT_RETURN_IF_END;
            
            if( i->getName() == "link" )
            {
                while( i != endIter && i->getName() == "link" )
                {
                    auto item = makeLink();
                    isSuccess &= item->fromXElement( message, *i );
                    addLink( item );
                    ++i;
                }
            }
            MX_CREDIT_RETURN_IF_END;
            
            if( i->getName() == "bookmark" )
            {
                while( i != endIter && i->getName() == "bookmark" )
                {
                    auto item = makeBookmark();
                    isSuccess &= item->fromXElement( message, *i );
                    addBookmark( item );
                    ++i;
                }
            }
            MX_CREDIT_RETURN_IF_END;
            
            if( i->getName() == "credit-image" || i->getName() == "credit-words" )
            {
                isCreditImageOrCreditWordsFound = true;
                if( i->getName() == "credit-image" )
                {
                    myCreditChoice->setChoice( CreditChoice::Choice::creditImage );
                    isSuccess &= myCreditChoice->getCreditImage()->fromXElement( message, *i );
                    MX_RETURN_IS_SUCCESS;
                }
                
                if( i->getName() == "credit-words" )
                {
                    myCreditChoice->setChoice( CreditChoice::Choice::creditWords );
                    isSuccess &= myCreditChoice->getCreditWords()->fromXElement( message, *i );
                    ++i;
                    MX_CREDIT_RETURN_IF_END;
                    
                    auto creditWordsGroup = makeCreditWordsGroup();
                    while( i != endIter && ((i->getName() == "link")||(i->getName() == "bookmark")||(i->getName() == "credit-words" )))
                    {
                        while( i != endIter && i->getName() == "link" )
                        {
                            auto link = makeLink();
                            isSuccess &= link->fromXElement( message, xelement );
                            creditWordsGroup->addLink( link );
                            ++i;
                        }
                        MX_CREDIT_RETURN_IF_END;
                        
                        while( i != endIter && i->getName() == "bookmark" )
                        {
                            auto bookmark = makeBookmark();
                            isSuccess &= bookmark->fromXElement( message, xelement );
                            creditWordsGroup->addBookmark( bookmark );
                            ++i;
                        }
                        MX_CREDIT_RETURN_IF_END;
                        
                        if( i->getName() == "credit-words" )
                        {
                            isSuccess &= creditWordsGroup->getCreditWords()->fromXElement( message, *i );
                            myCreditChoice->addCreditWordsGroup( creditWordsGroup );
                            creditWordsGroup = makeCreditWordsGroup();
                            ++i;
                        }
                        MX_CREDIT_RETURN_IF_END;
                    }
                }
            }
            MX_RETURN_IS_SUCCESS;
        }

    }
}
