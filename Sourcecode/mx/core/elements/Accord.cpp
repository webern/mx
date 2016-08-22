// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/Accord.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/TuningAlter.h"
#include "mx/core/elements/TuningOctave.h"
#include "mx/core/elements/TuningStep.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Accord::Accord()
        :myAttributes( std::make_shared<AccordAttributes>() )
        ,myTuningStep( makeTuningStep() )
        ,myTuningAlter( makeTuningAlter() )
        ,myHasTuningAlter( false )
        ,myTuningOctave( makeTuningOctave() )
        {}


        bool Accord::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& Accord::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& Accord::streamName( std::ostream& os ) const
        {
            os << "accord";
            return os;
        }


        bool Accord::hasContents() const
        {
            return true;
        }


        std::ostream& Accord::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            isOneLineOnly = false;
            os << std::endl;
            myTuningStep->toStream( os, indentLevel+1 );
            if ( myHasTuningAlter )
            {
                os << std::endl;
                myTuningAlter->toStream( os, indentLevel+1 );
            }
            os << std::endl;
            myTuningOctave->toStream( os, indentLevel+1 );
            os << std::endl;
            return os;
        }


        AccordAttributesPtr Accord::getAttributes() const
        {
            return myAttributes;
        }


        void Accord::setAttributes( const AccordAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        TuningStepPtr Accord::getTuningStep() const
        {
            return myTuningStep;
        }


        void Accord::setTuningStep( const TuningStepPtr& value )
        {
            if( value )
            {
                myTuningStep = value;
            }
        }


        TuningAlterPtr Accord::getTuningAlter() const
        {
            return myTuningAlter;
        }


        void Accord::setTuningAlter( const TuningAlterPtr& value )
        {
            if( value )
            {
                myTuningAlter = value;
            }
        }


        bool Accord::getHasTuningAlter() const
        {
            return myHasTuningAlter;
        }


        void Accord::setHasTuningAlter( const bool value )
        {
            myHasTuningAlter = value;
        }


        TuningOctavePtr Accord::getTuningOctave() const
        {
            return myTuningOctave;
        }


        void Accord::setTuningOctave( const TuningOctavePtr& value )
        {
            if( value )
            {
                myTuningOctave = value;
            }
        }


        bool Accord::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            bool isTuningStepFound = false;
            bool isTuningOctaveFound = false;

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if ( importElement( message, *it, isSuccess, *myTuningStep, isTuningStepFound ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myTuningAlter, myHasTuningAlter ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myTuningOctave, isTuningOctaveFound ) ) { continue; }
            }

            if( !isTuningStepFound )
            {
                message << "Accord: '" << myTuningStep->getElementName() << "' is required but was not found" << std::endl;
            }
            if( !isTuningOctaveFound )
            {
                message << "Accord: '" << myTuningOctave->getElementName() << "' is required but was not found" << std::endl;
            }
            MX_RETURN_IS_SUCCESS;
        }

    }
}
