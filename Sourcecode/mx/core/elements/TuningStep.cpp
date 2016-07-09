// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/TuningStep.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        TuningStep::TuningStep()
        :myValue( StepEnum::a )
        {}


        TuningStep::TuningStep( const StepEnum& value )
        :myValue( value )
        {}


        bool TuningStep::hasAttributes() const
        {
            return false;
        }


        bool TuningStep::hasContents() const
        {
            return true;
        }


        std::ostream& TuningStep::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& TuningStep::streamName( std::ostream& os ) const
        {
            os << "tuning-step";
            return os;
        }


        std::ostream& TuningStep::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        StepEnum TuningStep::getValue() const
        {
            return myValue;
        }


        void TuningStep::setValue( const StepEnum& value )
        {
            myValue = value;
        }


        bool TuningStep::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_UNUSED( message );
            myValue = parseStepEnum( xelement.getValue() );
            return true;
        }

    }
}
