// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Lyric.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/EditorialGroup.h"
#include "mx/core/elements/EndLine.h"
#include "mx/core/elements/EndParagraph.h"
#include "mx/core/elements/Extend.h"
#include "mx/core/elements/Humming.h"
#include "mx/core/elements/Laughing.h"
#include "mx/core/elements/LyricTextChoice.h"
#include "mx/core/elements/SyllabicTextGroup.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Lyric::Lyric()
        :myAttributes( std::make_shared<LyricAttributes>() )
        ,myLyricTextChoice( makeLyricTextChoice() )
        ,myEndLine( makeEndLine() )
        ,myHasEndLine( false )
        ,myEndParagraph( makeEndParagraph() )
        ,myHasEndParagraph( false )
        ,myEditorialGroup( makeEditorialGroup() )
        {}


        bool Lyric::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& Lyric::streamAttributes( std::ostream& os ) const
        {
            if ( myAttributes->hasValues() )
            {
                myAttributes->toStream( os );
            }
            return os;
        }


        std::ostream& Lyric::streamName( std::ostream& os ) const
        {
            return os << "lyric";
        }


        bool Lyric::hasContents() const
        {
            return true;
        }


        LyricAttributesPtr Lyric::getAttributes() const
        {
            return myAttributes;
        }


        void Lyric::setAttributes( const LyricAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        std::ostream& Lyric::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            os << std::endl;
            myLyricTextChoice->streamContents( os, indentLevel+1, isOneLineOnly );
            if ( myHasEndLine )
            {
                os << std::endl;
                myEndLine->toStream( os, indentLevel+1 );
            }
            if ( myHasEndParagraph )
            {
                os << std::endl;
                myEndParagraph->toStream( os, indentLevel+1 );
            }
            if( myEditorialGroup->hasContents() )
            {
                os << std::endl;
            }
            myEditorialGroup->streamContents( os, indentLevel+1, isOneLineOnly );
            os << std::endl;
            isOneLineOnly = false;
            return os;
        }


        LyricTextChoicePtr Lyric::getLyricTextChoice() const
        {
            return myLyricTextChoice;
        }


        void Lyric::setLyricTextChoice( const LyricTextChoicePtr& value )
        {
            if ( value )
            {
                myLyricTextChoice = value;
            }
        }


        EndLinePtr Lyric::getEndLine() const
        {
            return myEndLine;
        }


        void Lyric::setEndLine( const EndLinePtr& value )
        {
            if ( value )
            {
                myEndLine = value;
            }
        }


        bool Lyric::getHasEndLine() const
        {
            return myHasEndLine;
        }


        void Lyric::setHasEndLine( const bool value )
        {
            myHasEndLine = value;
        }


        EndParagraphPtr Lyric::getEndParagraph() const
        {
            return myEndParagraph;
        }


        void Lyric::setEndParagraph( const EndParagraphPtr& value )
        {
            if ( value )
            {
                myEndParagraph = value;
            }
        }


        bool Lyric::getHasEndParagraph() const
        {
            return myHasEndParagraph;
        }


        void Lyric::setHasEndParagraph( const bool value )
        {
            myHasEndParagraph = value;
        }


        EditorialGroupPtr Lyric::getEditorialGroup() const
        {
            return myEditorialGroup;
        }


        void Lyric::setEditorialGroup( const EditorialGroupPtr& value )
        {
            if ( value )
            {
                myEditorialGroup = value;
            }
        }


        bool Lyric::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            bool isLyricTextChoiceFound = false;
            
            auto endIter = xelement.end();
            for( auto it = xelement.begin(); it != endIter; ++it )
            {

                if( checkSetChoiceMember(
                    message, *it, isSuccess, myLyricTextChoice,
                    "extend", &LyricTextChoice::getExtend,
                    static_cast<int>( LyricTextChoice::Choice::extend ) ) )
                { isLyricTextChoiceFound = true; continue; }

                else if( checkSetChoiceMember(
                    message, *it, isSuccess, myLyricTextChoice,
                    "laughing", &LyricTextChoice::getLaughing,
                    static_cast<int>( LyricTextChoice::Choice::laughing ) ) )
                { isLyricTextChoiceFound = true; continue; }

                else if( checkSetChoiceMember(
                    message, *it, isSuccess, myLyricTextChoice,
                    "humming", &LyricTextChoice::getHumming,
                    static_cast<int>( LyricTextChoice::Choice::humming ) ) )
                { isLyricTextChoiceFound = true; continue; }
                
                else
                {
                    myLyricTextChoice->setChoice( LyricTextChoice::Choice::syllabicTextGroup );
                    SyllabicTextGroupPtr ptr = myLyricTextChoice->getSyllabicTextGroup();
                    importGroup( message, it, endIter, isSuccess, ptr );
                }
                if ( importElement( message, *it, isSuccess, *myEndLine, myHasEndLine ) ) { continue; }
                if ( importElement( message, *it, isSuccess, *myEndParagraph, myHasEndParagraph ) ) { continue; }
                importGroup( message, it, endIter, isSuccess, myEditorialGroup );

            }

            MX_RETURN_IS_SUCCESS;
        }

    }
}
