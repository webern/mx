// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/MarkData.h"
#include "mx/impl/PositionFunctions.h"
#include "mx/impl/PrintFunctions.h"

namespace mx
{
    namespace impl
    {
        template<typename T>
        void parseMarkDataAttributes(const T& elem, api::MarkData& outMarkData )
        {
            const auto& attr = *( elem.getAttributes() );
            outMarkData.positionData = getPositionData( attr );
            outMarkData.printData = getPrintData( attr );
        }
	}
}
