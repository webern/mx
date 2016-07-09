// MusicXML Class Library v0.2
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/elements/Time.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/TimeChoice.h"
#include "mx/core/elements/SenzaMisura.h"
#include "mx/core/elements/TimeSignatureGroup.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        Time::Time()
        :myAttributes( std::make_shared<TimeAttributes>() )
        ,myChoice( makeTimeChoice() )
        {}


        bool Time::hasAttributes() const
        {
            return myAttributes->hasValues();
        }


        std::ostream& Time::streamAttributes( std::ostream& os ) const
        {
            return myAttributes->toStream( os );
        }


        std::ostream& Time::streamName( std::ostream& os ) const
        {
            os << "time";
            return os;
        }


        bool Time::hasContents() const
        {
            return true;
        }


        std::ostream& Time::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            os << std::endl;
            myChoice->streamContents( os, indentLevel+1, isOneLineOnly );
            os << std::endl;
            isOneLineOnly = false;
            return os;
        }


        TimeAttributesPtr Time::getAttributes() const
        {
            return myAttributes;
        }


        void Time::setAttributes( const TimeAttributesPtr& value )
        {
            if ( value )
            {
                myAttributes = value;
            }
        }


        TimeChoicePtr Time::getTimeChoice() const
        {
            return myChoice;
        }


        void Time::setTimeChoice( const TimeChoicePtr& value )
        {
            myChoice = value;
        }


        bool Time::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            bool isSuccess = true;
            isSuccess &= myAttributes->fromXElement( message, xelement );
            bool isFirstTimeSignatureAdded = false;
            
            auto endIter = xelement.end();
            
            for( auto it = xelement.begin(); it != endIter; ++it )
            {
                if( it->getName() == "senza-misura" )
                {
                    myChoice->setChoice( TimeChoice::Choice::senzaMisura );
                    isSuccess &= myChoice->getSenzaMisura()->fromXElement( message, *it );
                }
                else
                {
                    myChoice->setChoice( TimeChoice::Choice::timeSignature );
                    auto timeSignature = makeTimeSignatureGroup();
                    importGroup( message, it, endIter, isSuccess, timeSignature );
                    
                    if( !isFirstTimeSignatureAdded && myChoice->getTimeSignatureGroupSet().size() == 1 )
                    {
                        myChoice->addTimeSignatureGroup( timeSignature );
                        myChoice->removeTimeSignatureGroup( myChoice->getTimeSignatureGroupSet().cbegin() );
                        isFirstTimeSignatureAdded = true;
                    }
                    else
                    {
                        myChoice->addTimeSignatureGroup( timeSignature );
                        isFirstTimeSignatureAdded = true;
                    }
                }
            }
            
            return isSuccess;
        }

    }
}
