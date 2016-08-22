// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/TimeModificationNormalTypeNormalDot.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/NormalDot.h"
#include "mx/core/elements/NormalType.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        TimeModificationNormalTypeNormalDot::TimeModificationNormalTypeNormalDot()
        :myNormalType( makeNormalType() )
        ,myNormalDotSet()
        {}


        bool TimeModificationNormalTypeNormalDot::hasAttributes() const
        {
            return false;
        }


        std::ostream& TimeModificationNormalTypeNormalDot::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& TimeModificationNormalTypeNormalDot::streamName( std::ostream& os ) const
        {
            return os;
        }


        bool TimeModificationNormalTypeNormalDot::hasContents() const
        {
            return true;
        }


        std::ostream& TimeModificationNormalTypeNormalDot::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
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


        NormalTypePtr TimeModificationNormalTypeNormalDot::getNormalType() const
        {
            return myNormalType;
        }


        void TimeModificationNormalTypeNormalDot::setNormalType( const NormalTypePtr& value )
        {
            if( value )
            {
                myNormalType = value;
            }
        }


        const NormalDotSet& TimeModificationNormalTypeNormalDot::getNormalDotSet() const
        {
            return myNormalDotSet;
        }


        void TimeModificationNormalTypeNormalDot::removeNormalDot( const NormalDotSetIterConst& value )
        {
            if ( value != myNormalDotSet.cend() )
            {
                myNormalDotSet.erase( value );
            }
        }


        void TimeModificationNormalTypeNormalDot::addNormalDot( const NormalDotPtr& value )
        {
            if ( value )
            {
                myNormalDotSet.push_back( value );
            }
        }


        void TimeModificationNormalTypeNormalDot::clearNormalDotSet()
        {
            myNormalDotSet.clear();
        }


        NormalDotPtr TimeModificationNormalTypeNormalDot::getNormalDot( const NormalDotSetIterConst& setIterator ) const
        {
            if( setIterator != myNormalDotSet.cend() )
            {
                return *setIterator;
            }
            return NormalDotPtr();
        }


        bool TimeModificationNormalTypeNormalDot::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            bool isNormalTypeFound = false;

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if ( importElement( message, *it, isSuccess, *myNormalType, isNormalTypeFound ) ) { continue; }
                importElementSet( message, it, endIter, isSuccess, "normal-dot", myNormalDotSet );
            }

            if( !isNormalTypeFound )
            {
                message << "TimeModificationNormalTypeNormalDot: '" << myNormalType->getElementName() << "' is required but was not found" << std::endl;
            }
            MX_RETURN_IS_SUCCESS;
        }

    }
}
