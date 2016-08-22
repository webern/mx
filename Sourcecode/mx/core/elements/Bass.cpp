// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/Bass.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/BassAlter.h"
#include "mx/core/elements/BassStep.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Bass::Bass()
        :myBassStep( makeBassStep() )
        ,myBassAlter( makeBassAlter() )
        ,myHasBassAlter( false )
        {}


        bool Bass::hasAttributes() const
        {
            return false;
        }


        std::ostream& Bass::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& Bass::streamName( std::ostream& os ) const
        {
            os << "bass";
            return os;
        }


        bool Bass::hasContents() const
        {
            return true;
        }


        std::ostream& Bass::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            os << std::endl;
            myBassStep->toStream( os, indentLevel+1 );
            if ( myHasBassAlter )
            {
                os << std::endl;
                myBassAlter->toStream( os, indentLevel+1 );
            }
            os << std::endl;
            isOneLineOnly = false;
            return os;
        }


        BassStepPtr Bass::getBassStep() const
        {
            return myBassStep;
        }


        void Bass::setBassStep( const BassStepPtr& value )
        {
            if( value )
            {
                myBassStep = value;
            }
        }


        BassAlterPtr Bass::getBassAlter() const
        {
            return myBassAlter;
        }


        void Bass::setBassAlter( const BassAlterPtr& value )
        {
            if( value )
            {
                myBassAlter = value;
            }
        }


        bool Bass::getHasBassAlter() const
        {
            return myHasBassAlter;
        }


        void Bass::setHasBassAlter( const bool value )
        {
            myHasBassAlter = value;
        }


        bool Bass::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            bool isBassStepFound = false;

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if ( importElement( message, *it, isSuccess, *myBassStep, isBassStepFound ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myBassAlter, myHasBassAlter ) ) { continue; }
            }

            if( !isBassStepFound )
            {
                message << "Bass: '" << myBassStep->getElementName() << "' is required but was not found" << std::endl;
            }
            MX_RETURN_IS_SUCCESS;
        }

    }
}
