// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/KeyChoice.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/NonTraditionalKey.h"
#include "mx/core/elements/TraditionalKey.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        KeyChoice::KeyChoice()
        :myChoice( Choice::traditionalKey )
        ,myTraditionalKey( std::make_shared<TraditionalKey>() )
        ,myNonTraditionalKeySet()
        {}


        bool KeyChoice::hasAttributes() const
        {
            return false;
        }


        std::ostream& KeyChoice::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& KeyChoice::streamName( std::ostream& os ) const
        {
            return os;
        }


        bool KeyChoice::hasContents() const
        {
            return true;
        }


        std::ostream& KeyChoice::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            if ( myChoice == Choice::traditionalKey )
            {
                if ( myTraditionalKey )
                {
                    myTraditionalKey->streamContents( os, indentLevel, isOneLineOnly );
                }
            }
            else if ( myChoice == Choice::nonTraditionalKey )
            {
                for ( auto it = myNonTraditionalKeySet.cbegin();
                     it != myNonTraditionalKeySet.cend();
                     ++it )
                {
                    if ( it != myNonTraditionalKeySet.cbegin() )
                    {
                        os << std::endl;
                    }
                    (*it)->streamContents( os, indentLevel, isOneLineOnly );
                }
                if ( myNonTraditionalKeySet.size() > 1 )
                {
                    isOneLineOnly = false;
                }
            }
            return os;
        }


        KeyChoice::Choice KeyChoice::getChoice() const
        {
            return myChoice;
        }


        void KeyChoice::setChoice( const Choice value )
        {
            myChoice = value;
        }


        TraditionalKeyPtr KeyChoice::getTraditionalKey() const
        {
            return myTraditionalKey;
        }


        void KeyChoice::setTraditionalKey( const TraditionalKeyPtr& value )
        {
            if ( value )
            {
                myTraditionalKey = value;
            }
        }


        const NonTraditionalKeySet& KeyChoice::getNonTraditionalKeySet() const
        {
            return myNonTraditionalKeySet;
        }


        void KeyChoice::removeNonTraditionalKey( const NonTraditionalKeySetIterConst& value )
        {
            if ( value != myNonTraditionalKeySet.cend() )
            {
                myNonTraditionalKeySet.erase( value );
            }
        }


        void KeyChoice::addNonTraditionalKey( const NonTraditionalKeyPtr& value )
        {
            if ( value )
            {
                myNonTraditionalKeySet.push_back( value );
            }
        }


        void KeyChoice::clearNonTraditionalKeySet()
        {
            myNonTraditionalKeySet.clear();
        }


        MX_FROM_XELEMENT_UNUSED( KeyChoice );

    }
}
