// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/SystemLayoutData.h"

namespace mx
{
    namespace api
    {
        // TODO - the documentation below if from MusicXML XSD and not very helpful for our case, improve documentation.
        // A system is a group of staves that are read and played simultaneously.
        // System layout includes left and right margins and the vertical distance
        // from the previous system. The system distance is measured from the
        // bottom line of the previous system to the top line of the current
        // system. It is ignored for the first system on a page. The top system
        // distance is measured from the page's top margin to the top line of the
        // first system. It is ignored for all but the first system on a page.
        //
        // Sometimes the sum of measure widths in a system may not equal the system
        // width specified by the layout elements due to roundoff or other errors.
        // The behavior when reading MusicXML files in these cases is application-
        // dependent. For instance, applications may find that the system layout
        // data is more reliable than the sum of the measure widths, and adjust the
        // measure widths accordingly.
        class SystemData
        {
        public:
            
            // instead of placing these within the part and measure data (like
            // MusicXML does), the SystemData class will declaritively specify
            // instructions about the system layout at the top-level of the
            // score.  Specify the measure index for the first measure in the
            // system. 'new-system' will be set to yes for the this measure.
            int measureIndex;

            /// System layout. Note, all members of SystemLayoutData are optional.
            SystemLayoutData layout;
            
            SystemData()
            : measureIndex{ -1 }
            , layout{}
            {
                
            }
        };
        
        inline bool operator<( const SystemData& lhs, const SystemData& rhs ) { return lhs.measureIndex < rhs.measureIndex; }

        MXAPI_EQUALS_BEGIN( SystemData )
        MXAPI_EQUALS_MEMBER( measureIndex )
        MXAPI_EQUALS_MEMBER( layout )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( SystemData );
    }
}
