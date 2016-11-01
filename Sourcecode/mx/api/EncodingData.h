// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/ApiCommon.h"

#include <string>
#include <vector>

namespace mx
{
    namespace api
    {
        class SupportedItem
        {
        public:
            std::string elementName;
            std::string attributeName;
            std::string specificValue;
            bool isSupported;

            SupportedItem()
            : elementName()
            , attributeName()
            , specificValue()
            , isSupported(true)
            {

            }
        };
        
        class EncodingDate
        {
        public:
            int year;
            int month;
            int day;
            EncodingDate() : year(-1), month(-1), day(-1) {}
        };
        
        class EncodingData
        {
        public:
            EncodingDate encodingDate;
            std::string encoder;
            std::string encodingDescription;
            std::vector<std::string> software;
            std::vector<SupportedItem> supportedItems; 
        };
        
        MXAPI_EQUALS_BEGIN( SupportedItem )
        MXAPI_EQUALS_MEMBER( elementName )
        MXAPI_EQUALS_MEMBER( attributeName )
        MXAPI_EQUALS_MEMBER( specificValue )
        MXAPI_EQUALS_MEMBER( isSupported )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( SupportedItem );
        
        MXAPI_EQUALS_BEGIN( EncodingDate )
        MXAPI_EQUALS_MEMBER( year )
        MXAPI_EQUALS_MEMBER( month )
        MXAPI_EQUALS_MEMBER( day )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( EncodingDate );
        
        MXAPI_EQUALS_BEGIN( EncodingData )
        MXAPI_EQUALS_MEMBER( encodingDate )
        MXAPI_EQUALS_MEMBER( encoder )
        MXAPI_EQUALS_MEMBER( encodingDescription )
        MXAPI_EQUALS_MEMBER( software )
        MXAPI_EQUALS_MEMBER( supportedItems )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( EncodingData );
    }
}
