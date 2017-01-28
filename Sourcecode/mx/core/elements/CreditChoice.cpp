// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/CreditChoice.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/CreditImage.h"
#include "mx/core/elements/CreditWords.h"
#include "mx/core/elements/Bookmark.h"
#include "mx/core/elements/Link.h"
#include "mx/core/elements/CreditWordsGroup.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        CreditChoice::CreditChoice()
        :myChoice( Choice::creditWords )
        ,myCreditImage( makeCreditImage() )
        ,myCreditWords( makeCreditWords() )
        ,myCreditWordsGroupSet()
        {}


        bool CreditChoice::hasAttributes() const
        {
            return false;
        }


        std::ostream& CreditChoice::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& CreditChoice::streamName( std::ostream& os ) const
        {
            return os;
        }


        bool CreditChoice::hasContents() const
        {
            return true;
        }


        std::ostream& CreditChoice::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            switch ( myChoice )
            {
                case Choice::creditImage:
                    myCreditImage->toStream( os, indentLevel );
                    break;
                case Choice::creditWords:
                {
                    myCreditWords->toStream( os, indentLevel );
    
                    if ( myCreditWordsGroupSet.size() > 0 )
                    {
                        os << std::endl;
                    }
                    
                    for( auto x : myCreditWordsGroupSet )
                    {
                        x->streamContents( os, indentLevel, isOneLineOnly );
                    }

                    break;
                }
                default:
                    break;
            }
            isOneLineOnly = false;
            return os;
        }


        CreditChoice::Choice CreditChoice::getChoice() const
        {
            return myChoice;
        }


        void CreditChoice::setChoice( const CreditChoice::Choice value )
        {
            myChoice = value;
        }


        CreditImagePtr CreditChoice::getCreditImage() const
        {
            return myCreditImage;
        }


        void CreditChoice::setCreditImage( const CreditImagePtr& value )
        {
            if ( value )
            {
                myCreditImage = value;
            }
        }


        CreditWordsPtr CreditChoice::getCreditWords() const
        {
            return myCreditWords;
        }


        void CreditChoice::setCreditWords( const CreditWordsPtr& value )
        {
            if ( value )
            {
                myCreditWords = value;
            }
        }


        const CreditWordsGroupSet& CreditChoice::getCreditWordsGroupSet() const
        {
            return myCreditWordsGroupSet;
        }


        void CreditChoice::removeCreditWordsGroup( const CreditWordsGroupSetIterConst& value )
        {
            if ( value != myCreditWordsGroupSet.cend() )
            {
                myCreditWordsGroupSet.erase( value );
            }
        }


        void CreditChoice::addCreditWordsGroup( const CreditWordsGroupPtr& value )
        {
            if ( value )
            {
                myCreditWordsGroupSet.push_back( value );
            }
        }


        void CreditChoice::clearCreditWordsGroupSet()
        {
            myCreditWordsGroupSet.clear();
        }


        CreditWordsGroupPtr CreditChoice::getCreditWordsGroup( const CreditWordsGroupSetIterConst& setIterator ) const
        {
            if( setIterator != myCreditWordsGroupSet.cend() )
            {
                return *setIterator;
            }
            return CreditWordsGroupPtr();
        }


        MX_FROM_XELEMENT_UNUSED( CreditChoice );

    }
}
