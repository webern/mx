// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Dynamics.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Dynamics::Dynamics( const DynamicsValue& value )
        :myValue( value )
        ,myAttributes( std::make_shared<DynamicsAttributes>() )
        {}


        Dynamics::Dynamics()
        :myValue()
        ,myAttributes( std::make_shared<DynamicsAttributes>() )
        {}
        std::ostream& Dynamics::streamName( std::ostream& os ) const
        {
            return os << "dynamics";
        }


        std::ostream& Dynamics::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            isOneLineOnly = false;
            os << std::endl;
            if ( myValue.getValue() == DynamicsEnum::otherDynamics )
            {
                indent( os, indentLevel + 1 );
                os << "<";
                core::toStream( os, myValue.getValue() );
                os << ">";
                os << myValue;
                os << "</";
                core::toStream( os, myValue.getValue() );
                os << ">";
            }
            else
            {
                indent( os, indentLevel + 1 );
                os << "<";
                core::toStream( os, myValue.getValue() );
                os << "/>";
            }
            os << std::endl;
            return os;
        }


        DynamicsAttributesPtr Dynamics::getAttributes() const
        {
            return myAttributes;
        }


        bool Dynamics::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        bool Dynamics::hasContents() const
        {
            return true;
        }


        std::ostream& Dynamics::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        DynamicsValue Dynamics::getValue() const
        {
            return myValue;
        }


        void Dynamics::setValue( const DynamicsValue& value )
        {
            myValue = value;
        }


        bool Dynamics::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            
            
            auto b = xelement.begin();
            auto e = xelement.end();
            
            if( b != e )
            {
                if( b->getName() == "other-dynamics" )
                {
                    myValue.setValue( b->getValue() );
                    MX_RETURN_IS_SUCCESS;
                }
                else
                {
                    myValue.setValue( b->getName() );
                    MX_RETURN_IS_SUCCESS;
                }
            }
            
            message << "Dynamics: parsing failed" << std::endl;
            return false;
        }

    }
}
