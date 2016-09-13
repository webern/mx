// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/api/StaffData.h"
#include "mx/api/PartData.h"
#include "mx/core/elements/PartwiseMeasure.h"

namespace mx
{
    namespace impl
    {
        void createStavesFromMx( int inGlobalTicksPerQuarter, const core::PartwiseMeasureSet& inMxMeasures, std::vector<api::StaffData>& outStaves );
    }
}
