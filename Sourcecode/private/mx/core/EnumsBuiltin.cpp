// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/EnumsBuiltin.h"

namespace mx
{
    namespace core
    {
        XlinkActuate parseXlinkActuate( const std::string& value )
        {
            if ( value == "onLoad" ) { return XlinkActuate::onLoad; }
            else if ( value == "onRequest" ) { return XlinkActuate::onRequest; }
            else if ( value == "other" ) { return XlinkActuate::other; }
            else if ( value == "none" ) { return XlinkActuate::none; }
            return XlinkActuate::onLoad;
        }


        std::string toString( const XlinkActuate value )
        {
            switch ( value )
            {
                case XlinkActuate::onLoad: return "onLoad";
                case XlinkActuate::onRequest: return "onRequest";
                case XlinkActuate::other: return "other";
                case XlinkActuate::none: return "none";
                default: break;
            }
            return "none";
        }


        std::ostream& toStream( std::ostream& os, const XlinkActuate value )
        {
            return os << toString( value );
        }


        std::ostream& operator<<( std::ostream& os, const XlinkActuate value )
        {
            return toStream( os, value );
        }
        
        XlinkShow parseXlinkShow( const std::string& value )
        {
            if ( value == "new" ) { return XlinkShow::new_; }
            else if ( value == "replace" ) { return XlinkShow::replace; }
            else if ( value == "embed" ) { return XlinkShow::embed; }
            else if ( value == "other" ) { return XlinkShow::other; }
            else if ( value == "none" ) { return XlinkShow::none; }
            return XlinkShow::new_;
        }


        std::string toString( const XlinkShow value )
        {
            switch ( value )
            {
                case XlinkShow::new_: return "new";
                case XlinkShow::replace: return "replace";
                case XlinkShow::embed: return "embed";
                case XlinkShow::other: return "other";
                case XlinkShow::none: return "none";
                default: break;
            }
            return "none";
        }


        std::ostream& toStream( std::ostream& os, const XlinkShow value )
        {
            return os << toString( value );
        }


        std::ostream& operator<<( std::ostream& os, const XlinkShow value )
        {
            return toStream( os, value );
        }
        
        XlinkType parseXlinkType( const std::string& value )
        {
            if ( value == "simple" ) { return XlinkType::simple; }
            else if ( value == "extended" ) { return XlinkType::extended; }
            else if ( value == "title" ) { return XlinkType::title; }
            else if ( value == "resource" ) { return XlinkType::resource; }
            else if ( value == "locator" ) { return XlinkType::locator; }
            else if ( value == "arc" ) { return XlinkType::arc; }
            return XlinkType::simple;
        }


        std::string toString( const XlinkType value )
        {
            switch ( value )
            {
                case XlinkType::simple: return "simple";
                case XlinkType::extended: return "extended";
                case XlinkType::title: return "title";
                case XlinkType::resource: return "resource";
                case XlinkType::locator: return "locator";
                case XlinkType::arc: return "arc";
                default: break;
            }
            return "simple";
        }


        std::ostream& toStream( std::ostream& os, const XlinkType value )
        {
            return os << toString( value );
        }


        std::ostream& operator<<( std::ostream& os, const XlinkType value )
        {
            return toStream( os, value );
        }

        XmlSpace parseXmlSpace( const std::string& value )
        {
            if ( value == "default" ) { return XmlSpace::default_; }
            else if ( value == "preserve" ) { return XmlSpace::preserve; }
            return XmlSpace::default_;
        }


        std::string toString( const XmlSpace value )
        {
            switch ( value )
            {
                case XmlSpace::default_: return "default";
                case XmlSpace::preserve: return "preserve";
                default: break;
            }
            return "default";
        }


        std::ostream& toStream( std::ostream& os, const XmlSpace value )
        {
            return os << toString( value );
        }


        std::ostream& operator<<( std::ostream& os, const XmlSpace value )
        {
            return toStream( os, value );
        }
    }
}
