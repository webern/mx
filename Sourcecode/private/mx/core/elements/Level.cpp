// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Level.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Level::Level()
        :myValue()
        ,myAttributes( std::make_shared<LevelAttributes>() )
        {}


        Level::Level( const XsString& value )
        :myValue( value )
        ,myAttributes( std::make_shared<LevelAttributes>() )
        {}


        bool Level::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Level::hasContents() const
        {
            return true;
        }


        std::ostream& Level::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Level::streamName( std::ostream& os ) const
        {
            os << "level";
            return os;
        }


        std::ostream& Level::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        LevelAttributesPtr Level::getAttributes() const
        {
            return myAttributes;
        }


        void Level::setAttributes( const LevelAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        XsString Level::getValue() const
        {
            return myValue;
        }


        void Level::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool Level::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.setValue( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
