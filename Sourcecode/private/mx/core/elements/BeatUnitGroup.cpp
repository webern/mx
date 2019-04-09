// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/BeatUnitGroup.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/BeatUnit.h"
#include "mx/core/elements/BeatUnitDot.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        BeatUnitGroup::BeatUnitGroup()
        :myBeatUnit( makeBeatUnit() )
        ,myBeatUnitDotSet()
        {}


        bool BeatUnitGroup::hasAttributes() const
        {
            return false;
        }


        std::ostream& BeatUnitGroup::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& BeatUnitGroup::streamName( std::ostream& os ) const
        {
            return os;
        }


        bool BeatUnitGroup::hasContents() const
        {
            return true;
        }


        std::ostream& BeatUnitGroup::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            isOneLineOnly = false;
            myBeatUnit->toStream( os, indentLevel );
            for ( auto x : myBeatUnitDotSet )
            {
                os << std::endl;
                x->toStream( os, indentLevel );
            }
            return os;
        }


        BeatUnitPtr BeatUnitGroup::getBeatUnit() const
        {
            return myBeatUnit;
        }


        void BeatUnitGroup::setBeatUnit( const BeatUnitPtr& value )
        {
            if ( value )
            {
                myBeatUnit = value;
            }
        }


        const BeatUnitDotSet& BeatUnitGroup::getBeatUnitDotSet() const
        {
            return myBeatUnitDotSet;
        }


        void BeatUnitGroup::addBeatUnitDot( const BeatUnitDotPtr& value )
        {
            if ( value )
            {
                myBeatUnitDotSet.push_back( value );
            }
        }


        void BeatUnitGroup::removeBeatUnitDot( const BeatUnitDotSetIterConst& setIterator )
        {
            if ( setIterator != myBeatUnitDotSet.cend() )
            {
                myBeatUnitDotSet.erase( setIterator );
            }
        }


        void BeatUnitGroup::clearBeatUnitDotSet()
        {
            myBeatUnitDotSet.clear();
        }
        
        bool BeatUnitGroup::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            bool isBeatUnitFound = false;
            bool isFirstBeatUnitDotAdded = false;
            
            for( auto it = xelement.begin(); it != xelement.end(); ++it )
            {
                const std::string elementName = it->getName();
                
                if( elementName == "beat-unit" )
                {
                    isBeatUnitFound = true;
                    isSuccess &= myBeatUnit->fromXElement( message, *it );
                }
                else if( elementName == "beat-unit-dot" )
                {
                    auto beatUnitDot = makeBeatUnitDot();
                    isSuccess &= beatUnitDot->fromXElement( message, *it );
                    
                    if( !isFirstBeatUnitDotAdded && myBeatUnitDotSet.size() == 1 )
                    {
                        *( myBeatUnitDotSet.begin() ) = beatUnitDot;
                        isFirstBeatUnitDotAdded = true;
                    }
                    else
                    {
                        myBeatUnitDotSet.push_back( beatUnitDot );
                        isFirstBeatUnitDotAdded = true;
                    }
                }
                else
                {
                    if( !isBeatUnitFound )
                    {
                        isSuccess = false;
                        message << "BeatUnitGroup: 'beat-unit' element is required but was not found" << std::endl;
                    }
                    break;
                }
            }
            MX_RETURN_IS_SUCCESS;
        }

    }
}
