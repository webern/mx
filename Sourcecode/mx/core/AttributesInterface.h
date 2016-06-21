// MusicXML Class Library v0.1.1
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include <iostream>
#include <memory>

namespace mx
{
    namespace core
    {
        struct AttributesInterface;
        using AttributesPtr = std::shared_ptr<AttributesInterface>;
        using AttributesUPtr = std::unique_ptr<AttributesInterface>;

        struct AttributesInterface
        {
            AttributesInterface();
            virtual ~AttributesInterface();
            virtual bool hasValues() const;
            virtual std::ostream& toStream( std::ostream& os ) const;
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
                os <<"\"";
            }
            return os;
        }
    }
}
