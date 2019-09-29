// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/NormalTypeNormalDotGroup.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/NormalDot.h"
#include "mx/core/elements/NormalType.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        NormalTypeNormalDotGroup::NormalTypeNormalDotGroup()
        :myNormalType( makeNormalType() )
        ,myNormalDotSet()
        {}


        bool NormalTypeNormalDotGroup::hasAttributes() const
        {
            return false;
        }


        std::ostream& NormalTypeNormalDotGroup::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& NormalTypeNormalDotGroup::streamName( std::ostream& os ) const
        {
            return os;
        }


        bool NormalTypeNormalDotGroup::hasContents() const
        {
            return true;
        }


        std::ostream& NormalTypeNormalDotGroup::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            isOneLineOnly = false;
            myNormalType->toStream( os, indentLevel );
            for ( auto x : myNormalDotSet )
            {
                os << std::endl;
                x->toStream( os, indentLevel );
            }
            return os;
        }


        NormalTypePtr NormalTypeNormalDotGroup::getNormalType() const
        {
            return myNormalType;
        }


        void NormalTypeNormalDotGroup::setNormalType( const NormalTypePtr& value )
        {
            if( value )
            {
                myNormalType = value;
            }
        }


        const NormalDotSet& NormalTypeNormalDotGroup::getNormalDotSet() const
        {
            return myNormalDotSet;
        }


        void NormalTypeNormalDotGroup::removeNormalDot( const NormalDotSetIterConst& value )
        {
            if ( value != myNormalDotSet.cend() )
            {
                myNormalDotSet.erase( value );
            }
        }


        void NormalTypeNormalDotGroup::addNormalDot( const NormalDotPtr& value )
        {
            if ( value )
            {
                myNormalDotSet.push_back( value );
            }
        }


        void NormalTypeNormalDotGroup::clearNormalDotSet()
        {
            myNormalDotSet.clear();
        }


        NormalDotPtr NormalTypeNormalDotGroup::getNormalDot( const NormalDotSetIterConst& setIterator ) const
        {
            if( setIterator != myNormalDotSet.cend() )
            {
                return *setIterator;
            }
            return NormalDotPtr();
        }

        
        bool NormalTypeNormalDotGroup::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            bool isSuccess = true;
            bool isNormalTypeFound = false;
            bool isFirstNormalDotAdded = false;
            
            for( auto it = xelement.begin(); it != xelement.end(); ++it )
            {
                const std::string elementName = it->getName();
                
                if( elementName == "normal-type" )
                {
                    isNormalTypeFound = true;
                    isSuccess &= myNormalType->fromXElement( message, *it );
                }
                else if( elementName == "normal-dot" )
                {
                    auto normalDot = makeNormalDot();
                    isSuccess &= normalDot->fromXElement( message, *it );
                    
                    if( !isFirstNormalDotAdded && myNormalDotSet.size() == 1 )
                    {
                        *( myNormalDotSet.begin() ) = normalDot;
                        isFirstNormalDotAdded = true;
                    }
                    else
                    {
                        myNormalDotSet.push_back( normalDot );
                        isFirstNormalDotAdded = true;
                    }
                }
                else
                {
                    if( !isNormalTypeFound )
                    {
                        isSuccess = false;
                        message << "NormalTypeNormalDotGroup: 'normal-type' element is required but was not found" << std::endl;
                    }
                    break;
                }
            }
            MX_RETURN_IS_SUCCESS;
        }
    }
}
