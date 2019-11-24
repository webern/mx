// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Unpitched.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/DisplayStepOctaveGroup.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Unpitched::Unpitched()
        :myDisplayStepOctaveGroup( makeDisplayStepOctaveGroup() )
        ,myHasDisplayStepOctaveGroup( false )
        {}


        bool Unpitched::hasAttributes() const
        {
            return false;
        }


        std::ostream& Unpitched::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& Unpitched::streamName( std::ostream& os ) const
        {
            os << "unpitched";
            return os;
        }


        bool Unpitched::hasContents() const
        {
            return myHasDisplayStepOctaveGroup;
        }


        std::ostream& Unpitched::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
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


        DisplayStepOctaveGroupPtr Unpitched::getDisplayStepOctaveGroup() const
        {
            return myDisplayStepOctaveGroup;
        }


        void Unpitched::setDisplayStepOctaveGroup( const DisplayStepOctaveGroupPtr& value )
        {
            if ( value )
            {
                myDisplayStepOctaveGroup = value;
            }
        }


        bool Unpitched::getHasDisplayStepOctaveGroup() const
        {
            return myHasDisplayStepOctaveGroup;
        }


        void Unpitched::setHasDisplayStepOctaveGroup( const bool value )
        {
            myHasDisplayStepOctaveGroup = value;
        }
        
        
        bool Unpitched::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            bool isSuccess = true;
            
            auto it = xelement.begin();
            auto endIter = xelement.end();
            importGroup( message, it, endIter, isSuccess, myDisplayStepOctaveGroup, myHasDisplayStepOctaveGroup );
            MX_RETURN_IS_SUCCESS;
        }

    }
}
