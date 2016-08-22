// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/EditorialVoiceGroup.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/Footnote.h"
#include "mx/core/elements/Level.h"
#include "mx/core/elements/Voice.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        EditorialVoiceGroup::EditorialVoiceGroup()
        :myFootnote( makeFootnote() )
        ,myHasFootnote( false )
        ,myLevel( makeLevel() )
        ,myHasLevel( false )
        ,myVoice( makeVoice() )
        ,myHasVoice( false )
        {}


        bool EditorialVoiceGroup::hasAttributes() const
        {
            return false;
        }


        std::ostream& EditorialVoiceGroup::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& EditorialVoiceGroup::streamName( std::ostream& os ) const
        {
            return os;
        }


        bool EditorialVoiceGroup::hasContents() const
        {
            return myHasFootnote || myHasLevel || myHasVoice;
        }


        std::ostream& EditorialVoiceGroup::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            bool firstItem = true;
            isOneLineOnly = true;
            if ( myHasFootnote )
            {
                myFootnote->toStream( os, indentLevel );
                firstItem = false;
            }
            if ( myHasLevel )
            {
                if( ! firstItem ) os << std::endl;
                myLevel->toStream( os, indentLevel );
                firstItem = false;
            }
            if ( myHasVoice )
            {
                if( ! firstItem ) os << std::endl;
                myVoice->toStream( os, indentLevel );
                firstItem = false;
            }
            isOneLineOnly = ! hasContents();
            return os;
        }


        FootnotePtr EditorialVoiceGroup::getFootnote() const
        {
            return myFootnote;
        }


        void EditorialVoiceGroup::setFootnote( const FootnotePtr& value )
        {
            if ( value )
            {
                myFootnote = value;
            }
        }


        bool EditorialVoiceGroup::getHasFootnote() const
        {
            return myHasFootnote;
        }


        void EditorialVoiceGroup::setHasFootnote( const bool value )
        {
            myHasFootnote = value;
        }


        LevelPtr EditorialVoiceGroup::getLevel() const
        {
            return myLevel;
        }


        void EditorialVoiceGroup::setLevel( const LevelPtr& value )
        {
            if ( value )
            {
                myLevel = value;
            }
        }


        bool EditorialVoiceGroup::getHasLevel() const
        {
            return myHasLevel;
        }


        void EditorialVoiceGroup::setHasLevel( const bool value )
        {
            myHasLevel = value;
        }


        VoicePtr EditorialVoiceGroup::getVoice() const
        {
            return myVoice;
        }


        void EditorialVoiceGroup::setVoice( const VoicePtr& value )
        {
            if ( value )
            {
                myVoice = value;
            }
        }


        bool EditorialVoiceGroup::getHasVoice() const
        {
            return myHasVoice;
        }


        void EditorialVoiceGroup::setHasVoice( const bool value )
        {
            myHasVoice = value;
        }
        
        
        MX_FROM_XELEMENT_UNUSED( EditorialVoiceGroup );

    }
}
