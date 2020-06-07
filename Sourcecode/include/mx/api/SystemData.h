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
            /// Should a system break occur at this measure.
            Bool systemBreak;

            /// System layout. Note, all members of SystemLayoutData are optional.
            SystemLayoutData layout;

            /// Returns true if any of the members of SystemData have values.
            inline bool isUsed() const { return systemBreak != Bool::unspecified || layout.isUsed(); }

            /// Explicit constructor.
            inline explicit SystemData( Bool inSystemBreak, SystemLayoutData inLayout = SystemLayoutData{} )
                : systemBreak{ inSystemBreak }
                , layout{ std::move( inLayout ) }
            {

            }

            /// Default constructor.
            SystemData()
                : SystemData{ Bool::unspecified  }
            {
                
            }
        };

        MXAPI_EQUALS_BEGIN( SystemData )
        MXAPI_EQUALS_MEMBER( systemBreak )
        MXAPI_EQUALS_MEMBER( layout )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( SystemData );
    }
}
