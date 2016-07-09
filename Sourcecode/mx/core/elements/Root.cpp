// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/Root.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/RootAlter.h"
#include "mx/core/elements/RootStep.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Root::Root()
        :myRootStep( makeRootStep() )
        ,myRootAlter( makeRootAlter() )
        ,myHasRootAlter( false )
        {}


        bool Root::hasAttributes() const
        {
            return false;
        }


        std::ostream& Root::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& Root::streamName( std::ostream& os ) const
        {
            os << "root";
            return os;
        }


        bool Root::hasContents() const
        {
            return true;
        }


        std::ostream& Root::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            os << std::endl;
            myRootStep->toStream( os, indentLevel+1 );
            if ( myHasRootAlter )
            {
                os << std::endl;
                myRootAlter->toStream( os, indentLevel+1 );
            }
            os << std::endl;
            isOneLineOnly = false;
            return os;
        }


        RootStepPtr Root::getRootStep() const
        {
            return myRootStep;
        }


        void Root::setRootStep( const RootStepPtr& value )
        {
            if( value )
            {
                myRootStep = value;
            }
        }


        RootAlterPtr Root::getRootAlter() const
        {
            return myRootAlter;
        }


        void Root::setRootAlter( const RootAlterPtr& value )
        {
            if( value )
            {
                myRootAlter = value;
            }
        }


        bool Root::getHasRootAlter() const
        {
            return myHasRootAlter;
        }


        void Root::setHasRootAlter( const bool value )
        {
            myHasRootAlter = value;
        }


        bool Root::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            bool isRootStepFound = false;

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if ( importElement( message, *it, isSuccess, *myRootStep, isRootStepFound ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myRootAlter, myHasRootAlter ) ) { continue; }
            }

            if( !isRootStepFound )
            {
                message << "Root: '" << myRootStep->getElementName() << "' is required but was not found" << std::endl;
            }
            MX_RETURN_IS_SUCCESS;
        }

    }
}
