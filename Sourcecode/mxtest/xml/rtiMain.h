#pragma once

//#include "pugixml/pugixml.hpp"

#include <iostream>
#include "mx/xml/XFactory.h"

#include <string>
#include <fstream>

const std::string XML_DIRECTORY( "./" );
const std::string SUITE_DIR( "lysuite/" );
const std::string FILE_NAME( "01a-Pitches-Pitches.xml" );
const std::string TEMP_DIR( "temp/" );
const std::string ERROR_DIR( "error/" );

namespace MxRtiTest
{
    void rtiMain()
    {
        auto doc = mx::xml::XFactory::makeXDoc();
        doc->parse( XML_DIRECTORY + SUITE_DIR + FILE_NAME );
        doc->write( XML_DIRECTORY + TEMP_DIR + std::string( "expected_" ) + FILE_NAME );
        
    }
}