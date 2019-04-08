// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/ElisionSyllabicGroup.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/Elision.h"
#include "mx/core/elements/Syllabic.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        ElisionSyllabicGroup::ElisionSyllabicGroup()
        :myElision( makeElision() )
        ,mySyllabic( makeSyllabic() )
        ,myHasSyllabic( false )
        {}


        bool ElisionSyllabicGroup::hasAttributes() const
        {
            return false;
        }


        std::ostream& ElisionSyllabicGroup::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& ElisionSyllabicGroup::streamName( std::ostream& os ) const
        {
            return os;
        }


        bool ElisionSyllabicGroup::hasContents() const
        {
            return true;
        }


        std::ostream& ElisionSyllabicGroup::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            isOneLineOnly = true;
            myElision->toStream( os, indentLevel );
            if ( myHasSyllabic )
            {
                isOneLineOnly = false;
                os << std::endl;
                mySyllabic->toStream( os, indentLevel );
            }
            return os;
        }


        ElisionPtr ElisionSyllabicGroup::getElision() const
        {
            return myElision;
        }


        void ElisionSyllabicGroup::setElision( const ElisionPtr& value )
        {
            if ( value )
            {
                myElision = value;
            }
        }


        SyllabicPtr ElisionSyllabicGroup::getSyllabic() const
        {
            return mySyllabic;
        }


        void ElisionSyllabicGroup::setSyllabic( const SyllabicPtr& value )
        {
            if ( value )
            {
                mySyllabic = value;
            }
        }


        bool ElisionSyllabicGroup::getHasSyllabic() const
        {
            return myHasSyllabic;
        }


        void ElisionSyllabicGroup::setHasSyllabic( const bool value )
        {
            myHasSyllabic = value;
        }
        
        
        MX_FROM_XELEMENT_UNUSED( ElisionSyllabicGroup );
        
    }
}
