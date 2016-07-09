// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/Pitch.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/Alter.h"
#include "mx/core/elements/Octave.h"
#include "mx/core/elements/Step.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Pitch::Pitch()
        :myStep( makeStep() )
        ,myAlter( makeAlter() )
        ,myHasAlter( false )
        ,myOctave( makeOctave() )
        {}


        bool Pitch::hasAttributes() const
        {
            return false;
        }


        std::ostream& Pitch::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& Pitch::streamName( std::ostream& os ) const
        {
            os << "pitch";
            return os;
        }


        bool Pitch::hasContents() const
        {
            return true;
        }


        std::ostream& Pitch::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            os << std::endl;
            myStep->toStream( os, indentLevel+1 );
            if ( myHasAlter )
            {
                os << std::endl;
                myAlter->toStream( os, indentLevel+1 );
            }
            os << std::endl;
            myOctave->toStream( os, indentLevel+1 );
            isOneLineOnly = false;
            os << std::endl;
            return os;
        }


        StepPtr Pitch::getStep() const
        {
            return myStep;
        }


        void Pitch::setStep( const StepPtr& value )
        {
            if( value )
            {
                myStep = value;
            }
        }


        AlterPtr Pitch::getAlter() const
        {
            return myAlter;
        }


        void Pitch::setAlter( const AlterPtr& value )
        {
            if( value )
            {
                myAlter = value;
            }
        }


        bool Pitch::getHasAlter() const
        {
            return myHasAlter;
        }


        void Pitch::setHasAlter( const bool value )
        {
            myHasAlter = value;
        }


        OctavePtr Pitch::getOctave() const
        {
            return myOctave;
        }


        void Pitch::setOctave( const OctavePtr& value )
        {
            if( value )
            {
                myOctave = value;
            }
        }


        bool Pitch::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            bool isStepFound = false;
            bool isOctaveFound = false;

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if ( importElement( message, *it, isSuccess, *myStep, isStepFound ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myAlter, myHasAlter ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myOctave, isOctaveFound ) ) { continue; }
            }
            
            if( !isStepFound )
            {
                message << "Pitch: 'step' element is required but was not found" << std::endl;
            }
            
            if( !isOctaveFound )
            {
                message << "Pitch: 'octave' element is required but was not found" << std::endl;
            }
            
            MX_RETURN_IS_SUCCESS;
        }

    }
}
