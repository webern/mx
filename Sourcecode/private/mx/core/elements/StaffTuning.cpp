// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/StaffTuning.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/TuningAlter.h"
#include "mx/core/elements/TuningOctave.h"
#include "mx/core/elements/TuningStep.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        StaffTuning::StaffTuning()
        :myAttributes( std::make_shared<StaffTuningAttributes>() )
        ,myTuningStep( makeTuningStep() )
        ,myTuningAlter( makeTuningAlter() )
        ,myHasTuningAlter( false )
        ,myTuningOctave( makeTuningOctave() )
        {}


        bool StaffTuning::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& StaffTuning::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& StaffTuning::streamName( std::ostream& os ) const
        {
            os << "staff-tuning";
            return os;
        }


        bool StaffTuning::hasContents() const
        {
            return true;
        }


        std::ostream& StaffTuning::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
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


        StaffTuningAttributesPtr StaffTuning::getAttributes() const
        {
            return myAttributes;
        }


        void StaffTuning::setAttributes( const StaffTuningAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        TuningStepPtr StaffTuning::getTuningStep() const
        {
            return myTuningStep;
        }


        void StaffTuning::setTuningStep( const TuningStepPtr& value )
        {
            if( value )
            {
                myTuningStep = value;
            }
        }


        TuningAlterPtr StaffTuning::getTuningAlter() const
        {
            return myTuningAlter;
        }


        void StaffTuning::setTuningAlter( const TuningAlterPtr& value )
        {
            if( value )
            {
                myTuningAlter = value;
            }
        }


        bool StaffTuning::getHasTuningAlter() const
        {
            return myHasTuningAlter;
        }


        void StaffTuning::setHasTuningAlter( const bool value )
        {
            myHasTuningAlter = value;
        }


        TuningOctavePtr StaffTuning::getTuningOctave() const
        {
            return myTuningOctave;
        }


        void StaffTuning::setTuningOctave( const TuningOctavePtr& value )
        {
            if( value )
            {
                myTuningOctave = value;
            }
        }


        bool StaffTuning::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
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
                message << "StaffTuning: '" << myTuningStep->getElementName() << "' is required but was not found" << std::endl;
            }
            if( !isTuningOctaveFound )
            {
                message << "StaffTuning: '" << myTuningOctave->getElementName() << "' is required but was not found" << std::endl;
            }
            MX_RETURN_IS_SUCCESS;
        }

    }
}
