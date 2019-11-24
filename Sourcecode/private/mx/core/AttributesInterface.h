// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/UnusedParameter.h"

#include <iostream>
#include <memory>

namespace ezxml
{
    class XElement;
}

namespace mx  
{
    namespace core
    {
        struct AttributesInterface;
        using AttributesPtr = std::shared_ptr<AttributesInterface>;
        using AttributesUPtr = std::unique_ptr<AttributesInterface>;

        struct AttributesInterface
        {
            virtual ~AttributesInterface() {}
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
            virtual bool fromXElement( std::ostream& message, ::ezxml::XElement& xelement ) final;

        private:
            virtual bool fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement ) = 0;
        };

        std::ostream& operator<<( std::ostream& os, const AttributesInterface& value );
        
        template<class T>
        std::ostream& streamAttribute( std::ostream& os,
                                       const T& value,
                                       const char* attributeName,
                                       const bool isPresent )
        {
            if ( isPresent )
            {
                os << " ";
                os << attributeName;
                os << "=\"";
                os << value;
                os << "\"";
            }
            return os;
        }
    }
}
