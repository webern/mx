// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/GroupAbbreviationDisplay.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/DisplayTextOrAccidentalText.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        GroupAbbreviationDisplay::GroupAbbreviationDisplay()
        :myAttributes( std::make_shared<GroupAbbreviationDisplayAttributes>() )
        ,myDisplayTextOrAccidentalTextSet()
        {}


        bool GroupAbbreviationDisplay::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& GroupAbbreviationDisplay::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& GroupAbbreviationDisplay::streamName( std::ostream& os ) const
        {
            os << "group-abbreviation-display";
            return os;
        }


        bool GroupAbbreviationDisplay::hasContents() const
        {
            return myDisplayTextOrAccidentalTextSet.size() > 0;
        }


        std::ostream& GroupAbbreviationDisplay::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            if ( hasContents() )
            {
                for ( auto x : myDisplayTextOrAccidentalTextSet )
                {
                    os << std::endl;
                    x->streamContents( os, indentLevel+1, isOneLineOnly );
                }
                isOneLineOnly = false;
                os << std::endl;
            }
            isOneLineOnly = !hasContents();
            return os;
        }


        GroupAbbreviationDisplayAttributesPtr GroupAbbreviationDisplay::getAttributes() const
        {
            return myAttributes;
        }


        void GroupAbbreviationDisplay::setAttributes( const GroupAbbreviationDisplayAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        const DisplayTextOrAccidentalTextSet& GroupAbbreviationDisplay::getDisplayTextOrAccidentalText() const
        {
            return myDisplayTextOrAccidentalTextSet;
        }
        
        
        void GroupAbbreviationDisplay::addDisplayTextOrAccidentalText( const DisplayTextOrAccidentalTextPtr& value )
        {
            if ( value )
            {
                myDisplayTextOrAccidentalTextSet.push_back( value );
            }
        }
        
        
        void GroupAbbreviationDisplay::removeDisplayTextOrAccidentalText( const DisplayTextOrAccidentalTextSetIterConst& setIterator )
        {
            if ( setIterator != myDisplayTextOrAccidentalTextSet.cend() )
            {
                myDisplayTextOrAccidentalTextSet.erase( setIterator );
            }
        }
        
        
        void GroupAbbreviationDisplay::clearDisplayTextOrAccidentalTextSet()
        {
            myDisplayTextOrAccidentalTextSet.clear();
        }
        
        
        DisplayTextOrAccidentalTextPtr GroupAbbreviationDisplay::getDisplayTextOrAccidentalText( const DisplayTextOrAccidentalTextSetIterConst& setIterator ) const
        {
            if ( setIterator != myDisplayTextOrAccidentalTextSet.cend() )
            {
                return *setIterator;
            }
            return makeDisplayTextOrAccidentalText();
        }

        
        const DisplayTextOrAccidentalTextSet& GroupAbbreviationDisplay::getDisplayTextOrAccidentalTextSet() const
        {
            return myDisplayTextOrAccidentalTextSet;
        }
        

        bool GroupAbbreviationDisplay::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            
            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                auto item = makeDisplayTextOrAccidentalText();
                isSuccess &= item->fromXElement( message, *it );
                myDisplayTextOrAccidentalTextSet.push_back( item );
            }
            
            MX_RETURN_IS_SUCCESS;
        }

    }
}
