// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/PartGroup.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/EditorialGroup.h"
#include "mx/core/elements/GroupAbbreviation.h"
#include "mx/core/elements/GroupAbbreviationDisplay.h"
#include "mx/core/elements/GroupBarline.h"
#include "mx/core/elements/GroupName.h"
#include "mx/core/elements/GroupNameDisplay.h"
#include "mx/core/elements/GroupSymbol.h"
#include "mx/core/elements/GroupTime.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        PartGroup::PartGroup()
        :myAttributes( std::make_shared<PartGroupAttributes>() )
        ,myGroupName( makeGroupName() )
        ,myHasGroupName( false )
        ,myGroupNameDisplay( makeGroupNameDisplay() )
        ,myHasGroupNameDisplay( false )
        ,myGroupAbbreviation( makeGroupAbbreviation() )
        ,myHasGroupAbbreviation( false )
        ,myGroupAbbreviationDisplay( makeGroupAbbreviationDisplay() )
        ,myHasGroupAbbreviationDisplay( false )
        ,myGroupSymbol( makeGroupSymbol() )
        ,myHasGroupSymbol( false )
        ,myGroupBarline( makeGroupBarline() )
        ,myHasGroupBarline( false )
        ,myGroupTime( makeGroupTime() )
        ,myHasGroupTime( false )
        ,myEditorialGroup( makeEditorialGroup() )
        {}


        bool PartGroup::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& PartGroup::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& PartGroup::streamName( std::ostream& os ) const
        {
            os << "part-group";
            return os;
        }


        bool PartGroup::hasContents() const
        {
            return myHasGroupName
            || myHasGroupNameDisplay
            || myHasGroupAbbreviation
            || myHasGroupAbbreviationDisplay
            || myHasGroupSymbol
            || myHasGroupBarline
            || myHasGroupTime
            || myEditorialGroup->hasContents();
        }


        std::ostream& PartGroup::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            if ( hasContents() )
            {
                if ( myHasGroupName )
                {
                    os << std::endl;
                    myGroupName->toStream( os, indentLevel+1 );
                }
                if ( myHasGroupNameDisplay )
                {
                    os << std::endl;
                    myGroupNameDisplay->toStream( os, indentLevel+1 );
                }
                if ( myHasGroupAbbreviation )
                {
                    os << std::endl;
                    myGroupAbbreviation->toStream( os, indentLevel+1 );
                }
                if ( myHasGroupAbbreviationDisplay )
                {
                    os << std::endl;
                    myGroupAbbreviationDisplay->toStream( os, indentLevel+1 );
                }
                if ( myHasGroupSymbol )
                {
                    os << std::endl;
                    myGroupSymbol->toStream( os, indentLevel+1 );
                }
                if ( myHasGroupBarline )
                {
                    os << std::endl;
                    myGroupBarline->toStream( os, indentLevel+1 );
                }
                if ( myHasGroupTime )
                {
                    os << std::endl;
                    myGroupTime->toStream( os, indentLevel+1 );
                }
                if ( myEditorialGroup->hasContents() )
                {
                    os << std::endl;
                    myEditorialGroup->streamContents( os, indentLevel+1, isOneLineOnly );
                }
                os << std::endl;
                isOneLineOnly = false;
            }
            else
            {
                isOneLineOnly = true;
            }
            return os;
        }


        PartGroupAttributesPtr PartGroup::getAttributes() const
        {
            return myAttributes;
        }


        void PartGroup::setAttributes( const PartGroupAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        GroupNamePtr PartGroup::getGroupName() const
        {
            return myGroupName;
        }


        void PartGroup::setGroupName( const GroupNamePtr& value )
        {
            if( value )
            {
                myGroupName = value;
            }
        }


        bool PartGroup::getHasGroupName() const
        {
            return myHasGroupName;
        }


        void PartGroup::setHasGroupName( const bool value )
        {
            myHasGroupName = value;
        }


        GroupNameDisplayPtr PartGroup::getGroupNameDisplay() const
        {
            return myGroupNameDisplay;
        }


        void PartGroup::setGroupNameDisplay( const GroupNameDisplayPtr& value )
        {
            if( value )
            {
                myGroupNameDisplay = value;
            }
        }


        bool PartGroup::getHasGroupNameDisplay() const
        {
            return myHasGroupNameDisplay;
        }


        void PartGroup::setHasGroupNameDisplay( const bool value )
        {
            myHasGroupNameDisplay = value;
        }


        GroupAbbreviationPtr PartGroup::getGroupAbbreviation() const
        {
            return myGroupAbbreviation;
        }


        void PartGroup::setGroupAbbreviation( const GroupAbbreviationPtr& value )
        {
            if( value )
            {
                myGroupAbbreviation = value;
            }
        }


        bool PartGroup::getHasGroupAbbreviation() const
        {
            return myHasGroupAbbreviation;
        }


        void PartGroup::setHasGroupAbbreviation( const bool value )
        {
            myHasGroupAbbreviation = value;
        }


        GroupAbbreviationDisplayPtr PartGroup::getGroupAbbreviationDisplay() const
        {
            return myGroupAbbreviationDisplay;
        }


        void PartGroup::setGroupAbbreviationDisplay( const GroupAbbreviationDisplayPtr& value )
        {
            if( value )
            {
                myGroupAbbreviationDisplay = value;
            }
        }


        bool PartGroup::getHasGroupAbbreviationDisplay() const
        {
            return myHasGroupAbbreviationDisplay;
        }


        void PartGroup::setHasGroupAbbreviationDisplay( const bool value )
        {
            myHasGroupAbbreviationDisplay = value;
        }


        GroupSymbolPtr PartGroup::getGroupSymbol() const
        {
            return myGroupSymbol;
        }


        void PartGroup::setGroupSymbol( const GroupSymbolPtr& value )
        {
            if( value )
            {
                myGroupSymbol = value;
            }
        }


        bool PartGroup::getHasGroupSymbol() const
        {
            return myHasGroupSymbol;
        }


        void PartGroup::setHasGroupSymbol( const bool value )
        {
            myHasGroupSymbol = value;
        }


        GroupBarlinePtr PartGroup::getGroupBarline() const
        {
            return myGroupBarline;
        }


        void PartGroup::setGroupBarline( const GroupBarlinePtr& value )
        {
            if( value )
            {
                myGroupBarline = value;
            }
        }


        bool PartGroup::getHasGroupBarline() const
        {
            return myHasGroupBarline;
        }


        void PartGroup::setHasGroupBarline( const bool value )
        {
            myHasGroupBarline = value;
        }


        GroupTimePtr PartGroup::getGroupTime() const
        {
            return myGroupTime;
        }


        void PartGroup::setGroupTime( const GroupTimePtr& value )
        {
            if( value )
            {
                myGroupTime = value;
            }
        }


        bool PartGroup::getHasGroupTime() const
        {
            return myHasGroupTime;
        }


        void PartGroup::setHasGroupTime( const bool value )
        {
            myHasGroupTime = value;
        }


        EditorialGroupPtr PartGroup::getEditorialGroup() const
        {
            return myEditorialGroup;
        }


        void PartGroup::setEditorialGroup( const EditorialGroupPtr& value )
        {
            if( value )
            {
                myEditorialGroup = value;
            }
        }
        
        
        bool PartGroup::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            
            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if ( importElement( message, *it, isSuccess, *myGroupName, myHasGroupName ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myGroupNameDisplay, myHasGroupNameDisplay ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myGroupAbbreviation, myHasGroupAbbreviation ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myGroupAbbreviationDisplay, myHasGroupAbbreviationDisplay ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myGroupSymbol, myHasGroupSymbol ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myGroupBarline, myHasGroupBarline ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myGroupTime, myHasGroupTime ) ) { continue; }
                importGroup( message, it, endIter, isSuccess, myEditorialGroup );
            }
            
            MX_RETURN_IS_SUCCESS;
        }
    }
}
