// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once
#include "mxtest/file/PathRoot.h"
#include <string>
#include <sstream>

namespace mxtest
{

#ifdef _MSC_VER
    constexpr const char FILE_PATH_SEPARATOR = '\\';
#else
    constexpr const char FILE_PATH_SEPARATOR = '/';
#endif

    inline const std::string getResourcesDirectoryPath()
    {
        std::stringstream ss;
        ss << MX_REPO_ROOT_PATH << FILE_PATH_SEPARATOR;
        ss << "Resources" << FILE_PATH_SEPARATOR;
        return ss.str();
    }

}
