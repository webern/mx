// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/AttributesInterface.h"

namespace mx
{
    namespace core
    {
        bool AttributesInterface::hasValues() const { return false; }
        std::ostream& AttributesInterface::toStream( std::ostream& os ) const
        {
            return os;
        }


        std::ostream& operator<<( std::ostream& os, const AttributesInterface& value )
        {
            return value.toStream( os );
        }
    }
}
