// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/impl/AccidentalMarkFunctions.h"
#include "mx/core/elements/AccidentalMark.h"
#include "mx/impl/ParseMarkDataAttributes.h"

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
            parseMarkDataAttributes( myAccidentalMark, markData );
            
            markData.smuflName = api::MarkSmufl::getName( markType, markData.positionData.placement );
            markData.smuflCodepoint = api::MarkSmufl::getCodepoint( markType, markData.positionData.placement );
            return markData;
        }
    }
}
