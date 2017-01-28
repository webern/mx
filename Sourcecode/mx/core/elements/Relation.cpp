// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Relation.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Relation::Relation()
        :myValue()
        ,myAttributes( std::make_shared<RelationAttributes>() )
        {}


        Relation::Relation( const XsString& value )
        :myValue( value )
        ,myAttributes( std::make_shared<RelationAttributes>() )
        {}


        bool Relation::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Relation::hasContents() const
        {
            return true;
        }


        std::ostream& Relation::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Relation::streamName( std::ostream& os ) const
        {
            os << "relation";
            return os;
        }


        std::ostream& Relation::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly  ) const
        {
            MX_UNUSED( indentLevel );
            isOneLineOnly = true;
            os << myValue;
            return os;
        }


        RelationAttributesPtr Relation::getAttributes() const
        {
            return myAttributes;
        }


        void Relation::setAttributes( const RelationAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        XsString Relation::getValue() const
        {
            return myValue;
        }


        void Relation::setValue( const XsString& value )
        {
            myValue = value;
        }


        bool Relation::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            myValue.setValue( xelement.getValue() );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
