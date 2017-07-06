// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Pedal.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Pedal::Pedal()
        :ElementInterface()
        ,myAttributes( std::make_shared<PedalAttributes>() )
        {}


        bool Pedal::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Pedal::hasContents() const  { return false; }
        std::ostream& Pedal::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Pedal::streamName( std::ostream& os ) const  { os << "pedal"; return os; }
        std::ostream& Pedal::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        PedalAttributesPtr Pedal::getAttributes() const
        {
            return myAttributes;
        }


        void Pedal::setAttributes( const PedalAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        bool Pedal::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            return myAttributes->fromXElement( message, xelement );
        }

    }
}
