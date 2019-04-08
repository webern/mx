// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include <string>

namespace mx
{
    namespace api
    {
        /// @brief TODO
        ///
        /// @detailed TODO
        ///
        class Version
        {
        public:
            static int getVersionMajor();
            static int getVersionMinor();
            static int getVersionPatch();
            static const std::string& getString();

        private:
            int myMajor;
            int myMinor;
            int myPatch;
            int myBuild;
            std::string myString;

        private:
            Version();
            static const Version& getInstance();
        };
    }
}
