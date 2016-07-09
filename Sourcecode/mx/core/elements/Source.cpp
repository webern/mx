// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/Source.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Source::Source()
        :myValue()
        {}


        Source::Source( const XsString& value )
        :myValue( value )
        {}


        bool Source::hasAttributes() const
        {
            return false;
        }


        bool Source::hasContents() const
        {
            return true;
        }


        std::ostream& Source::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& Source::streamName( std::ostream& os ) const
        {
            os << "source";
            return os;
        }


        std::ostream& Source::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        XsString Source::getValue() const
        {
            return myValue;
        }


        void Source::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool Source::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            MX_UNUSED( xelement );
            myValue.setValue( xelement.getValue() );
            return true;
        }

    }
}
