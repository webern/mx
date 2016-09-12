// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/impl/StaffFunctions.h"

namespace mx
{
    namespace impl
    {
        void createOneMeasure( int inMeasureIndex, const core::PartwiseMeasure& inMxMeasure, std::vector<api::StaffData>& outStaves );

        void createStavesFromMx( const core::PartwiseMeasureSet& inMxMeasures, std::vector<api::StaffData>& outStaves )
        {
            int measureIndex = 0;
            for( const auto& mxMeasure: inMxMeasures )
            {
                createOneMeasure( measureIndex++, *mxMeasure, outStaves );
            }
        }
        
        void createOneMeasure( int inMeasureIndex, const core::PartwiseMeasure& inMxMeasure, std::vector<api::StaffData>& outStaves )
        {
            std::cout << " meausre " << inMeasureIndex << std::endl;
        }

    }
}
