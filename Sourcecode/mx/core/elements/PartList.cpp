// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/PartList.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/PartGroup.h"
#include "mx/core/elements/PartGroupOrScorePart.h"
#include "mx/core/elements/ScorePart.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        PartList::PartList()
        :myPartGroupSet()
        ,myScorePart( makeScorePart() )
        ,myPartGroupOrScorePartSet()
        {}


        bool PartList::hasAttributes() const
        {
            return false;
        }


        std::ostream& PartList::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& PartList::streamName( std::ostream& os ) const
        {
            os << "part-list";
            return os;
        }


        bool PartList::hasContents() const
        {
            return true;
        }


        std::ostream& PartList::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            for ( auto x : myPartGroupSet )
            {
                os << std::endl;
                x->toStream( os, indentLevel+1 );
            }
            os << std::endl;
            myScorePart->toStream( os, indentLevel+1 );
            for ( auto x : myPartGroupOrScorePartSet )
            {
                os << std::endl;
                x->streamContents( os, indentLevel+1, isOneLineOnly );
            }
            isOneLineOnly = false;
            os << std::endl;
            return os;
        }


        const PartGroupSet& PartList::getPartGroupSet() const
        {
            return myPartGroupSet;
        }


        void PartList::addPartGroup( const PartGroupPtr& value )
        {
            if ( value )
            {
                myPartGroupSet.push_back( value );
            }
        }


        void PartList::removePartGroup( const PartGroupSetIterConst& value )
        {
            if ( value != myPartGroupSet.cend() )
            {
                myPartGroupSet.erase( value );
            }
        }


        void PartList::clearPartGroupSet()
        {
            myPartGroupSet.clear();
        }


        PartGroupPtr PartList::getPartGroup( const PartGroupSetIterConst& setIterator ) const
        {
            if( setIterator != myPartGroupSet.cend() )
            {
                return *setIterator;
            }
            return PartGroupPtr();
        }


        ScorePartPtr PartList::getScorePart() const
        {
            return myScorePart;
        }


        void PartList::setScorePart( const ScorePartPtr& value )
        {
            if ( value )
            {
                myScorePart = value;
            }
        }


        const PartGroupOrScorePartSet& PartList::getPartGroupOrScorePartSet() const
        {
            return myPartGroupOrScorePartSet;
        }


        void PartList::addPartGroupOrScorePart( const PartGroupOrScorePartPtr& value )
        {
            if ( value )
            {
                myPartGroupOrScorePartSet.push_back( value );
            }
        }


        void PartList::removePartGroupOrScorePart( const PartGroupOrScorePartSetIterConst& value )
        {
            if ( value != myPartGroupOrScorePartSet.cend() )
            {
                myPartGroupOrScorePartSet.erase( value );
            }
        }


        void PartList::clearPartGroupOrScorePartSet()
        {
            myPartGroupOrScorePartSet.clear();
        }


        PartGroupOrScorePartPtr PartList::getPartGroupOrScorePart( const PartGroupOrScorePartSetIterConst& setIterator ) const
        {
            if( setIterator != myPartGroupOrScorePartSet.cend() )
            {
                return *setIterator;
            }
            return PartGroupOrScorePartPtr();
        }
        
        
        bool PartList::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            bool isFirstScorePartEncountered = false;
            
            for( auto it = xelement.begin(); it != xelement.end(); ++it )
            {
                const std::string elementName = it->getName();
                if( elementName != "part-group" && elementName != "score-part" )
                {
                    message << "PartList: fromXElement encountered unexpected element '" << elementName << "'" << std::endl;
                }
                if( !isFirstScorePartEncountered )
                {
                    if( elementName == "part-group" )
                    {
                        auto partGroup = makePartGroup();
                        isSuccess &= partGroup->fromXElement( message, *it );
                        myPartGroupSet.push_back( partGroup );
                    }
                    else if( elementName == "score-part" )
                    {
                        isFirstScorePartEncountered = true;
                        isSuccess &= myScorePart->fromXElement( message, *it );
                    }
                }
                else
                {
                    auto partGroupOrScorePart = makePartGroupOrScorePart();
                    
                    if( elementName == "part-group" )
                    {
                        partGroupOrScorePart->setChoice( PartGroupOrScorePart::Choice::partGroup );
                        partGroupOrScorePart->getPartGroup()->fromXElement( message, *it );
                    }
                    else if( elementName == "score-part" )
                    {
                        partGroupOrScorePart->setChoice( PartGroupOrScorePart::Choice::scorePart );
                        partGroupOrScorePart->getScorePart()->fromXElement( message, *it );
                    }
                    
                    myPartGroupOrScorePartSet.push_back( partGroupOrScorePart );
                }
            }
            
            if( !isFirstScorePartEncountered )
            {
                message << "PartList: at least one 'score-part' elements are required but none were found" << std::endl;
                isSuccess = false;
            }
            
            MX_RETURN_IS_SUCCESS;
        }

    }
}
