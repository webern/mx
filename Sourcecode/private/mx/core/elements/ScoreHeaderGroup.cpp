// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/ScoreHeaderGroup.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/Credit.h"
#include "mx/core/elements/Defaults.h"
#include "mx/core/elements/Identification.h"
#include "mx/core/elements/MovementNumber.h"
#include "mx/core/elements/MovementTitle.h"
#include "mx/core/elements/PartList.h"
#include "mx/core/elements/Work.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        ScoreHeaderGroup::ScoreHeaderGroup()
        :myWork( makeWork() )
        ,myHasWork( false )
        ,myMovementNumber( makeMovementNumber() )
        ,myHasMovementNumber( false )
        ,myMovementTitle( makeMovementTitle() )
        ,myHasMovementTitle( false )
        ,myIdentification( makeIdentification() )
        ,myHasIdentification( false )
        ,myDefaults( makeDefaults() )
        ,myHasDefaults( false )
        ,myCreditSet()
        ,myPartList( makePartList() )
        {}


        bool ScoreHeaderGroup::hasAttributes() const
        {
            return false;
        }


        std::ostream& ScoreHeaderGroup::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& ScoreHeaderGroup::streamName( std::ostream& os ) const
        {
            return os;
        }


        bool ScoreHeaderGroup::hasContents() const
        {
            return true;
        }


        std::ostream& ScoreHeaderGroup::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            isOneLineOnly = false;
            bool isFirst = true;
            if ( myHasWork )
            {
                if ( !isFirst )
                {
                    os << std::endl;
                }
                myWork->toStream( os, indentLevel );
                isFirst = false;
            }
            if ( myHasMovementNumber )
            {
                if ( !isFirst )
                {
                    os << std::endl;
                }
                myMovementNumber->toStream( os, indentLevel );
                isFirst = false;
            }
            if ( myHasMovementTitle )
            {
                if ( !isFirst )
                {
                    os << std::endl;
                }
                myMovementTitle->toStream( os, indentLevel );
                isFirst = false;
            }
            if ( myHasIdentification )
            {
                if ( !isFirst )
                {
                    os << std::endl;
                }
                myIdentification->toStream( os, indentLevel );
                isFirst = false;
            }
            if ( myHasDefaults )
            {
                if ( !isFirst )
                {
                    os << std::endl;
                }
                myDefaults->toStream( os, indentLevel );
                isFirst = false;
            }
            for ( auto x : myCreditSet )
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
            myPartList->toStream( os, indentLevel );
            return os;
        }


        WorkPtr ScoreHeaderGroup::getWork() const
        {
            return myWork;
        }


        void ScoreHeaderGroup::setWork( const WorkPtr& value )
        {
            if ( value )
            {
                myWork = value;
            }
        }


        bool ScoreHeaderGroup::getHasWork() const
        {
            return myHasWork;
        }


        void ScoreHeaderGroup::setHasWork( const bool value )
        {
            myHasWork = value;
        }


        MovementNumberPtr ScoreHeaderGroup::getMovementNumber() const
        {
            return myMovementNumber;
        }


        void ScoreHeaderGroup::setMovementNumber( const MovementNumberPtr& value )
        {
            if ( value )
            {
                myMovementNumber = value;
            }
        }


        bool ScoreHeaderGroup::getHasMovementNumber() const
        {
            return myHasMovementNumber;
        }


        void ScoreHeaderGroup::setHasMovementNumber( const bool value )
        {
            myHasMovementNumber = value;
        }


        MovementTitlePtr ScoreHeaderGroup::getMovementTitle() const
        {
            return myMovementTitle;
        }


        void ScoreHeaderGroup::setMovementTitle( const MovementTitlePtr& value )
        {
            if ( value )
            {
                myMovementTitle = value;
            }
        }


        bool ScoreHeaderGroup::getHasMovementTitle() const
        {
            return myHasMovementTitle;
        }


        void ScoreHeaderGroup::setHasMovementTitle( const bool value )
        {
            myHasMovementTitle = value;
        }


        IdentificationPtr ScoreHeaderGroup::getIdentification() const
        {
            return myIdentification;
        }


        void ScoreHeaderGroup::setIdentification( const IdentificationPtr& value )
        {
            if ( value )
            {
                myIdentification = value;
            }
        }


        bool ScoreHeaderGroup::getHasIdentification() const
        {
            return myHasIdentification;
        }


        void ScoreHeaderGroup::setHasIdentification( const bool value )
        {
            myHasIdentification = value;
        }


        DefaultsPtr ScoreHeaderGroup::getDefaults() const
        {
            return myDefaults;
        }


        void ScoreHeaderGroup::setDefaults( const DefaultsPtr& value )
        {
            if ( value )
            {
                myDefaults = value;
            }
        }


        bool ScoreHeaderGroup::getHasDefaults() const
        {
            return myHasDefaults;
        }


        void ScoreHeaderGroup::setHasDefaults( const bool value )
        {
            myHasDefaults = value;
        }


        const CreditSet& ScoreHeaderGroup::getCreditSet() const
        {
            return myCreditSet;
        }


        void ScoreHeaderGroup::addCredit( const CreditPtr& value )
        {
            if ( value )
            {
                myCreditSet.push_back( value );
            }
        }


        void ScoreHeaderGroup::removeCredit( const CreditSetIterConst& value )
        {
            if ( value != myCreditSet.cend() )
            {
                myCreditSet.erase( value );
            }
        }


        void ScoreHeaderGroup::clearCreditSet()
        {
            myCreditSet.clear();
        }


        CreditPtr ScoreHeaderGroup::getCredit( const CreditSetIterConst& setIterator ) const
        {
            if( setIterator != myCreditSet.cend() )
            {
                return *setIterator;
            }
            return CreditPtr();
        }


        PartListPtr ScoreHeaderGroup::getPartList() const
        {
            return myPartList;
        }


        void ScoreHeaderGroup::setPartList( const PartListPtr& value )
        {
            if ( value )
            {
                myPartList = value;
            }
        }
        
        
        bool ScoreHeaderGroup::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            bool isSuccess = true;
            bool isPartListFound = false;
            bool isFirstCreditAdded = false;
            for( auto it = xelement.begin(); it != xelement.end(); ++it )
            {
                const std::string elementName = it->getName();
                
                if( elementName == "work" )
                {
                    myHasWork = true;
                    isSuccess &= myWork->fromXElement( message, *it );
                }
                else if( elementName == "movement-number" )
                {
                    myHasMovementNumber = true;
                    isSuccess &= myMovementNumber->fromXElement( message, *it );
                }
                else if( elementName == "movement-title" )
                {
                    myHasMovementTitle = true;
                    isSuccess &= myMovementTitle->fromXElement( message, *it );
                }
                else if( elementName == "identification" )
                {
                    myHasIdentification = true;
                    isSuccess &= myIdentification->fromXElement( message, *it );
                }
                else if( elementName == "defaults" )
                {
                    myHasDefaults = true;
                    isSuccess &= myDefaults->fromXElement( message, *it );
                }
                else if( elementName == "credit" )
                {
                    auto credit = makeCredit();
                    isSuccess &= credit->fromXElement( message, *it );
                    
                    if( !isFirstCreditAdded && myCreditSet.size() == 1 )
                    {
                        *( myCreditSet.begin() ) = credit;
                        isFirstCreditAdded = true;
                    }
                    else
                    {
                        myCreditSet.push_back( credit );
                        isFirstCreditAdded = true;
                    }

                }
                else if( elementName == "part-list" )
                {
                    isPartListFound = true;
                    isSuccess &= myPartList->fromXElement( message, *it );
                }
                else
                {
                    if( !isPartListFound )
                    {
                        message << "ScoreHeaderGroup: a 'part-list' element is required but was not found" << std::endl;
                        return false;
                    }
                }
                
            }
            
            MX_RETURN_IS_SUCCESS;
        }

    }
}
