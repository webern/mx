// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
    namespace core
    {

        MX_FORWARD_DECLARE_ELEMENT( Encoder )
        MX_FORWARD_DECLARE_ELEMENT( EncodingDate )
        MX_FORWARD_DECLARE_ELEMENT( EncodingDescription )
        MX_FORWARD_DECLARE_ELEMENT( Software )
        MX_FORWARD_DECLARE_ELEMENT( Supports )
        MX_FORWARD_DECLARE_ELEMENT( EncodingChoice )

        inline EncodingChoicePtr makeEncodingChoice() { return std::make_shared<EncodingChoice>(); }

        class EncodingChoice : public ElementInterface
        {
        public:
            enum class Choice
            {
                encodingDate = 1,
                encoder = 2,
                software = 3,
                encodingDescription = 4,
                supports = 5
            };
            EncodingChoice();

            virtual bool hasAttributes() const;
            virtual std::ostream& streamAttributes( std::ostream& os ) const;
            virtual std::ostream& streamName( std::ostream& os ) const;
            virtual bool hasContents() const;
            virtual std::ostream& streamContents( std::ostream& os, const int indentLevel, bool& isOneLineOnly ) const;

            /* _________ Choice _________ */
            EncodingChoice::Choice getChoice() const;
            void setChoice( const EncodingChoice::Choice value );

            /* _________ EncodingDate minOccurs = 1, maxOccurs = 1 _________ */
            EncodingDatePtr getEncodingDate() const;
            void setEncodingDate( const EncodingDatePtr& value );

            /* _________ Encoder minOccurs = 1, maxOccurs = 1 _________ */
            EncoderPtr getEncoder() const;
            void setEncoder( const EncoderPtr& value );

            /* _________ Software minOccurs = 1, maxOccurs = 1 _________ */
            SoftwarePtr getSoftware() const;
            void setSoftware( const SoftwarePtr& value );

            /* _________ EncodingDescription minOccurs = 1, maxOccurs = 1 _________ */
            EncodingDescriptionPtr getEncodingDescription() const;
            void setEncodingDescription( const EncodingDescriptionPtr& value );

            /* _________ Supports minOccurs = 1, maxOccurs = 1 _________ */
            SupportsPtr getSupports() const;
            void setSupports( const SupportsPtr& value );

            bool fromXElement( std::ostream& message, xml::XElement& xelement );

        private:
            Choice myChoice;
            EncodingDatePtr myEncodingDate;
            EncoderPtr myEncoder;
            SoftwarePtr mySoftware;
            EncodingDescriptionPtr myEncodingDescription;
            SupportsPtr mySupports;
        };
    }
}
