// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/NoteheadText.h"
#include "mx/core/elements/NoteheadTextChoice.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/AccidentalText.h"
#include "mx/core/elements/DisplayText.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        NoteheadText::NoteheadText()
        :myNoteheadTextChoiceSet()
        {
            myNoteheadTextChoiceSet.push_back( makeNoteheadTextChoice() );
        }


        bool NoteheadText::hasAttributes() const
        {
            return false;
        }


        std::ostream& NoteheadText::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& NoteheadText::streamName( std::ostream& os ) const
        {
            os << "notehead-text";
            return os;
        }


        bool NoteheadText::hasContents() const
        {
            return true;
        }


        std::ostream& NoteheadText::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            isOneLineOnly = false;
            for( auto x : myNoteheadTextChoiceSet )
            {
                os << std::endl;
                x->streamContents( os, indentLevel+1, isOneLineOnly );
            }
            os << std::endl;
            return os;
        }


        const NoteheadTextChoiceSet& NoteheadText::getNoteheadTextChoiceSet() const
        {
            return myNoteheadTextChoiceSet;
        }
        
        
        void NoteheadText::removeNoteheadTextChoice( const NoteheadTextChoiceSetIterConst& value )
        {
            if ( value != myNoteheadTextChoiceSet.cend() )
            {
                myNoteheadTextChoiceSet.erase( value );
            }
        }
        
        
        void NoteheadText::addNoteheadTextChoice( const NoteheadTextChoicePtr& value )
        {
            if ( value )
            {
                myNoteheadTextChoiceSet.push_back( value );
            }
        }
        
        
        void NoteheadText::clearNoteheadTextChoiceSet()
        {
            myNoteheadTextChoiceSet.clear();
            myNoteheadTextChoiceSet.push_back( makeNoteheadTextChoice() );
        }


        bool NoteheadText::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            bool isFirstItemAdded = false;
            auto it = xelement.begin();
            auto endIter = xelement.end();
            
            for( ; it != endIter; ++ it )
            {
                if( it->getName() == "accidental-text" )
                {
                    auto item = makeNoteheadTextChoice();
                    item->setChoice( NoteheadTextChoice::Choice::accidentalText );
                    isSuccess &= item->getAccidentalText()->fromXElement( message, xelement );
                    
                    if( !isFirstItemAdded && myNoteheadTextChoiceSet.size() == 1 )
                    {
                        *myNoteheadTextChoiceSet.begin() = item;
                        isFirstItemAdded = true;
                    }
                    else
                    {
                        myNoteheadTextChoiceSet.push_back( item );
                        isFirstItemAdded = true;
                    }
                }
                else if( it->getName() == "display-text" )
                {
                    auto item = makeNoteheadTextChoice();
                    item->setChoice( NoteheadTextChoice::Choice::displayText );
                    isSuccess &= item->getDisplayText()->fromXElement( message, xelement );
                    
                    if( !isFirstItemAdded && myNoteheadTextChoiceSet.size() == 1 )
                    {
                        *myNoteheadTextChoiceSet.begin() = item;
                        isFirstItemAdded = true;
                    }
                    else
                    {
                        myNoteheadTextChoiceSet.push_back( item );
                        isFirstItemAdded = true;
                    }
                }
                
            }
            
            MX_RETURN_IS_SUCCESS;
        }

    }
}
