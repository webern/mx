// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/NoteheadTextChoice.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/AccidentalText.h"
#include "mx/core/elements/DisplayText.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        NoteheadTextChoice::NoteheadTextChoice()
        :myChoice( Choice::displayText )
        ,myDisplayText( makeDisplayText() )
        ,myAccidentalText( makeAccidentalText() )
        {}
        
        
        bool NoteheadTextChoice::hasAttributes() const
        {
            return false;
        }
        
        
        std::ostream& NoteheadTextChoice::streamAttributes( std::ostream& os ) const
        {
            return os;
        }
        
        
        std::ostream& NoteheadTextChoice::streamName( std::ostream& os ) const
        {
            os << "notehead-text";
            return os;
        }
        
        
        bool NoteheadTextChoice::hasContents() const
        {
            return true;
        }
        
        
        std::ostream& NoteheadTextChoice::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( indentLevel );
            MX_UNUSED( isOneLineOnly );
            switch ( myChoice )
            {
                case Choice::displayText:
                    myDisplayText->toStream( os, indentLevel );
                    break;
                case Choice::accidentalText:
                    myAccidentalText->toStream( os, indentLevel );
                    break;
                default:
                    break;
            }
            return os;
        }
        
        
        NoteheadTextChoice::Choice NoteheadTextChoice::getChoice() const
        {
            return myChoice;
        }
        
        
        void NoteheadTextChoice::setChoice( NoteheadTextChoice::Choice value )
        {
            myChoice = value;
        }
        
        
        DisplayTextPtr NoteheadTextChoice::getDisplayText() const
        {
            return myDisplayText;
        }
        
        
        void NoteheadTextChoice::setDisplayText( const DisplayTextPtr& value )
        {
            if( value )
            {
                myDisplayText = value;
            }
        }
        
        
        AccidentalTextPtr NoteheadTextChoice::getAccidentalText() const
        {
            return myAccidentalText;
        }
        
        
        void NoteheadTextChoice::setAccidentalText( const AccidentalTextPtr& value )
        {
            if( value )
            {
                myAccidentalText = value;
            }
        }
        
        
        bool NoteheadTextChoice::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            
            if( xelement.getName() == "display-text" )
            {
                myChoice = Choice::displayText;
                return getDisplayText()->fromXElement( message, xelement );
            }
            
            if( xelement.getName() == "accidental-text" )
            {
                myChoice = Choice::accidentalText;
                return getAccidentalText()->fromXElement( message, xelement );
            }
            
            message << "NoteheadTextChoice: '" << xelement.getName() << "' is not a valid element" << std::endl;
            
            return false;
        }
        
    }
}
