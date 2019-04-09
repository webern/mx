// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/StaffFunctions.h"
#include "mx/core/elements/Backup.h"
#include "mx/core/elements/Direction.h"
#include "mx/core/elements/EditorialVoiceGroup.h"
#include "mx/core/elements/Forward.h"
#include "mx/core/elements/Harmony.h"
#include "mx/core/elements/MusicDataChoice.h"
#include "mx/core/elements/MusicDataGroup.h"
#include "mx/core/elements/Note.h"
#include "mx/core/elements/Properties.h"
#include "mx/core/elements/Staff.h"
#include "mx/core/elements/Staves.h"
#include "mx/core/elements/Voice.h"
#include "mx/utility/Throw.h"

#include <map>

namespace mx
{
    namespace impl
    {
        
        int findStaffCountInOneMeasure( const core::MusicDataChoiceSet& inMusic );
        int findStaffCountInAllMeasures( const core::PartwiseMeasureSet& measures );
        
        
        void createStavesFromMx( int inGlobalTicksPerQuarter, const core::PartwiseMeasureSet& inMxMeasures, std::vector<api::StaffData>& outStaves )
        {
            MX_UNUSED( inGlobalTicksPerQuarter );
            MX_UNUSED( inMxMeasures );
            MX_UNUSED( outStaves );
#if 0
            const int staffCount = findStaffCountInAllMeasures( inMxMeasures );
            outStaves.clear();
            outStaves.resize( static_cast<size_t>( staffCount ) );
            auto mxMeasureIter = inMxMeasures.cbegin();
            auto mxMeasureEnd = inMxMeasures.cend();
            bool isFirstMeasure = true;
            IntMeasureDataMap currentMeasureData;
            
            impl::MeasureFunctions measureFunc{ staffCount, inGlobalTicksPerQuarter  };
            
            for( ; mxMeasureIter != mxMeasureEnd; ++mxMeasureIter )
            {
                auto currentMeasurePropertiesPayload = MeasurePropertiesPayload{};
                currentMeasureData = measureFunc.parseMeasure( **mxMeasureIter, currentMeasurePropertiesPayload );
                isFirstMeasure = false;
                
                
                if( currentMeasureData.size() != outStaves.size() ) { MX_THROW( "currentMeasureData.size() != outStaves.size()" ); } // TODO - remove this
                auto it = currentMeasureData.cbegin();
                
                for( int i = 0; i < static_cast<int>( outStaves.size() ) && it != currentMeasureData.cend(); ++i, ++it )
                {
                    if( i != it->first )
                    {
                        MX_THROW( "i != it->first" ); // TODO - remove this
                    }
                }
                
                for( int i = 0; i < static_cast<int>( outStaves.size() ); ++i )
                {
                    //auto& staff = outStaves.at( static_cast<size_t>( i ) );
                    auto& measureData = currentMeasureData.at( i );
                    measureData.multiMeasureRestCount = currentMeasurePropertiesPayload.multipleRest;
                    //staff.measures.push_back( measureData );
                }
            }
#endif
        }
        
        
        int findStaffCountInOneMeasure( const core::MusicDataChoiceSet& inMusic )
        {
            int outStaffCount = 1;
            
            for( const auto& mdc : inMusic )
            {
                if( mdc->getChoice() == core::MusicDataChoice::Choice::properties )
                {
                    const auto item = mdc->getProperties();
                    int tempStaffCount = 1;
                    
                    if( item->getHasStaves() )
                    {
                        tempStaffCount = item->getStaves()->getValue().getValue();
                        
                        if( tempStaffCount > outStaffCount )
                        {
                            outStaffCount = tempStaffCount;
                        }
                    }
                }
                
                mx::core::StaffPtr staff = nullptr;
                
                if( mdc->getChoice() == core::MusicDataChoice::Choice::note )
                {
                    const auto& item = *mdc->getNote();
                    if( item.getHasStaff() )
                    {
                        staff = item.getStaff();
                    }
                }
                
                if( mdc->getChoice() == core::MusicDataChoice::Choice::direction )
                {
                    const auto& item = *mdc->getDirection();
                    if( item.getHasStaff() )
                    {
                        staff = item.getStaff();
                    }
                }
                
                if( mdc->getChoice() == core::MusicDataChoice::Choice::forward )
                {
                    const auto& item = *mdc->getForward();
                    if( item.getHasStaff() )
                    {
                        staff = item.getStaff();
                    }
                }
                
                
                if( mdc->getChoice() == core::MusicDataChoice::Choice::harmony )
                {
                    const auto& item = *mdc->getHarmony();
                    if( item.getHasStaff() )
                    {
                        staff = item.getStaff();
                    }
                }
                
                if(staff)
                {
                    const int tempVal = static_cast<int>( staff->getValue().getValue() );
                    if( tempVal > outStaffCount )
                    {
                        outStaffCount = tempVal;
                    }
                }
                
                // TODO - look at all the notes and items for staff values
            }
            return outStaffCount;
        }
        
        
        int findStaffCountInAllMeasures( const core::PartwiseMeasureSet& measures )
        {
            int outStaffCount = 1;
            
            for( const auto& measure : measures )
            {
                const int tempCount = findStaffCountInOneMeasure( measure->getMusicDataGroup()->getMusicDataChoiceSet() );
                
                if( tempCount > outStaffCount )
                {
                    outStaffCount = tempCount;
                }
            }
            
            return outStaffCount;
        }
        
        
        
    }
}
