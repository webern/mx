// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/DisplayTextOrAccidentalText.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/AccidentalText.h"
#include "mx/core/elements/DisplayText.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        DisplayTextOrAccidentalText::DisplayTextOrAccidentalText()
        :myChoice( Choice::displayText )
        ,myDisplayText( makeDisplayText() )
        ,myAccidentalText( makeAccidentalText() )
        {}


        bool DisplayTextOrAccidentalText::hasAttributes() const
        {
            return false;
        }


        std::ostream& DisplayTextOrAccidentalText::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& DisplayTextOrAccidentalText::streamName( std::ostream& os ) const
        {
            return os;
        }


        bool DisplayTextOrAccidentalText::hasContents() const
        {
            if ( myChoice == Choice::accidentalText )
            {
                return myAccidentalText->hasContents();
            }
            if ( myChoice == Choice::displayText )
            {
                return myDisplayText->hasContents();
            }
            return false;
        }


        std::ostream& DisplayTextOrAccidentalText::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            MX_UNUSED( isOneLineOnly );
            if ( hasContents() )
            {
                switch ( myChoice )
                {
                    case Choice::accidentalText:
                    {
                        myAccidentalText->toStream( os, indentLevel );
                    }
                        break;
                    case Choice::displayText:
                    {
                        myDisplayText->toStream( os, indentLevel );
                    }
                        break;
                    default:
                        break;
                }
            }
            return os;
        }


        DisplayTextOrAccidentalText::Choice DisplayTextOrAccidentalText::getChoice() const
        {
            return myChoice;
        }


        void DisplayTextOrAccidentalText::setChoice( const DisplayTextOrAccidentalText::Choice value )
        {
            myChoice = value;
        }


        DisplayTextPtr DisplayTextOrAccidentalText::getDisplayText() const
        {
            return myDisplayText;
        }


        void DisplayTextOrAccidentalText::setDisplayText( const DisplayTextPtr& value )
        {
            if ( value )
            {
                myDisplayText = value;
            }
        }


        AccidentalTextPtr DisplayTextOrAccidentalText::getAccidentalText() const
        {
            return myAccidentalText;
        }


        void DisplayTextOrAccidentalText::setAccidentalText( const AccidentalTextPtr& value )
        {
            if ( value )
            {
                myAccidentalText = value;
            }
        }


        bool DisplayTextOrAccidentalText::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            MX_CHOICE_IF( displayText, "display-text", DisplayText );
            MX_CHOICE_IF( accidentalText, "accidental-text", AccidentalText );
            MX_BAD_ELEMENT_FAILURE( DisplayTextOrAccidentalText );
        }

    }
}
