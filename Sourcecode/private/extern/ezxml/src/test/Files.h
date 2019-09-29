#include "Path.h"

namespace ezxmltest
{
    constexpr const char* const FREEZING_SMALL = "freezing-small.xml";
    constexpr const char* const SIMPLE = "simple.xml";

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    constexpr const char* const SLASH = "\\";
#else
    constexpr const char* const SLASH = "/";
#endif

    constexpr const char* const TEST_DATA_DIRNAME = "testdata";
    constexpr const char* const TEST_INPUT_DIRNAME = "xml";
    constexpr const char* const TEST_OUTPUT_DIRNAME = "out";


    inline std::string
    slash()
    {
        return std::string{ SLASH };
    }


    inline std::string
    inputDirname()
    {
        return std::string{ TEST_DATA_DIRNAME } + slash() + std::string{ TEST_INPUT_DIRNAME };
    }


    inline std::string
    root()
    {
        std::string r = EZXML_ROOT;
        return r;
    }


    inline std::string
    inpath()
    {
        auto r = root();
        auto s = slash();
        auto idir = inputDirname();
        auto i = r + s + idir;
        return i;
    }


    inline std::string
    filepath( const std::string& filename )
    {
        auto i = inpath();
        auto s = slash();
        auto fp = i + s + filename;
        return fp;
    }
}