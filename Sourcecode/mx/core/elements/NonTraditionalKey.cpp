// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/NonTraditionalKey.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/KeyAccidental.h"
#include "mx/core/elements/KeyAlter.h"
#include "mx/core/elements/KeyStep.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        NonTraditionalKey::NonTraditionalKey()
        :ElementInterface()
        ,myKeyStep( std::make_shared<KeyStep>() )
        ,myKeyAlter( std::make_shared<KeyAlter>() )
        ,myKeyAccidental( std::make_shared<KeyAccidental>() )
        ,myHasKeyAccidental( false )
        {}


        bool NonTraditionalKey::hasAttributes() const
        {
            return false;
        }


        std::ostream& NonTraditionalKey::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& NonTraditionalKey::streamName( std::ostream& os ) const
        {
            return os;
        }


        bool NonTraditionalKey::hasContents() const
        {
            return true;
        }


        std::ostream& NonTraditionalKey::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            isOneLineOnly = false;
            if ( myKeyStep )
            {
                myKeyStep->toStream( os, indentLevel ) << std::endl;
            }
            if ( myKeyAlter )
            {
                myKeyAlter->toStream( os, indentLevel );
            }
            if ( myKeyAccidental && myHasKeyAccidental )
            {
                os << std::endl;
                myKeyAccidental->toStream( os, indentLevel );
            }
            return os;
        }


        KeyStepPtr NonTraditionalKey::getKeyStep() const
        {
            return myKeyStep;
        }


        void NonTraditionalKey::setKeyStep( const KeyStepPtr& value )
        {
            if ( value )
            {
                myKeyStep = value;
            }
        }


        KeyAlterPtr NonTraditionalKey::getKeyAlter() const
        {
            return myKeyAlter;
        }


        void NonTraditionalKey::setKeyAlter( const KeyAlterPtr& value )
        {
            if ( value )
            {
                myKeyAlter = value;
            }
        }


        KeyAccidentalPtr NonTraditionalKey::getKeyAccidental() const
        {
            return myKeyAccidental;
        }


        void NonTraditionalKey::setKeyAccidental( const KeyAccidentalPtr& value )
        {
            if ( value )
            {
                myKeyAccidental = value;
            }
        }


        bool NonTraditionalKey::getHasKeyAccidental() const
        {
            return myHasKeyAccidental;
        }


        void NonTraditionalKey::setHasKeyAccidental( const bool value )
        {
            myHasKeyAccidental = value;
        }


        bool NonTraditionalKey::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            bool isKeyStepFound = false;
            bool isKeyAlterFound = false;

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if ( importElement( message, *it, isSuccess, *myKeyStep, isKeyStepFound ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myKeyAlter, isKeyAlterFound ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myKeyAccidental, myHasKeyAccidental ) ) { continue; }
            }

            if( !isKeyStepFound )
            {
                message << "NonTraditionalKey: '" << myKeyStep->getElementName() << "' is required but was not found" << std::endl;
            }
            if( !isKeyAlterFound )
            {
                message << "NonTraditionalKey: '" << myKeyAlter->getElementName() << "' is required but was not found" << std::endl;
            }
            MX_RETURN_IS_SUCCESS;
        }

    }
}
