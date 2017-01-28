// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/DynamicsReader.h"
#include "mx/api/Smufl.h"
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

            if( dynamicType == core::DynamicsEnum::otherDynamics )
            {
                const auto value = myDynamic.getValue().getValueString();
                const auto tempChar = api::Smufl::findCodepoint( value );
                if( tempChar != 0 )
                {
                    markData.smuflName = value;
                    markData.smuflCodepoint = tempChar;
                }
            }
            else
            {
                markData.smuflName = api::MarkSmufl::getName( markType, markData.positionData.placement );
                markData.smuflCodepoint = api::MarkSmufl::getCodepoint( markType, markData.positionData.placement );
            }
            outMarks.emplace_back( std::move( markData ) );
        }
    }
}
