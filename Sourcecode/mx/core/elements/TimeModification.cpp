// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/TimeModification.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/ActualNotes.h"
#include "mx/core/elements/NormalNotes.h"
#include "mx/core/elements/NormalType.h"
#include "mx/core/elements/NormalDot.h"
#include "mx/core/elements/NormalTypeNormalDotGroup.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        TimeModification::TimeModification()
        :myActualNotes( makeActualNotes() )
        ,myNormalNotes( makeNormalNotes() )
        ,myNormalTypeNormalDotGroup( makeNormalTypeNormalDotGroup() )
        ,myHasNormalTypeNormalDotGroup( false )
        {}


        bool TimeModification::hasAttributes() const
        {
            return false;
        }


        std::ostream& TimeModification::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& TimeModification::streamName( std::ostream& os ) const
        {
            os << "time-modification";
            return os;
        }


        bool TimeModification::hasContents() const
        {
            return true;
        }


        std::ostream& TimeModification::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            os << std::endl;
            myActualNotes->toStream( os, indentLevel+1 );
            os << std::endl;
            myNormalNotes->toStream( os, indentLevel+1 );
            if ( myHasNormalTypeNormalDotGroup )
            {
                os << std::endl;
                myNormalTypeNormalDotGroup->streamContents( os, indentLevel+1, isOneLineOnly );
            }
            os << std::endl;
            isOneLineOnly = false;
            return os;
        }


        ActualNotesPtr TimeModification::getActualNotes() const
        {
            return myActualNotes;
        }


        void TimeModification::setActualNotes( const ActualNotesPtr& value )
        {
            if( value )
            {
                myActualNotes = value;
            }
        }


        NormalNotesPtr TimeModification::getNormalNotes() const
        {
            return myNormalNotes;
        }


        void TimeModification::setNormalNotes( const NormalNotesPtr& value )
        {
            if( value )
            {
                myNormalNotes = value;
            }
        }


        NormalTypeNormalDotGroupPtr TimeModification::getNormalTypeNormalDotGroup() const
        {
            return myNormalTypeNormalDotGroup;
        }


        void TimeModification::setNormalTypeNormalDotGroup( const NormalTypeNormalDotGroupPtr& value )
        {
            if( value )
            {
                myNormalTypeNormalDotGroup = value;
            }
        }


        bool TimeModification::getHasNormalTypeNormalDotGroup() const
        {
            return myHasNormalTypeNormalDotGroup;
        }


        void TimeModification::setHasNormalTypeNormalDotGroup( const bool value )
        {
            myHasNormalTypeNormalDotGroup = value;
        }


        bool TimeModification::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            bool isActualNotesFound = false;
            bool isNormalNotesFound = false;

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if ( importElement( message, *it, isSuccess, *myActualNotes, isActualNotesFound ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myNormalNotes, isNormalNotesFound ) ) { continue; }
                if( it->getName() == "normal-type" )
                {
                    myHasNormalTypeNormalDotGroup = true;
                    isSuccess = myNormalTypeNormalDotGroup->getNormalType()->fromXElement( message, *it );
                }
                
                if( it->getName() == "normal-dot" )
                {
                    myHasNormalTypeNormalDotGroup = true;
                    auto item = makeNormalDot();
                    isSuccess &= item->fromXElement( message, *it );
                    myNormalTypeNormalDotGroup->addNormalDot( item );
                }

            }

            if( !isActualNotesFound )
            {
                message << "TimeModification: '" << myActualNotes->getElementName() << "' is required but was not found" << std::endl;
            }
            if( !isNormalNotesFound )
            {
                message << "TimeModification: '" << myNormalNotes->getElementName() << "' is required but was not found" << std::endl;
            }
            return isSuccess;
        }

    }
}
