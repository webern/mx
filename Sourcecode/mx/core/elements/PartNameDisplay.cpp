// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/PartNameDisplay.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/DisplayTextOrAccidentalText.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        PartNameDisplay::PartNameDisplay()
        :myAttributes( std::make_shared<PartNameDisplayAttributes>() )
        ,myDisplayTextOrAccidentalTextSet()
        {}


        bool PartNameDisplay::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& PartNameDisplay::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& PartNameDisplay::streamName( std::ostream& os ) const
        {
            os << "part-name-display";
            return os;
        }


        bool PartNameDisplay::hasContents() const
        {
            return true;
        }


        std::ostream& PartNameDisplay::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            if ( myDisplayTextOrAccidentalTextSet.size() > 0 )
            {
                for ( auto x : myDisplayTextOrAccidentalTextSet )
                {
                    os << std::endl;
                    x->streamContents( os, indentLevel+1, isOneLineOnly );
                }
                isOneLineOnly = false;
                os << std::endl;
            }
            else
            {
                isOneLineOnly = true;
            }
            return os;
        }


        PartNameDisplayAttributesPtr PartNameDisplay::getAttributes() const
        {
            return myAttributes;
        }


        void PartNameDisplay::setAttributes( const PartNameDisplayAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        const DisplayTextOrAccidentalTextSet& PartNameDisplay::getDisplayTextOrAccidentalText() const
        {
            return myDisplayTextOrAccidentalTextSet;
        }


        void PartNameDisplay::addDisplayTextOrAccidentalText( const DisplayTextOrAccidentalTextPtr& value )
        {
            if ( value )
            {
                myDisplayTextOrAccidentalTextSet.push_back( value );
            }
        }


        void PartNameDisplay::removeDisplayTextOrAccidentalText( const DisplayTextOrAccidentalTextSetIterConst& setIterator )
        {
            if ( setIterator != myDisplayTextOrAccidentalTextSet.cend() )
            {
                myDisplayTextOrAccidentalTextSet.erase( setIterator );
            }
        }


        void PartNameDisplay::clearDisplayTextOrAccidentalTextSet()
        {
            myDisplayTextOrAccidentalTextSet.clear();
        }


        DisplayTextOrAccidentalTextPtr PartNameDisplay::getDisplayTextOrAccidentalText( const DisplayTextOrAccidentalTextSetIterConst& setIterator ) const
        {
            if ( setIterator != myDisplayTextOrAccidentalTextSet.cend() )
            {
                return *setIterator;
            }
            return makeDisplayTextOrAccidentalText();
        }


        bool PartNameDisplay::fromXElement( std::ostream& message, xml::XElement& xelement )
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
