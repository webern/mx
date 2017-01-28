// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/TimeRelation.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        TimeRelation::TimeRelation()
        :myValue( TimeRelationEnum::equals )
        {}


        TimeRelation::TimeRelation( const TimeRelationEnum& value )
        :myValue( value )
        {}


        bool TimeRelation::hasAttributes() const
        {
            return false;
        }


        bool TimeRelation::hasContents() const
        {
            return true;
        }


        std::ostream& TimeRelation::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& TimeRelation::streamName( std::ostream& os ) const
        {
            os << "time-relation";
            return os;
        }


        std::ostream& TimeRelation::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        TimeRelationEnum TimeRelation::getValue() const
        {
            return myValue;
        }


        void TimeRelation::setValue( const TimeRelationEnum& value )
        {
            myValue = value;
        }


        bool TimeRelation::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            myValue = parseTimeRelationEnum( xelement.getValue() );
            return true;
        }

    }
}
