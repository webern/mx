// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/xml/PugiDoc.h"
#include "mx/xml/PugiElement.h"
#include "mx/utility/Parse.h"
#include "mx/utility/Throw.h"

#include <fstream>

namespace mx
{
    namespace xml
    {
        PugiDoc::PugiDoc()
            : myDoc()
            , myXmlVersion( DEFAULT_XML_VERSION )
            , myEncoding( DEFAULT_ENCODING )
            , myIsStandalone( false )
            , myDoWriteBom( false )
        {
            std::istringstream is( R"(<?xml version="1.0" encoding="UTF-8"?><root/>)" );
            auto options = pugi::parse_default | pugi::parse_declaration | pugi::parse_doctype;
            myDoc.load( is, options );
        }

        // Don't look at me, I'm ugly.
        
        void PugiDoc::loadStream( std::istream& is )
        {
            auto options = pugi::parse_default | pugi::parse_declaration | pugi::parse_doctype;
            auto parseResult = myDoc.load( is, options );
            if( parseResult.status != pugi::status_ok )
            {
                std::stringstream ss;
                ss << "pugixml parsing failed - '";
                switch ( parseResult.status )
                {
                    case pugi::status_file_not_found:
                        ss << "status_file_not_found'";
                        break;

                    case pugi::status_io_error:
                        ss << "status_io_error'";
                        break;
                        
                    case pugi::status_out_of_memory:
                        ss << "status_out_of_memory'";
                        break;
                        
                    case pugi::status_internal_error:
                        ss << "status_internal_error'";
                        break;
                        
                    case pugi::status_unrecognized_tag:
                        ss << "status_unrecognized_tag'";
                        break;
                        
                    case pugi::status_bad_pi:
                        ss << "status_bad_pi'";
                        break;
                        
                    case pugi::status_bad_comment:
                        ss << "status_bad_comment'";
                        break;
                        
                    case pugi::status_bad_cdata:
                        ss << "status_bad_cdata'";
                        break;
                        
                    case pugi::status_bad_doctype:
                        ss << "status_bad_doctype'";
                        break;
                        
                    case pugi::status_bad_pcdata:
                        ss << "status_bad_pcdata'";
                        break;
                        
                    case pugi::status_bad_start_element:
                        ss << "status_bad_start_element'";
                        break;
                        
                    case pugi::status_bad_attribute:
                        ss << "status_bad_attribute'";
                        break;
                        
                    case pugi::status_end_element_mismatch:
                        ss << "status_end_element_mismatch'";
                        break;
                        
                    case pugi::status_append_invalid_root:
                        ss << "status_append_invalid_root'";
                        break;
                        
                    case pugi::status_no_document_element:
                        ss << "status_no_document_element'";
                        break;
                        
                    default:
                        break;
                }
                ss << " - " << parseResult.description();
                MX_THROW( ss.str() );
            }
            else if( myDoc.begin() == myDoc.end() )
            {
                MX_THROW( "pugixml parse created an empty document" );
            }
            else if( myDoc.begin()->type() != pugi::node_declaration )
            {
                auto xmlDeclaration = myDoc.prepend_child( pugi::node_declaration );
                xmlDeclaration.set_name( "xml" );
                auto ver = xmlDeclaration.append_attribute( "version" );
                ver.set_value( "1.0" );
                ver = xmlDeclaration.append_attribute( "encoding" );
                ver.set_value( "UTF-8" );
            }
            else
            {
                auto xmlDec = *( myDoc.begin() );
                if( xmlDec.attributes_begin() == xmlDec.attributes_end() )
                {
                    auto attr = xmlDec.append_attribute( "version" );
                    attr.set_value( "1.0" );
                    attr = xmlDec.append_attribute( "encoding" );
                    attr.set_value( "UTF-8" );
                }
                else
                {
                    auto it = xmlDec.attributes_begin();
                    auto end = xmlDec.attributes_end();
                    if( !utility::compareCaseInsensitive( "version", it->name() ) )
                    {
                        auto attr = xmlDec.prepend_attribute( "version" );
                        attr.set_value( "1.0" );
                    }
                    auto verAttr = xmlDec.attributes_begin();
                    it = xmlDec.attributes_begin();
                    ++it;
                    if( it == end || !utility::compareCaseInsensitive( "encoding", it->name() ) )
                    {
                        auto encodingAttr = xmlDec.insert_attribute_after( "encoding", *verAttr );
                        encodingAttr.set_value( "UTF-8" );
                    }
                }
            }
            parseXmlDeclarationValues();
            
            // if the detected encoding doesn't match the encoding tag, take the actual encoding ??
            switch ( parseResult.encoding )
            {
                case pugi::encoding_auto:
                case pugi::encoding_utf8:
                case pugi::encoding_utf16_le:
                case pugi::encoding_utf16_be:
                    setEncoding( Encoding::utfEight );
                    break;
                case pugi::encoding_utf16:
                    setEncoding( Encoding::utfSixteen );
                    break;
                case pugi::encoding_utf32_le:
                case pugi::encoding_utf32_be:
                case pugi::encoding_utf32:
                case pugi::encoding_wchar:
                case pugi::encoding_latin1:
                default:
                    setEncoding( Encoding::utfEight );
                    break;
            }
        }


