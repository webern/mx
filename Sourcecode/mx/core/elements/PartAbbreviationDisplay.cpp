// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/PartAbbreviationDisplay.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/DisplayTextOrAccidentalText.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        PartAbbreviationDisplay::PartAbbreviationDisplay()
        :myAttributes( std::make_shared<PartAbbreviationDisplayAttributes>() )
        ,myDisplayTextOrAccidentalTextSet()
        {}


        bool PartAbbreviationDisplay::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& PartAbbreviationDisplay::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& PartAbbreviationDisplay::streamName( std::ostream& os ) const
        {
            os << "part-abbreviation-display";
            return os;
        }


        bool PartAbbreviationDisplay::hasContents() const
        {
            return myDisplayTextOrAccidentalTextSet.size() > 0;
        }


        std::ostream& PartAbbreviationDisplay::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            if ( hasContents() )
            {
                for ( auto x : myDisplayTextOrAccidentalTextSet )
                {
                    os << std::endl;
                    x->streamContents( os, indentLevel+1, isOneLineOnly );
                }
                isOneLineOnly = !hasContents();
                os << std::endl;
            }
            else
            {
                isOneLineOnly = true;
            }
            return os;
        }


        PartAbbreviationDisplayAttributesPtr PartAbbreviationDisplay::getAttributes() const
        {
            return myAttributes;
        }


        void PartAbbreviationDisplay::setAttributes( const PartAbbreviationDisplayAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }
        
        
        const DisplayTextOrAccidentalTextSet& PartAbbreviationDisplay::getDisplayTextOrAccidentalText() const
        {
            return myDisplayTextOrAccidentalTextSet;
        }
        
        
        void PartAbbreviationDisplay::addDisplayTextOrAccidentalText( const DisplayTextOrAccidentalTextPtr& value )
        {
            if ( value )
            {
                myDisplayTextOrAccidentalTextSet.push_back( value );
            }
        }
        
        
        void PartAbbreviationDisplay::removeDisplayTextOrAccidentalText( const DisplayTextOrAccidentalTextSetIterConst& setIterator )
        {
            if ( setIterator != myDisplayTextOrAccidentalTextSet.cend() )
            {
                myDisplayTextOrAccidentalTextSet.erase( setIterator );
            }
        }
        
        
        void PartAbbreviationDisplay::clearDisplayTextOrAccidentalTextSet()
        {
            myDisplayTextOrAccidentalTextSet.clear();
        }
        
        
        DisplayTextOrAccidentalTextPtr PartAbbreviationDisplay::getDisplayTextOrAccidentalText( const DisplayTextOrAccidentalTextSetIterConst& setIterator ) const
        {
            if ( setIterator != myDisplayTextOrAccidentalTextSet.cend() )
            {
                return *setIterator;
            }
            return makeDisplayTextOrAccidentalText();
        }

        
        const DisplayTextOrAccidentalTextSet& PartAbbreviationDisplay::getDisplayTextOrAccidentalTextSet() const
        {
            return myDisplayTextOrAccidentalTextSet;
        }
        
        
        bool PartAbbreviationDisplay::fromXElement( std::ostream& message, xml::XElement& xelement )
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
