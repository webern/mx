// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/WavyLine.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        WavyLine::WavyLine()
        :ElementInterface()
        ,myAttributes( std::make_shared<WavyLineAttributes>() )
        {}


        bool WavyLine::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool WavyLine::hasContents() const  { return false; }
        std::ostream& WavyLine::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& WavyLine::streamName( std::ostream& os ) const  { os << "wavy-line"; return os; }
        std::ostream& WavyLine::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        WavyLineAttributesPtr WavyLine::getAttributes() const
        {
            return myAttributes;
        }


        void WavyLine::setAttributes( const WavyLineAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        bool WavyLine::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            return myAttributes->fromXElement( message, xelement );
        }

    }
}
