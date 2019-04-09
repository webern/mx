// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/DynamicsReader.h"
#include "mx/core/elements/Dynamics.h"
#include "mx/core/Enums.h"
#include "mx/impl/Converter.h"
#include "mx/impl/MarkDataFunctions.h"

#include <mutex>

namespace mx
{
    namespace impl
    {
        DynamicsReader::DynamicsReader( const core::Dynamics& inDynamic, impl::Cursor inCursor )
        : myDynamic{ inDynamic }
        , myCursor{ inCursor }
        {
            
        }
        
        
        void DynamicsReader::parseDynamics( std::vector<api::MarkData>& outMarks ) const
        {
            const auto dynamicType = myDynamic.getValue().getValue();
            Converter converter;
            const auto markType = converter.convertDynamic( dynamicType );
            auto markData = api::MarkData{};
            markData.markType = markType;
            markData.tickTimePosition = myCursor.tickTimePosition;
            markData.name = myDynamic.getValue().getValueString();
            markData.positionData = impl::getPositionData( *myDynamic.getAttributes() );
            outMarks.emplace_back( std::move( markData ) );
        }
    }
}
