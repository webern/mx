// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/EncodingChoice.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/Encoder.h"
#include "mx/core/elements/EncodingDate.h"
#include "mx/core/elements/EncodingDescription.h"
#include "mx/core/elements/Software.h"
#include "mx/core/elements/Supports.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        EncodingChoice::EncodingChoice()
        :myChoice( Choice::encodingDate )
        ,myEncodingDate( makeEncodingDate() )
        ,myEncoder( makeEncoder() )
        ,mySoftware( makeSoftware() )
        ,myEncodingDescription( makeEncodingDescription() )
        ,mySupports( makeSupports() )
        {}


        bool EncodingChoice::hasAttributes() const
        {
            return false;
        }


        std::ostream& EncodingChoice::streamAttributes( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& EncodingChoice::streamName( std::ostream& os ) const
        {
            os << "encoding";
            return os;
        }


        bool EncodingChoice::hasContents() const
        {
            return true;
        }


        std::ostream& EncodingChoice::streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const
        {
            os << std::endl;
            switch ( myChoice )
            {
                case Choice::encodingDate:
                {
                    myEncodingDate->toStream( os, indentLevel+1 );
                }
                    break;
                case Choice::encoder:
                {
                    myEncoder->toStream( os, indentLevel+1 );
                }
                    break;
                case Choice::software:
                {
                    mySoftware->toStream( os, indentLevel+1 );
                }
                    break;
                case Choice::encodingDescription:
                {
                    myEncodingDescription->toStream( os, indentLevel+1 );
                }
                    break;
                case Choice::supports:
                {
                    mySupports->toStream( os, indentLevel+1 );
                }
                    break;
                default:
                    break;
            }
            isOneLineOnly = false;
            return os;
        }


        EncodingChoice::Choice EncodingChoice::getChoice() const
        {
            return myChoice;
        }


        void EncodingChoice::setChoice( const EncodingChoice::Choice value )
        {
            myChoice = value;
        }


        EncodingDatePtr EncodingChoice::getEncodingDate() const
        {
            return myEncodingDate;
        }


        void EncodingChoice::setEncodingDate( const EncodingDatePtr& value )
        {
            if( value )
            {
                myEncodingDate = value;
            }
        }


        EncoderPtr EncodingChoice::getEncoder() const
        {
            return myEncoder;
        }


        void EncodingChoice::setEncoder( const EncoderPtr& value )
        {
            if( value )
            {
                myEncoder = value;
            }
        }


        SoftwarePtr EncodingChoice::getSoftware() const
        {
            return mySoftware;
        }


        void EncodingChoice::setSoftware( const SoftwarePtr& value )
        {
            if( value )
            {
                mySoftware = value;
            }
        }


        EncodingDescriptionPtr EncodingChoice::getEncodingDescription() const
        {
            return myEncodingDescription;
        }


        void EncodingChoice::setEncodingDescription( const EncodingDescriptionPtr& value )
        {
            if( value )
            {
                myEncodingDescription = value;
            }
        }


        SupportsPtr EncodingChoice::getSupports() const
        {
            return mySupports;
        }


        void EncodingChoice::setSupports( const SupportsPtr& value )
        {
            if( value )
            {
                mySupports = value;
            }
        }


        bool EncodingChoice::fromXElementImpl( std::ostream& message, xml::XElement& xelement )
        {
            MX_CHOICE_IF( encodingDate, "encoding-date", EncodingDate );
            MX_CHOICE_IF( encoder, "encoder", Encoder );
            MX_CHOICE_IF( software, "software", Software );
            MX_CHOICE_IF( encodingDescription, "encoding-description", EncodingDescription );
            MX_CHOICE_IF( supports, "supports", Supports );
            MX_BAD_ELEMENT_FAILURE( EncodingChoice );
        }

    }
}
