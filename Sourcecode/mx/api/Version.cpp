// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/api/Version.h"
#include "mx/impl/MxVersionDefines.h"
#include <sstream>

namespace mx
{
    namespace api
    {

////////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

        int
        Version::getVersionMajor()
        {
            return getInstance().myMajor;
        }


        int
        Version::getVersionMinor()
        {
            return getInstance().myMinor;
        }


        int
        Version::getVersionPatch()
        {
            return getInstance().myPatch;
        }


        const std::string&
        Version::getString()
        {
            return getInstance().myString;
        }

////////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

        Version::Version()
        : myMajor{ MX_VERSION_MAJOR }
        , myMinor{ MX_VERSION_MINOR }
        , myPatch{ MX_VERSION_PATCH }
        , myBuild{ MX_VERSION_BUILD }
        , myString{}
        {
            std::stringstream ss;
            ss <<
            myMajor << "." <<
            myMinor << "." <<
            myPatch << " (" <<
            myBuild << ")";
            myString = ss.str();
        }

        const Version&
        Version::getInstance()
        {
            const static Version v;
            return v;
        }
    }
}
