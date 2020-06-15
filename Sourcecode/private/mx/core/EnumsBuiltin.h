// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include <iostream>
#include <string>

namespace mx
{
    namespace core
    {
        enum class XlinkType
        {
            simple = 0,
			extended = 1,
            title = 2,
            resource = 3,
            locator = 4,
            arc = 5
        };
        XlinkType parseXlinkType( const std::string& value );
		std::string toString( const XlinkType value );
		std::ostream& toStream( std::ostream& os, const XlinkType value );
		std::ostream& operator<<( std::ostream& os, const XlinkType value );
        
        
        enum class XmlSpace
        {
            default_ = 0,
			preserve = 1
        };
        XmlSpace parseXmlSpace( const std::string& value );
		std::string toString( const XmlSpace value );
		std::ostream& toStream( std::ostream& os, const XmlSpace value );
		std::ostream& operator<<( std::ostream& os, const XmlSpace value );

                
        enum class XlinkActuate
        {
            onLoad = 0,
            onRequest = 1,
            other = 2,
            none = 3
        };
        XlinkActuate parseXlinkActuate( const std::string& value );
        std::string toString( const XlinkActuate value );
        std::ostream& toStream( std::ostream& os, const XlinkActuate value );
        std::ostream& operator<<( std::ostream& os, const XlinkActuate value );
        
        enum class XlinkShow
        {
            new_ = 0,
            replace = 1,
            embed = 2,
            other = 3,
            none = 4
        };
        XlinkShow parseXlinkShow( const std::string& value );
        std::string toString( const XlinkShow value );
        std::ostream& toStream( std::ostream& os, const XlinkShow value );
        std::ostream& operator<<( std::ostream& os, const XlinkShow value );
    }
}