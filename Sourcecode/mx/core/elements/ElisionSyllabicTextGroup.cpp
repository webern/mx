// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/ElisionSyllabicTextGroup.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/ElisionSyllabicGroup.h"
#include "mx/core/elements/Text.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        ElisionSyllabicTextGroup::ElisionSyllabicTextGroup()
        :myElisionSyllabicGroup( makeElisionSyllabicGroup() )
        ,myHasElisionSyllabicGroup( false )
        ,myText( makeText() )
        {}


        bool ElisionSyllabicTextGroup::hasAttributes() const
        {
            return false;
        }


        std::ostream& ElisionSyllabicTextGroup::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& ElisionSyllabicTextGroup::streamName( std::ostream& os ) const
        {
            return os;
        }


        bool ElisionSyllabicTextGroup::hasContents() const
        {
            return true;
        }


        std::ostream& ElisionSyllabicTextGroup::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            isOneLineOnly = true;
            if ( myHasElisionSyllabicGroup )
            {
                myElisionSyllabicGroup->streamContents( os, indentLevel, isOneLineOnly );
                isOneLineOnly = false;
                os << std::endl;
            }
            myText->toStream( os, indentLevel );
            return os;
        }


        ElisionSyllabicGroupPtr ElisionSyllabicTextGroup::getElisionSyllabicGroup() const
        {
            return myElisionSyllabicGroup;
        }


        void ElisionSyllabicTextGroup::setElisionSyllabicGroup( const ElisionSyllabicGroupPtr& value )
        {
            if ( value )
            {
                myElisionSyllabicGroup = value;
            }
        }


        bool ElisionSyllabicTextGroup::getHasElisionSyllabicGroup() const
        {
            return myHasElisionSyllabicGroup;
        }


        void ElisionSyllabicTextGroup::setHasElisionSyllabicGroup( const bool value )
        {
            myHasElisionSyllabicGroup = value;
        }


        TextPtr ElisionSyllabicTextGroup::getText() const
        {
            return myText;
        }


        void ElisionSyllabicTextGroup::setText( const TextPtr& value )
        {
            if ( value )
            {
                myText = value;
            }
        }
        
        
        MX_FROM_XELEMENT_UNUSED( ElisionSyllabicTextGroup );

    }
}
