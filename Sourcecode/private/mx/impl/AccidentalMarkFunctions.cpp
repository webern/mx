// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/AccidentalMarkFunctions.h"
#include "mx/core/elements/AccidentalMark.h"
#include "mx/impl/MarkDataFunctions.h"

namespace mx
{
    namespace impl
    {

        AccidentalMarkFunctions::AccidentalMarkFunctions( const core::AccidentalMark& inAccidentalMark, impl::Cursor inCursor )
        : myAccidentalMark{ inAccidentalMark }
        , myCursor{ inCursor }
        {
            
        }
        
        api::MarkData AccidentalMarkFunctions::parseAccidentalMark() const
        {
            const auto accidentalValue = myAccidentalMark.getValue();
            Converter converter;
            const auto markType = converter.convertAccidentalMark( accidentalValue );
            auto markData = api::MarkData{};
            markData.markType = markType;
            markData.tickTimePosition = myCursor.tickTimePosition;
            
            markData.name = core::toString( accidentalValue );
            markData.positionData = impl::getPositionData( *myAccidentalMark.getAttributes() );
            
            return markData;
        }
    }
}