        void PugiDoc::saveStream( std::ostream& os ) const
        {
            auto pugiEncoding = pugi::encoding_utf8;
            switch( myEncoding )
            {
                case Encoding::utfEight:
                    pugiEncoding = pugi::encoding_utf8;
                    break;
                case Encoding::utfSixteen:
                    pugiEncoding = pugi::encoding_utf16;
                    break;
                default:
                    pugiEncoding = pugi::encoding_utf8;
                    break;
            }
            auto flags = pugi::format_indent;
            
            if( myDoWriteBom )
            {
                flags = pugi::format_indent | pugi::format_write_bom;
            }
            
            pugi::xml_writer_stream writer( os );
            myDoc.save( writer, "  ", flags, pugiEncoding );
        }


        void PugiDoc::loadFile( const std::string& filename )
        {
            std::ifstream infile( filename.c_str() );
            if( !infile.is_open() )
            {
                throw std::runtime_error( "error opening input file" );
            }
            
            loadStream( infile );
            infile.close();
        }


        void PugiDoc::saveFile( const std::string& filename ) const
        {
            std::ofstream outfile( filename.c_str() );
            if( !outfile.is_open() )
            {
                throw std::runtime_error( "error opening file for writing" );
            }
            
            saveStream( outfile );
            outfile.close();
        }


        XmlVersion PugiDoc::getXmlVersion() const
        {
            return myXmlVersion;
        }


