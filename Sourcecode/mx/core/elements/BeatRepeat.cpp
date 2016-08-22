// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/BeatRepeat.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/SlashDot.h"
#include "mx/core/elements/SlashType.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        BeatRepeat::BeatRepeat()
        :myAttributes( std::make_shared<BeatRepeatAttributes>() )
        ,mySlashType( makeSlashType() )
        ,mySlashDotSet()
        {}


        bool BeatRepeat::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& BeatRepeat::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& BeatRepeat::streamName( std::ostream& os ) const
        {
            os << "beat-repeat";
            return os;
        }


        bool BeatRepeat::hasContents() const
        {
            return true;
        }


        std::ostream& BeatRepeat::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            isOneLineOnly = false;
            os << std::endl;
            mySlashType->toStream( os, indentLevel+1 );
            for ( auto x : mySlashDotSet )
            {
                if ( x )
                {
                    os << std::endl;
                    x->toStream( os, indentLevel+1 );
                }
            }
            os << std::endl;
            return os;
        }


        BeatRepeatAttributesPtr BeatRepeat::getAttributes() const
        {
            return myAttributes;
        }


        void BeatRepeat::setAttributes( const BeatRepeatAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        SlashTypePtr BeatRepeat::getSlashType() const
        {
            return mySlashType;
        }


        void BeatRepeat::setSlashType( const SlashTypePtr& value )
        {
            if( value )
            {
                mySlashType = value;
            }
        }


        const SlashDotSet& BeatRepeat::getSlashDotSet() const
        {
            return mySlashDotSet;
        }


        void BeatRepeat::removeSlashDot( const SlashDotSetIterConst& value )
        {
            if ( value != mySlashDotSet.cend() )
            {
                mySlashDotSet.erase( value );
            }
        }


        void BeatRepeat::addSlashDot( const SlashDotPtr& value )
        {
            if ( value )
            {
                mySlashDotSet.push_back( value );
            }
        }


        void BeatRepeat::clearSlashDotSet()
        {
            mySlashDotSet.clear();
        }


        bool BeatRepeat::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            bool isSlashTypeFound = false;

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if ( importElement( message, *it, isSuccess, *mySlashType, isSlashTypeFound ) ) { continue; }
                importElementSet( message, it, endIter, isSuccess, "slash-dot", mySlashDotSet );
            }

            if( !isSlashTypeFound )
            {
                message << "BeatRepeat: '" << mySlashType->getElementName() << "' is required but was not found" << std::endl;
            }
            MX_RETURN_IS_SUCCESS;
        }

    }
}
