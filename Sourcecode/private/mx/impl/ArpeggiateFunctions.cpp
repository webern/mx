// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Arpeggiate.h"
#include "mx/impl/ArpeggiateFunctions.h"
#include "mx/impl/MarkDataFunctions.h"

namespace mx
{
    namespace impl
    {

    ArpeggiateFunctions::ArpeggiateFunctions( const core::Arpeggiate& inArpeggiate, impl::Cursor inCursor )
        : myArpeggiate{ inArpeggiate }
        , myCursor{ inCursor }
        {
            
        }
        
        api::MarkData ArpeggiateFunctions::parseArpeggiate() const
        {
            const auto& attr = myArpeggiate.getAttributes();
            auto markType = api::MarkType::arpeggiate;
            if (attr->hasDirection) {
                
                switch (attr->direction)
                {
                    case core::UpDownNone::up:
                        markType = api::MarkType::arpeggiateUp;
                        break;
                        
                    case core::UpDownNone::down:
                        markType = api::MarkType::arpeggiateDown;
                        break;

                case core::UpDownNone::none:
                    markType = api::MarkType::arpeggiate;
                    break;
                }
            }
            api::MarkData markData{ markType };
            impl::parseMarkDataAttributes( attr, markData );
            markData.tickTimePosition = myCursor.tickTimePosition;
            
            return markData;
        }
    }
}
