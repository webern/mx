// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/MarkData.h"
#include "mx/impl/PositionFunctions.h"
#include "mx/impl/PrintFunctions.h"

namespace mx
{
    namespace impl
    {
        template<typename ATTRIBUTES_TYPE>
        void parseMarkDataAttributes( const ATTRIBUTES_TYPE& attr, api::MarkData& outMarkData )
        {
            outMarkData.positionData = getPositionData<ATTRIBUTES_TYPE>( attr );
            outMarkData.printData = getPrintData<ATTRIBUTES_TYPE>( attr );
        }
        
        template<typename ATTRIBUTES_TYPE>
        void setAttributesFromMarkData( const api::MarkData& markData, ATTRIBUTES_TYPE& outAttributes )
        {
            setAttributesFromPositionData( markData.positionData, outAttributes );
            setAttributesFromPrintData( markData.printData, outAttributes );
        }
	}
}
