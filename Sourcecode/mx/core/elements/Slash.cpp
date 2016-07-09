// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/Slash.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/SlashDot.h"
#include "mx/core/elements/SlashType.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Slash::Slash()
        :myAttributes( std::make_shared<SlashAttributes>() )
        ,mySlashType( makeSlashType() )
        ,mySlashDotSet()
        {}


        bool Slash::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& Slash::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& Slash::streamName( std::ostream& os ) const
        {
            os << "slash";
            return os;
        }


        bool Slash::hasContents() const
        {
            return true;
        }


        std::ostream& Slash::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
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


        SlashAttributesPtr Slash::getAttributes() const
        {
            return myAttributes;
        }


        void Slash::setAttributes( const SlashAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        SlashTypePtr Slash::getSlashType() const
        {
            return mySlashType;
        }


        void Slash::setSlashType( const SlashTypePtr& value )
        {
            if( value )
            {
                mySlashType = value;
            }
        }


        const SlashDotSet& Slash::getSlashDotSet() const
        {
            return mySlashDotSet;
        }


        void Slash::removeSlashDot( const SlashDotSetIterConst& value )
        {
            if ( value != mySlashDotSet.cend() )
            {
                mySlashDotSet.erase( value );
            }
        }


        void Slash::addSlashDot( const SlashDotPtr& value )
        {
            if ( value )
            {
                mySlashDotSet.push_back( value );
            }
        }


        void Slash::clearSlashDotSet()
        {
            mySlashDotSet.clear();
        }


        bool Slash::fromXElement( std::ostream& message, xml::XElement& xelement )
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
                message << "Slash: '" << mySlashType->getElementName() << "' is required but was not found" << std::endl;
            }
            MX_RETURN_IS_SUCCESS;
        }

    }
}
