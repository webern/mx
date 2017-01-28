// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/PartData.h"
#include "mx/api/StaffData.h"
#include "mx/core/elements/PartwiseMeasure.h"

namespace mx
{
    namespace impl
    {

        
        void createStavesFromMx( int inGlobalTicksPerQuarter, const core::PartwiseMeasureSet& inMxMeasures, std::vector<api::StaffData>& outStaves );
    }
}
