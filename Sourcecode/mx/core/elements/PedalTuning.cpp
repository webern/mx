// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/PedalTuning.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/PedalAlter.h"
#include "mx/core/elements/PedalStep.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        PedalTuning::PedalTuning()
        :myPedalStep( makePedalStep() )
        ,myPedalAlter( makePedalAlter() )
        {}


        bool PedalTuning::hasAttributes() const
        {
            return false;
        }


        std::ostream& PedalTuning::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& PedalTuning::streamName( std::ostream& os ) const
        {
            os << "pedal-tuning";
            return os;
        }


        bool PedalTuning::hasContents() const
        {
            return true;
        }


        std::ostream& PedalTuning::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            isOneLineOnly = false;
            os << std::endl;
            myPedalStep->toStream( os, indentLevel+1 );
            os << std::endl;
            myPedalAlter->toStream( os, indentLevel+1 );
            os << std::endl;
            return os;
        }


        PedalStepPtr PedalTuning::getPedalStep() const
        {
            return myPedalStep;
        }


        void PedalTuning::setPedalStep( const PedalStepPtr& value )
        {
            if( value )
            {
                myPedalStep = value;
            }
        }


        PedalAlterPtr PedalTuning::getPedalAlter() const
        {
            return myPedalAlter;
        }


        void PedalTuning::setPedalAlter( const PedalAlterPtr& value )
        {
            if( value )
            {
                myPedalAlter = value;
            }
        }


        bool PedalTuning::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            bool isPedalStepFound = false;
            bool isPedalAlterFound = false;

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if ( importElement( message, *it, isSuccess, *myPedalStep, isPedalStepFound ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myPedalAlter, isPedalAlterFound ) ) { continue; }
            }

            if( !isPedalStepFound )
            {
                message << "PedalTuning: '" << myPedalStep->getElementName() << "' is required but was not found" << std::endl;
            }
            if( !isPedalAlterFound )
            {
                message << "PedalTuning: '" << myPedalAlter->getElementName() << "' is required but was not found" << std::endl;
            }
            MX_RETURN_IS_SUCCESS;
        }

    }
}
