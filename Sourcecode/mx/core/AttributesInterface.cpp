// MusicXML Class Library v0.1.1
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/core/AttributesInterface.h"

namespace mx
{
    namespace core
    {
        AttributesInterface::AttributesInterface() {}
        AttributesInterface::~AttributesInterface() {}
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