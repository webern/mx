// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/Interchangeable.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/BeatType.h"
#include "mx/core/elements/Beats.h"
#include "mx/core/elements/TimeRelation.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Interchangeable::Interchangeable()
        :myAttributes( std::make_shared<InterchangeableAttributes>() )
        ,myTimeRelation( makeTimeRelation() )
        ,myHasTimeRelation( false )
        ,myBeats( makeBeats() )
        ,myBeatType( makeBeatType() )
        {}


        bool Interchangeable::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& Interchangeable::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& Interchangeable::streamName( std::ostream& os ) const
        {
            os << "interchangeable";
            return os;
        }


        bool Interchangeable::hasContents() const
        {
            return true;
        }


        std::ostream& Interchangeable::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            isOneLineOnly = false;
            if( myHasTimeRelation )
            {
                os << std::endl;
                myTimeRelation->toStream( os, indentLevel+1 );
            }
            os << std::endl;
            myBeats->toStream( os, indentLevel+1 );
            os << std::endl;
            myBeatType->toStream( os, indentLevel+1 );
            os << std::endl;
            return os;
        }


        InterchangeableAttributesPtr Interchangeable::getAttributes() const
        {
            return myAttributes;
        }


        void Interchangeable::setAttributes( const InterchangeableAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        TimeRelationPtr Interchangeable::getTimeRelation() const
        {
            return myTimeRelation;
        }


        void Interchangeable::setTimeRelation( const TimeRelationPtr& value )
        {
            if( value )
            {
                myTimeRelation = value;
            }
        }


        bool Interchangeable::getHasTimeRelation() const
        {
            return myHasTimeRelation;
        }


        void Interchangeable::setHasTimeRelation( const bool value )
        {
            myHasTimeRelation = value;
        }


        BeatsPtr Interchangeable::getBeats() const
        {
            return myBeats;
        }


        void Interchangeable::setBeats( const BeatsPtr& value )
        {
            if( value )
            {
                myBeats = value;
            }
        }


        BeatTypePtr Interchangeable::getBeatType() const
        {
            return myBeatType;
        }


        void Interchangeable::setBeatType( const BeatTypePtr& value )
        {
            if( value )
            {
                myBeatType = value;
            }
        }


        bool Interchangeable::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            bool isBeatsFound = false;
            bool isBeatTypeFound = false;

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if ( importElement( message, *it, isSuccess, *myTimeRelation, myHasTimeRelation ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myBeats, isBeatsFound ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myBeatType, isBeatTypeFound ) ) { continue; }
            }

            if( !isBeatsFound )
            {
                message << "Interchangeable: '" << myBeats->getElementName() << "' is required but was not found" << std::endl;
            }
            
            if( !isBeatTypeFound )
            {
                message << "Interchangeable: '" << myBeatType->getElementName() << "' is required but was not found" << std::endl;
            }
            
            MX_RETURN_IS_SUCCESS;
        }

    }
}
