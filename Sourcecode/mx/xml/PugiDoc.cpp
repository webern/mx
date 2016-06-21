// MusicXML Class Library v0.1.1
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/xml/PugiDoc.h"
#include "mx/xml/PugiElement.h"
#include "mx/utility/Parse.h"

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
        {
            
        }


        void PugiDoc::parse( std::istream& is )
        {
            auto options = pugi::parse_default | pugi::parse_declaration | pugi::parse_doctype;
            auto parseResult = myDoc.load( is, options );
            if( parseResult.status != pugi::status_ok )
            {
                throw std::runtime_error( "PugiDoc::parse failed" );
            }
            parseXmlDeclarationValues();
        }


        void PugiDoc::write( std::ostream& os ) const
        {
            pugi::xml_writer_stream writer( os );
            myDoc.save( writer, "  " );
        }


        void PugiDoc::parse( const std::string& filename )
        {
            std::ifstream infile( filename.c_str() );
            if( !infile.is_open() )
            {
                throw std::runtime_error( "error opening input file" );
            }
            
            this->parse( infile );
            infile.close();
        }


        void PugiDoc::write( const std::string& filename ) const
        {
            std::ofstream outfile( filename.c_str() );
            if( !outfile.is_open() )
            {
                throw std::runtime_error( "error opening file for writing" );
            }
            
            this->write( outfile );
            outfile.close();
        }


        XmlVersion PugiDoc::getXmlVersion() const
        {
            return myXmlVersion;
        }


        void PugiDoc::setXmlVersion( XmlVersion value )
        {
            myXmlVersion = value;
        }
        
        
        Encoding PugiDoc::getEncoding() const
        {
            return myEncoding;
        }
        
        
        void PugiDoc::setEncoding( Encoding value )
        {
            myEncoding = value;
        }
        
        
        bool PugiDoc::getHasStandaloneAttribute() const
        {
            auto standalone = getXmlDeclarationAttribute( "standalone" );
            if( mx::utility::compareCaseInsensitive( standalone.value(), "yes" ) )
            {
                return true;
            }
            return false;
        }
        
        
        void PugiDoc::setHasStandaloneAttribute( bool value )
        {
            addStandaloneValueIfNeeded( value );
            removeStandaloneValueIfNeeded( value );
            setStandaloneValueIfNeeded( value );
        }
        
        
        bool PugiDoc::getIsStandaloneValueYes() const
        {
            return myIsStandalone;
        }
        
        
        void PugiDoc::setIsStandaloneValueYes( bool value )
        {
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
            
        }
        
        
        std::string PugiDoc::getDoctypeDeclaration() const
        {
            return "";
        }
        
        
        void PugiDoc::setDoctypeDeclaration( const std::string& value )
        {
            
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
                myXmlVersion = DEFAULT_XML_VERSION;
                return;
            }
            
            auto version = xmlDeclaration.attribute( "version" );
            std::string value{ version.value() };
            if( value == "" )
            {
                myXmlVersion = DEFAULT_XML_VERSION;
                return;
            }
            
            myXmlVersion = parseXmlVersion( value );
        }
        
        
        void PugiDoc::parseEncodingFromDoc()
        {
            
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
        
        
        void PugiDoc::addStandaloneValueIfNeeded( bool shouldHaveStandaloneAttribute )
        {
            if( !shouldHaveStandaloneAttribute )
            {
                return;
            }
            
            if( shouldHaveStandaloneAttribute && getHasStandaloneAttribute() )
            {
                return;
            }
            
            auto declaration = getXmlDeclarationNode();
            declaration.append_attribute( "standalone" );
        }
        
        
        void PugiDoc::removeStandaloneValueIfNeeded( bool shouldHaveStandaloneAttribute )
        {
            if( shouldHaveStandaloneAttribute )
            {
                return;
            }
            
            if( !shouldHaveStandaloneAttribute && !getHasStandaloneAttribute() )
            {
                return;
            }
            auto declaration = getXmlDeclarationNode();
            /* why doesn't this work?
            auto it = std::find(
                declaration.attributes().begin(),
                declaration.attributes().end(),
                                [](const pugi::xml_attribute& attr)->bool { return std::string{ attr.name() } == std::string{ "standalone" }; } );
            */
            for( auto it = declaration.attributes().begin();
                 it != declaration.attributes().end(); ++ it )
            {
                if( std::string{ it->name() } == std::string{ "standalone" } )
                {
                    declaration.remove_attribute( *it );
                    return;
                }
            }
        }
        
        
        void PugiDoc::setStandaloneValueIfNeeded( bool shouldHaveStandaloneAttribute )
        {
            if( !getHasStandaloneAttribute() )
            {
                return;
            }
            auto declaration = getXmlDeclarationNode();
            for( auto it = declaration.attributes().begin();
                it != declaration.attributes().end(); ++ it )
            {
                if( std::string{ it->name() } == std::string{ "standalone" } )
                {
                    if( myIsStandalone )
                    {
                        it->set_value( "yes" );
                        return;
                    }
                    else
                    {
                        it->set_value( "no" );
                        return;
                    }
                }
            }
        }
    }
}