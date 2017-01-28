// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/TraditionalKey.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/Cancel.h"
#include "mx/core/elements/Fifths.h"
#include "mx/core/elements/Mode.h"

#include <iostream>

namespace mx
{
    namespace core
    {
        TraditionalKey::TraditionalKey()
        :ElementInterface()
        ,myCancel( std::make_shared<Cancel>( FifthsValue{ 0 } ) )
        ,myFifths( std::make_shared<Fifths>( FifthsValue{ 0 } ) )
        ,myMode( std::make_shared<Mode>() )
        ,myHasCancel( false )
        ,myHasMode( false )
        {}


        bool TraditionalKey::hasAttributes() const
        {
            return false;
        }


        std::ostream& TraditionalKey::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& TraditionalKey::streamName( std::ostream& os ) const
        {
            return os;
        }


        bool TraditionalKey::hasContents() const
        {
            return true;
        }


        std::ostream& TraditionalKey::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            if ( getHasCancel() && myCancel )
            {
                myCancel->toStream( os, indentLevel ) << std::endl;
            }
            if ( myFifths )
            {
                myFifths->toStream( os, indentLevel );
            }
            if ( getHasMode() && myMode )
            {
                os << std::endl;
                myMode->toStream( os, indentLevel );
            }
            isOneLineOnly = ( !myCancel && !myMode );
            return os;
        }


        CancelPtr TraditionalKey::getCancel() const
        {
            return myCancel;
        }


        void TraditionalKey::setCancel( const CancelPtr& value )
        {
            if ( value )
            {
                myCancel = value;
            }
        }


        bool TraditionalKey::getHasCancel() const
        {
            return myHasCancel;
        }


        void TraditionalKey::setHasCancel( const bool value )
        {
            myHasCancel = value;
        }


        FifthsPtr TraditionalKey::getFifths() const
        {
            return myFifths;
        }


        void TraditionalKey::setFifths( const FifthsPtr& value )
        {
            if ( value )
            {
                myFifths = value;
            }
        }


        ModePtr TraditionalKey::getMode() const
        {
            return myMode;
        }


        void TraditionalKey::setMode( const ModePtr& value )
        {
            if ( value )
            {
                myMode = value;
            }
        }


        bool TraditionalKey::getHasMode() const
        {
            return myHasMode;
        }


        void TraditionalKey::setHasMode( const bool value )
        {
            myHasMode = value;
        }


        bool TraditionalKey::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            bool isFifthsFound = false;

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if ( importElement( message, *it, isSuccess, *myCancel, myHasCancel ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myFifths, isFifthsFound ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myMode, myHasMode ) ) { continue; }
            }

            if( !isFifthsFound )
            {
                message << "TraditionalKey: '" << myFifths->getElementName() << "' is required but was not found" << std::endl;
            }
            MX_RETURN_IS_SUCCESS;
        }

    }
}
