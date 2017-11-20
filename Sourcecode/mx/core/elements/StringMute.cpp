// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/StringMute.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        StringMute::StringMute()
        :ElementInterface()
        ,myAttributes( std::make_shared<StringMuteAttributes>() )
        {}


        bool StringMute::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool StringMute::hasContents() const  { return false; }
        std::ostream& StringMute::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& StringMute::streamName( std::ostream& os ) const  { os << "string-mute"; return os; }
        std::ostream& StringMute::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        StringMuteAttributesPtr StringMute::getAttributes() const
        {
            return myAttributes;
        }


        void StringMute::setAttributes( const StringMuteAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        bool StringMute::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            return myAttributes->fromXElement( message, xelement );
        }

    }
}
