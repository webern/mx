// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/Stick.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/StickMaterial.h"
#include "mx/core/elements/StickType.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Stick::Stick()
        :myAttributes( std::make_shared<StickAttributes>() )
        ,myStickType( makeStickType() )
        ,myStickMaterial( makeStickMaterial() )
        {}


        bool Stick::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& Stick::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& Stick::streamName( std::ostream& os ) const
        {
            os << "stick";
            return os;
        }


        bool Stick::hasContents() const
        {
            return true;
        }


        std::ostream& Stick::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            isOneLineOnly = false;
            os << std::endl;
            myStickType->toStream( os, indentLevel+1 );
            os << std::endl;
            myStickMaterial->toStream( os, indentLevel+1 );
            os << std::endl;
            return os;
        }


        StickAttributesPtr Stick::getAttributes() const
        {
            return myAttributes;
        }


        void Stick::setAttributes( const StickAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        StickTypePtr Stick::getStickType() const
        {
            return myStickType;
        }


        void Stick::setStickType( const StickTypePtr& value )
        {
            if( value )
            {
                myStickType = value;
            }
        }


        StickMaterialPtr Stick::getStickMaterial() const
        {
            return myStickMaterial;
        }


        void Stick::setStickMaterial( const StickMaterialPtr& value )
        {
            if( value )
            {
                myStickMaterial = value;
            }
        }


        bool Stick::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            bool isStickTypeFound = false;
            bool isStickMaterialFound = false;

            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if ( importElement( message, *it, isSuccess, *myStickType, isStickTypeFound ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myStickMaterial, isStickMaterialFound ) ) { continue; }
            }

            if( !isStickTypeFound )
            {
                message << "Stick: '" << myStickType->getElementName() << "' is required but was not found" << std::endl;
            }
            if( !isStickMaterialFound )
            {
                message << "Stick: '" << myStickMaterial->getElementName() << "' is required but was not found" << std::endl;
            }
            MX_RETURN_IS_SUCCESS;
        }

    }
}
