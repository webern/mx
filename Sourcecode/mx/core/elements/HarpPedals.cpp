// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/HarpPedals.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/PedalTuning.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        HarpPedals::HarpPedals()
        :myAttributes( std::make_shared<HarpPedalsAttributes>() )
        ,myPedalTuningSet()
        {
            myPedalTuningSet.push_back( makePedalTuning() );
        }


        bool HarpPedals::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& HarpPedals::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& HarpPedals::streamName( std::ostream& os ) const
        {
            os << "harp-pedals";
            return os;
        }


        bool HarpPedals::hasContents() const
        {
            return true;
        }


        std::ostream& HarpPedals::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            isOneLineOnly = false;
            for ( auto x : myPedalTuningSet )
            {
                os << std::endl;
                x->toStream( os, indentLevel+1 );
            }
            os << std::endl;
            return os;
        }


        HarpPedalsAttributesPtr HarpPedals::getAttributes() const
        {
            return myAttributes;
        }


        void HarpPedals::setAttributes( const HarpPedalsAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        const PedalTuningSet& HarpPedals::getPedalTuningSet() const
        {
            return myPedalTuningSet;
        }


        void HarpPedals::removePedalTuning( const PedalTuningSetIterConst& value )
        {
            if ( value != myPedalTuningSet.cend() )
            {
                if ( myPedalTuningSet.size() > 1 )
                {
                    myPedalTuningSet.erase( value );
                }
            }
        }


        void HarpPedals::addPedalTuning( const PedalTuningPtr& value )
        {
            if ( value )
            {
                myPedalTuningSet.push_back( value );
            }
        }


        void HarpPedals::clearPedalTuningSet()
        {
            myPedalTuningSet.clear();
            myPedalTuningSet.push_back( makePedalTuning() );
        }


        PedalTuningPtr HarpPedals::getPedalTuning( const PedalTuningSetIterConst& setIterator ) const
        {
            if( setIterator != myPedalTuningSet.cend() )
            {
                return *setIterator;
            }
            return PedalTuningPtr();
        }


        bool HarpPedals::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                importElementSet( message, it, endIter, isSuccess, "pedal-tuning", myPedalTuningSet );
            }

            MX_RETURN_IS_SUCCESS;
        }

    }
}
