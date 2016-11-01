// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/impl/DynamicsWriter.h"
#include "mx/core/elements/Dynamics.h"
#include "mx/impl/MarkDataFunctions.h"

namespace mx
{
    namespace impl
    {
        DynamicsWriter::DynamicsWriter( const api::MarkData& inMark, impl::Cursor inCursor )
        : myMarkData{ inMark }
        , myCursor{ inCursor }
        , myDirectionPlacement( api::Placement::unspecified )
        , myConverter{}
        {
            MX_ASSERT( isMarkDynamic(inMark.markType ) );
        }
        
        
        DynamicsWriter::DynamicsWriter( const api::MarkData& inMark, impl::Cursor inCursor, api::Placement directionPlacement )
        : myMarkData{ inMark }
        , myCursor{ inCursor }
        , myDirectionPlacement( directionPlacement )
        , myConverter{}
        {
            MX_ASSERT( isMarkDynamic(inMark.markType ) );
        }
        
        core::DynamicsPtr DynamicsWriter::getDynamics() const
        {
            auto dyn = core::makeDynamics();
            
            // if( myMarkData.positionData.placement != api::Placement::unspecified )
            // {
            //     dyn->getAttributes()->hasPlacement = true;
            //     dyn->getAttributes()->placement = myConverter.convert( myMarkData.positionData.placement );
            // }
            const auto value = myConverter.convertDynamic( myMarkData.markType );
            core::DynamicsValue dynamicsValue;
            dynamicsValue.setValue( value );
            const bool isOther = value == core::DynamicsEnum::otherDynamics;
            
            if( isOther && !myMarkData.smuflName.empty() )
            {
                dynamicsValue.setValue( myMarkData.smuflName );
            }
            else if ( isOther && !myMarkData.name.empty() )
            {
                dynamicsValue.setValue( myMarkData.name );
            }
            impl::setAttributesFromMarkData( myMarkData, *dyn->getAttributes() );
            dyn->setValue( dynamicsValue );
            return dyn;
        }
    }
}
