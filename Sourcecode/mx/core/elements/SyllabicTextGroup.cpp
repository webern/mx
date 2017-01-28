// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/SyllabicTextGroup.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/ElisionSyllabicTextGroup.h"
#include "mx/core/elements/Extend.h"
#include "mx/core/elements/Syllabic.h"
#include "mx/core/elements/Text.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        SyllabicTextGroup::SyllabicTextGroup()
        :mySyllabic( makeSyllabic() )
        ,myHasSyllabic( false )
        ,myText( makeText() )
        ,myElisionSyllabicTextGroupSet()
        ,myExtend( makeExtend() )
        ,myHasExtend( false )
        {}


        bool SyllabicTextGroup::hasAttributes() const
        {
            return false;
        }


        std::ostream& SyllabicTextGroup::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& SyllabicTextGroup::streamName( std::ostream& os ) const
        {
            return os;
        }


        bool SyllabicTextGroup::hasContents() const
        {
            return true;
        }


        std::ostream& SyllabicTextGroup::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            isOneLineOnly = true;
            if ( myHasSyllabic )
            {
                mySyllabic->toStream( os, indentLevel );
                os << std::endl;
            }
            myText->toStream( os, indentLevel );
            for ( auto x : myElisionSyllabicTextGroupSet )
            {
                os << std::endl;
                x->streamContents( os, indentLevel, isOneLineOnly );
            }
            if ( myHasExtend )
            {
                os << std::endl;
                myExtend->toStream( os, indentLevel );
            }
            isOneLineOnly = ! ( myHasSyllabic || myElisionSyllabicTextGroupSet.size() > 0 || myHasExtend );
            return os;
        }


        SyllabicPtr SyllabicTextGroup::getSyllabic() const
        {
            return mySyllabic;
        }


        void SyllabicTextGroup::setSyllabic( const SyllabicPtr& value )
        {
            if ( value )
            {
                mySyllabic = value;
            }
        }


        bool SyllabicTextGroup::getHasSyllabic() const
        {
            return myHasSyllabic;
        }


        void SyllabicTextGroup::setHasSyllabic( const bool value )
        {
            myHasSyllabic = value;
        }


        TextPtr SyllabicTextGroup::getText() const
        {
            return myText;
        }


        void SyllabicTextGroup::setText( const TextPtr& value )
        {
            if ( value )
            {
                myText = value;
            }
        }


        const ElisionSyllabicTextGroupSet& SyllabicTextGroup::getElisionSyllabicTextGroupSet() const
        {
            return myElisionSyllabicTextGroupSet;
        }


        void SyllabicTextGroup::addElisionSyllabicTextGroup( const ElisionSyllabicTextGroupPtr& value )
        {
            if ( value )
            {
                myElisionSyllabicTextGroupSet.push_back( value );
            }
        }


        void SyllabicTextGroup::removeElisionSyllabicTextGroup( const ElisionSyllabicTextGroupSetIterConst& value )
        {
            if ( value != myElisionSyllabicTextGroupSet.cend() )
            {
                myElisionSyllabicTextGroupSet.erase( value );
            }
        }


        void SyllabicTextGroup::clearElisionSyllabicTextGroupSet()
        {
            myElisionSyllabicTextGroupSet.clear();
        }


        ElisionSyllabicTextGroupPtr SyllabicTextGroup::getElisionSyllabicTextGroup( const ElisionSyllabicTextGroupSetIterConst& setIterator ) const
        {
            if( setIterator != myElisionSyllabicTextGroupSet.cend() )
            {
                return *setIterator;
            }
            return ElisionSyllabicTextGroupPtr();
        }


        ExtendPtr SyllabicTextGroup::getExtend() const
        {
            return myExtend;
        }


        void SyllabicTextGroup::setExtend( const ExtendPtr& value )
        {
            if ( value )
            {
                myExtend = value;
            }
        }


        bool SyllabicTextGroup::getHasExtend() const
        {
            return myHasExtend;
        }


        void SyllabicTextGroup::setHasExtend( const bool value )
        {
            myHasExtend = value;
        }


        MX_FROM_XELEMENT_UNUSED( SyllabicTextGroup );

    }
}
