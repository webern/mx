// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Percussion.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/PercussionChoice.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Percussion::Percussion()
        :myAttributes( std::make_shared<PercussionAttributes>() )
        ,myChoice( makePercussionChoice() )
        {}


        bool Percussion::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& Percussion::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& Percussion::streamName( std::ostream& os ) const
        {
            os << "percussion";
            return os;
        }


        bool Percussion::hasContents() const
        {
            return true;
        }


        std::ostream& Percussion::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            os << std::endl;
            myChoice->streamContents( os, indentLevel+1, isOneLineOnly );
            isOneLineOnly = false;
            os << std::endl;
            return os;
        }


        PercussionAttributesPtr Percussion::getAttributes() const
        {
            return myAttributes;
        }


        void Percussion::setAttributes( const PercussionAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        PercussionChoicePtr Percussion::getPercussionChoice() const
        {
            return myChoice;
        }


        void Percussion::setPercussionChoice( const PercussionChoicePtr& value )
        {
            if( value )
            {
                myChoice = value;
            }
        }


        bool Percussion::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            isSuccess &= myChoice->fromXElement( message, xelement );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
