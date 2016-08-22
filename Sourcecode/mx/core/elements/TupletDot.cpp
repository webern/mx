// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/TupletDot.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        TupletDot::TupletDot()
        :ElementInterface()
        ,myAttributes( std::make_shared<TupletDotAttributes>() )
        {}


        bool TupletDot::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool TupletDot::hasContents() const  { return false; }
        std::ostream& TupletDot::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& TupletDot::streamName( std::ostream& os ) const  { os << "tuplet-dot"; return os; }
        std::ostream& TupletDot::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            return os;
        }


        TupletDotAttributesPtr TupletDot::getAttributes() const
        {
            return myAttributes;
        }


        void TupletDot::setAttributes( const TupletDotAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        bool TupletDot::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            return myAttributes->fromXElement( message, xelement );
        }

    }
}
