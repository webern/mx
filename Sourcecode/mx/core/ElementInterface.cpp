// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/ElementInterface.h"
#include <sstream>

namespace mx
{
    namespace core
    {
        const char* INDENT = "   ";
 
        ElementInterface::ElementInterface()
        {}
        
        ElementInterface::~ElementInterface() {}
        
        
        std::ostream& ElementInterface::streamOpenTag( std::ostream& os ) const
        {
            os << "<";
            this->streamName( os );
            if ( hasAttributes() )
            {
                streamAttributes( os );
            }
            os << ">";
            return os;
        }


        std::ostream& ElementInterface::streamCloseTag( std::ostream& os ) const
        {
            os << "</";
            this->streamName( os );
            os << ">";
            return os;
        }


        std::ostream& ElementInterface::streamSelfCloseTag( std::ostream& os ) const
        {
            os << "<";
            this->streamName( os );
            if ( hasAttributes() )
            {
                streamAttributes( os );
            }
            os << "/>";
            return os;
        }
        bool ElementInterface::hasContents() const
        {
            std::stringstream ss;
            bool discard;
            streamContents( ss, 0, discard );
            return ( ss.str() ).length() > 0;
        }


        std::ostream& ElementInterface::toStream( std::ostream& os, const int indentLevel ) const
        {
            indent( os, indentLevel );
            if ( hasContents() )
            {
                streamOpenTag( os );
                bool isOneLineOnly = false;
                streamContents( os, indentLevel, isOneLineOnly );
                if ( !isOneLineOnly )
                {
                    indent( os, indentLevel );
                }
                streamCloseTag( os );
            }
            else
            {
                streamSelfCloseTag( os );
            }
            return os;
        }
        
        
        const std::string ElementInterface::getElementName() const
        {
            std::stringstream ss;
            this->streamName( ss );
            return ss.str();
        }


        std::ostream& indent( std::ostream& os, const int indentLevel )
        {
            for ( int i = 0; i < indentLevel; ++i )
            {
                os << INDENT;
            }
            return os;
        }
        
        
        bool ElementInterface::hasAttributes() const
        {
            return false;
        }


        std::ostream& operator<<( std::ostream& os, const ElementInterface& value )
        {
            return value.toStream( os, 0 );
        }
    }
}
