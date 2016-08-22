// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

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
    }
}