        void PugiDoc::setXmlVersion( XmlVersion value )
        {
            if( value == XmlVersion::unknown )
            {
                MX_THROW( "cannot set the XmlVersion to 'unknown'" );
            }
            if( myDoc.begin() == myDoc.end() )
            {
                MX_THROW( "the xml document does not have an xml declaration - bad state" );
            }
            else if ( myDoc.begin()->type() != pugi::node_declaration )
            {
                MX_THROW( "the first node in the xml document should be an xml declaration - bad state" );
            }
            else
            {
                auto node = *( myDoc.begin() );
                if( node.attributes_begin() == node.attributes_end() )
                {
                    node.prepend_attribute( "version" );
                }
                auto attr = node.attributes_begin();
                if( attr == node.attributes_end() )
                {
                    MX_THROW( "something bad happened - this line should be unreachable" );
                }
                if( !utility::compareCaseInsensitive( "version", attr->name() ) )
                {
                    MX_THROW( "the first attribute of the xml declaration must be 'version'" );
                }
                attr->set_value( toString( value ).c_str() );
            }
            myXmlVersion = value;
        }
        
        
        Encoding PugiDoc::getEncoding() const
        {
            return myEncoding;
        }
        
        
        void PugiDoc::setEncoding( Encoding value )
        {
            if( value == Encoding::unknown )
            {
                MX_THROW( "the encoding cannot be unknown" );
            }
            auto attr = getXmlDeclarationAttribute( "encoding" );
            if( ! utility::compareCaseInsensitive( "encoding", attr.name() ) )
            {
                MX_THROW( "the 'encoding' attribute could not be found" );
            }
            attr.set_value( toString( value ).c_str() );
            myEncoding = value;
        }
        
        
        bool PugiDoc::getHasStandaloneAttribute() const
        {
            auto standalone = getXmlDeclarationAttribute( "standalone" );
            if( mx::utility::compareCaseInsensitive( standalone.name(), "standalone" ) )
            {
                return true;
            }
            return false;
        }
        
        
        void PugiDoc::setHasStandaloneAttribute( bool value )
        {
            bool isStandaloneCurrentlyPresent = getHasStandaloneAttribute();
            if( isStandaloneCurrentlyPresent && value )
            {
                return;
            }
            else if ( !isStandaloneCurrentlyPresent && !value )
            {
                return;
            }
            else if ( value )
            {
                auto xmlDeclarationNode = getXmlDeclarationNode();
                auto it = xmlDeclarationNode.attributes_begin();
                if( it == xmlDeclarationNode.attributes_end() )
                {
                    MX_THROW( "the xml declaration node must have attributes" );
                }
                if( !utility::compareCaseInsensitive( "version", it->name() ) )
                {
                    MX_THROW( "the first xml declaration attribute must be 'version'" );
                }
                ++it;
                if( it == xmlDeclarationNode.attributes_end() )
                {
                    MX_THROW( "the xml declaration node must have an 'encoding' attribute" );
                }
                if( !utility::compareCaseInsensitive( "encoding", it->name() ) )
                {
                    MX_THROW( "the second attribute of the xml declaration node must be 'encoding'" );
                }
                auto attr = xmlDeclarationNode.insert_attribute_after( "standalone", *it );
                if( myIsStandalone )
                {
                    attr.set_value( "yes" );
                }
                else
                {
                    attr.set_value( "no" );
                }
                return;
            }
            else if ( !value )
            {
                auto xmlDeclarationNode = getXmlDeclarationNode();
                for( auto it = xmlDeclarationNode.attributes_begin();
                     it != xmlDeclarationNode.attributes_end(); ++it )
                {
                    if( utility::compareCaseInsensitive( "standalone", it->name() ) )
                    {
                        xmlDeclarationNode.remove_attribute( *it );
                        myIsStandalone = false;
                        return;
                    }
                }
            }
        }
        
        
        bool PugiDoc::getIsStandalone() const
        {
            return myIsStandalone;
        }
        
        
        void PugiDoc::setIsStandalone( bool value )
        {
            setHasStandaloneAttribute( true );
            auto attr = getXmlDeclarationAttribute( "standalone" );
            if( value )
            {
                attr.set_value( "yes" );
            }
            else
            {
                attr.set_value( "no" );
            }
            myIsStandalone = value;
        }
        
        
        bool PugiDoc::getHasDoctypeDeclaration() const
        {
            auto doctype = getDoctypeNode();
            if( doctype.type() != pugi::node_doctype )
            {
                return false;
            }
            return true;
        }
        
        
        void PugiDoc::setHasDoctypeDeclaration( bool value )
        {
            bool isDoctypeCurrentlyPresent = getHasDoctypeDeclaration();
            if( value && isDoctypeCurrentlyPresent )
            {
                // is requested and already exists, nothing to do
                return;
            }
            else if( !isDoctypeCurrentlyPresent && !value )
            {
                // is not requested and doesn't exist, nothing to do
                return;
            }
            else if ( value )
            {
                // add doctype
                if( myDoc.begin() == myDoc.end() )
                {
                    MX_THROW( "empty documet" );
                }
                auto xmlDeclarationNode = *( myDoc.begin() );
                if( xmlDeclarationNode.type() != pugi::node_declaration )
                {
                    MX_THROW( "bad xml document state" );
                }
                myDoc.insert_child_after( pugi::node_doctype, xmlDeclarationNode );
                return;
            }
            else if ( !value )
            {
                // delete doctype
                for( auto it = myDoc.begin(); it != myDoc.end(); ++it )
                {
                    if( it->type() == pugi::node_doctype )
                    {
                        myDoc.remove_child( *it );
                        return;
                    }
                }
            }
        }
        
        
        std::string PugiDoc::getDoctypeValue() const
        {
            auto doctypeNode = getDoctypeNode();
            return doctypeNode.value();
        }
        
        
        void PugiDoc::setDoctypeValue( const std::string& value )
        {
            setHasDoctypeDeclaration( true );
            auto doctypeNode = getDoctypeNode();
            doctypeNode.set_value( value.c_str() );
        }
        
        
        XElementPtr PugiDoc::getRoot() const
        {
            for( auto it = myDoc.begin();
                 it != myDoc.end(); ++ it )
            {
                if( it->type() == pugi::node_element )
                {
                    return std::make_shared<PugiElement>( *it, XDoc::shared_from_this() );
                }
            }
            return XElementPtr{};
        }
        
        
        void PugiDoc::parseXmlDeclarationValues()
        {
            parseXmlVersionFromDoc();
            parseEncodingFromDoc();
            parseStandalone();
        }
        
        
        void PugiDoc::parseXmlVersionFromDoc()
        {
            auto xmlDeclaration = getXmlDeclarationNode();
            if( xmlDeclaration.type() != pugi::node_declaration )
            {
                MX_THROW( "the xml document must have an xml declaration as its first node" );
            }
            
            auto version = xmlDeclaration.attribute( "version" );
            std::string value{ version.value() };
            if( value == "" )
            {
                myXmlVersion = DEFAULT_XML_VERSION;
                setXmlVersion( DEFAULT_XML_VERSION );
                return;
            }
            
            myXmlVersion = parseXmlVersion( value );
        }
        
        
        void PugiDoc::parseEncodingFromDoc()
        {
            auto attr = getXmlDeclarationAttribute( "encoding" );
            if( !utility::compareCaseInsensitive( "encoding", attr.name() ) )
            {
                MX_THROW( "encoding value was not found" );
            }
            myEncoding = parseEncoding( attr.value() );
            attr.set_value( toString( myEncoding ).c_str() );
        }
        
        
        void PugiDoc::parseStandalone()
        {
            
            if( !getHasStandaloneAttribute() )
            {
                myIsStandalone = false;
                return;
            }
            auto standalone = getXmlDeclarationAttribute( "standalone" );
            if( std::string{ standalone.value() } == std::string{ "yes" } )
            {
                myIsStandalone = true;
                return;
            }
            myIsStandalone = false;
        }
        
        
        pugi::xml_node PugiDoc::getDoctypeNode() const
        {
            if( myDoc.empty() )
            {
                return pugi::xml_node{};
            }
            
            auto childrenIter = myDoc.begin();
            if( childrenIter == myDoc.end() )
            {
                return pugi::xml_node{};
            }
            if( childrenIter->type() == pugi::node_declaration )
            {
                ++childrenIter;
            }
            if( childrenIter == myDoc.end() ||
                childrenIter->type() != pugi::node_doctype )
            {
                return pugi::xml_node{};
            }
            else return *childrenIter;
        }
        
        
        pugi::xml_node PugiDoc::getXmlDeclarationNode() const
        {
            if( myDoc.empty() )
            {
                return pugi::xml_node{};
            }
            
            auto firstNode = myDoc.first_child();
            
            if( firstNode.type() != pugi::node_declaration )
            {
                return pugi::xml_node{};
            }
            std::string nodeName{ firstNode.name() };
            if( !mx::utility::compareCaseInsensitive( nodeName, "xml" ) )
            {
                return pugi::xml_node{};
            }
            
            return firstNode;
        }

        
        pugi::xml_attribute PugiDoc::getXmlDeclarationAttribute( const char* const name ) const
        {
            auto declaration = getXmlDeclarationNode();
            if( declaration.type() != pugi::node_declaration )
            {
                return pugi::xml_attribute{};
            }
            return declaration.attribute( name );
        }
        
        
        void PugiDoc::setDoWriteByteOrderMark( bool value )
        {
            myDoWriteBom = value;
        }
    }
}
