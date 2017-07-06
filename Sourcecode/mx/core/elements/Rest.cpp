// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Rest.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/DisplayStepOctaveGroup.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Rest::Rest()
        :myAttributes( std::make_shared<RestAttributes>() )
        ,myDisplayStepOctaveGroup( makeDisplayStepOctaveGroup() )
        ,myHasDisplayStepOctaveGroup( false )
        {}


        bool Rest::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& Rest::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& Rest::streamName( std::ostream& os ) const
        {
            os << "rest";
            return os;
        }


        bool Rest::hasContents() const
        {
            return myHasDisplayStepOctaveGroup;
        }


        std::ostream& Rest::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            if ( myHasDisplayStepOctaveGroup )
            {
                os << std::endl;
                myDisplayStepOctaveGroup->streamContents( os, indentLevel+1, isOneLineOnly );
                isOneLineOnly = false;
                os << std::endl;
            }
            else
            {
                isOneLineOnly = true;
            }
            return os;
        }


        RestAttributesPtr Rest::getAttributes() const
        {
            return myAttributes;
        }


        void Rest::setAttributes( const RestAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        DisplayStepOctaveGroupPtr Rest::getDisplayStepOctaveGroup() const
        {
            return myDisplayStepOctaveGroup;
        }


        void Rest::setDisplayStepOctaveGroup( const DisplayStepOctaveGroupPtr& value )
        {
            if ( value )
            {
                myDisplayStepOctaveGroup = value;
            }
        }


        bool Rest::getHasDisplayStepOctaveGroup() const
        {
            return myHasDisplayStepOctaveGroup;
        }


        void Rest::setHasDisplayStepOctaveGroup( const bool value )
        {
            myHasDisplayStepOctaveGroup = value;
        }


        bool Rest::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );

            auto it = xelement.begin();
            auto endIter = xelement.end();
            importGroup( message, it, endIter, isSuccess, myDisplayStepOctaveGroup, myHasDisplayStepOctaveGroup );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
