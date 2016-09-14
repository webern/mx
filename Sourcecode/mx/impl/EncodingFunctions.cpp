// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/impl/EncodingFunctions.h"
#include "mx/core/Date.h"
#include "mx/core/elements/Encoder.h"
#include "mx/core/elements/Encoding.h"
#include "mx/core/elements/Encoding.h"
#include "mx/core/elements/EncodingChoice.h"
#include "mx/core/elements/EncodingDate.h"
#include "mx/core/elements/EncodingDescription.h"
#include "mx/core/elements/Identification.h"
#include "mx/core/elements/Software.h"
#include "mx/core/elements/Supports.h"

namespace mx
{
    namespace impl
    {
        void createEncoding( const api::EncodingData& inEncoding, core::ScoreHeaderGroup& header )
        {
            auto identification = header.getIdentification();
            auto encoding = identification->getEncoding();
            
            if( !inEncoding.encoder.empty() )
            {
                header.setHasIdentification( true );
                identification->setHasEncoding( true );
                auto item = core::makeEncodingChoice();
                item->setChoice( core::EncodingChoice::Choice::encoder );
                item->getEncoder()->setValue( core::XsString( inEncoding.encoder ) );
                encoding->addEncodingChoice( item );
            }
            
            core::Date tryDate{ inEncoding.encodingDate.year, inEncoding.encodingDate.month, inEncoding.encodingDate.day };
            const bool isYearValid = inEncoding.encodingDate.year == tryDate.getYear();
            const bool isMonthValid = inEncoding.encodingDate.month == tryDate.getMonth();
            const bool isDayValid = inEncoding.encodingDate.day == tryDate.getDay();
            if( isYearValid || isMonthValid || isDayValid )
            {
                header.setHasIdentification( true );
                identification->setHasEncoding( true );
                auto item = core::makeEncodingChoice();
                item->setChoice( core::EncodingChoice::Choice::encodingDate );
                item->getEncodingDate()->setValue( tryDate );
                encoding->addEncodingChoice( item );
            }
            
            if( !inEncoding.encodingDescription.empty() )
            {
                header.setHasIdentification( true );
                identification->setHasEncoding( true );
                auto item = core::makeEncodingChoice();
                item->setChoice( core::EncodingChoice::Choice::encodingDescription );
                item->getEncodingDescription()->setValue( core::XsString( inEncoding.encodingDescription ) );
                encoding->addEncodingChoice( item );
            }
            
            for( const auto& s : inEncoding.software )
            {
                header.setHasIdentification( true );
                identification->setHasEncoding( true );
                auto item = core::makeEncodingChoice();
                item->setChoice( core::EncodingChoice::Choice::software );
                item->getSoftware()->setValue( core::XsString( s ) );
                encoding->addEncodingChoice( item );
            }

            for ( const auto& s : inEncoding.supportedItems )
            {
                header.setHasIdentification( true );
                identification->setHasEncoding( true );
                auto item = core::makeEncodingChoice();
                item->setChoice( core::EncodingChoice::Choice::supports );
                auto supports = item->getSupports();
                auto attributes = supports->getAttributes();

                if ( !s.elementName.empty() )
                {
                    attributes->element.setValue( s.elementName );
                }

                if ( !s.attributeName.empty() )
                {
                    attributes->hasAttribute = true;
                    attributes->attribute.setValue( s.attributeName );
                }

                if ( !s.specificValue.empty() )
                {
                    attributes->hasValue = true;
                    attributes->value.setValue( s.specificValue );
                }

                attributes->type = s.isSupported ? core::YesNo::yes : core::YesNo::no;
                encoding->addEncodingChoice( item );
            }
        }


        api::EncodingData createEncoding( const core::Encoding& inEncoding )
        {
            api::EncodingData outEncoding;
            bool isDateFound = false;
            bool isEncoderFound = false;
            bool isDescriptionFound = false;
            for( auto ec : inEncoding.getEncodingChoiceSet() )
            {
                switch( ec->getChoice() )
                {
                    case core::EncodingChoice::Choice::encodingDate:
                    {
                        if( !isDateFound )
                        {
                            outEncoding.encodingDate.year = ec->getEncodingDate()->getValue().getYear();
                            outEncoding.encodingDate.month = ec->getEncodingDate()->getValue().getMonth();
                            outEncoding.encodingDate.day = ec->getEncodingDate()->getValue().getDay();
                            isDateFound = true;
                        }
                        break;
                    }
                    case core::EncodingChoice::Choice::encoder:
                    {
                        if( !isEncoderFound )
                        {
                            outEncoding.encoder = ec->getEncoder()->getValue().getValue();
                            isEncoderFound = true;
                        }
                        break;
                    }
                    case core::EncodingChoice::Choice::encodingDescription:
                    {
                        if( !isDescriptionFound )
                        {
                            outEncoding.encodingDescription = ec->getEncodingDescription()->getValue().getValue();
                            isDescriptionFound = true;
                        }
                        break;
                    }
                    case core::EncodingChoice::Choice::software:
                    {
                        outEncoding.software.emplace_back( ec->getSoftware()->getValue().getValue() );
                        break;
                    }
                    case core::EncodingChoice::Choice::supports:
                    {
                        auto supportsElement = ec->getSupports();
                        auto attr = supportsElement->getAttributes();
                        api::SupportedItem item;
                        item.elementName = attr->element.getValue();
                        if( attr->hasAttribute )
                        {
                            item.attributeName = attr->attribute.getValue();
                        }
                        item.isSupported = ( attr->type == core::YesNo::yes );
                        if( attr->hasValue )
                        {
                            item.specificValue = attr->value.getValue();
                        }
                        outEncoding.supportedItems.push_back( std::move( item ) );
                        break;
                    }
                }
                
                
            }
            return outEncoding;
        }
    }
}
