// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/ScoreWriter.h"
#include "mx/core/elements/Creator.h"
#include "mx/core/elements/Encoding.h"
#include "mx/core/elements/GroupBarline.h"
#include "mx/core/elements/GroupName.h"
#include "mx/core/elements/Identification.h"
#include "mx/core/elements/Miscellaneous.h"
#include "mx/core/elements/MovementNumber.h"
#include "mx/core/elements/MovementTitle.h"
#include "mx/core/elements/PartGroup.h"
#include "mx/core/elements/GroupSymbol.h"
#include "mx/core/elements/PartGroupOrScorePart.h"
#include "mx/core/elements/PartList.h"
#include "mx/core/elements/PartwisePart.h"
#include "mx/core/elements/Relation.h"
#include "mx/core/elements/Rights.h"
#include "mx/core/elements/ScoreHeaderGroup.h"
#include "mx/core/elements/ScorePart.h"
#include "mx/core/elements/ScorePart.h"
#include "mx/core/elements/ScorePartwise.h"
#include "mx/core/elements/Source.h"
#include "mx/core/elements/Work.h"
#include "mx/core/elements/WorkNumber.h"
#include "mx/core/elements/WorkTitle.h"
#include "mx/impl/EncodingFunctions.h"
#include "mx/impl/LayoutFunctions.h"
#include "mx/impl/PageTextFunctions.h"
#include "mx/impl/PartReader.h"
#include "mx/impl/PartWriter.h"
#include "mx/impl/TimeReader.h"
#include "mx/impl/Converter.h"

namespace mx
{
    namespace impl
    {
        ScoreWriter::ScoreWriter( const api::ScoreData& inScoreData )
        : myScoreData{ inScoreData }
        , myMutex{}
        , myOutScorePartwise{ nullptr }
        {
            myScoreData.sort();
        }
        
        core::ScorePartwisePtr ScoreWriter::getScorePartwise() const
        {
            std::lock_guard<std::mutex> lock{ myMutex };
            myOutScorePartwise = core::makeScorePartwise();

            switch ( this->myScoreData.musicXmlVersion )
            {
                case api::MusicXmlVersion::unspecified:
                {
                    myOutScorePartwise->getAttributes()->hasVersion = false;
                    myOutScorePartwise->getAttributes()->version = core::XsToken{ "" };
                }
                    break;

                case api::MusicXmlVersion::ThreePointZero:
                {
                    myOutScorePartwise->getAttributes()->hasVersion = true;
                    myOutScorePartwise->getAttributes()->version = core::XsToken{ "3.0" };
                }
                    break;

                default:
                {
                    myOutScorePartwise->getAttributes()->hasVersion = false;
                    myOutScorePartwise->getAttributes()->version = core::XsToken{ "" };
                }
                    break;
            }

            auto header = myOutScorePartwise->getScoreHeaderGroup();
            
            header->setHasWork( !myScoreData.workTitle.empty() || !myScoreData.workNumber.empty() );
            header->getWork()->setHasWorkTitle( !myScoreData.workTitle.empty() );
            header->getWork()->getWorkTitle()->setValue( core::XsString( myScoreData.workTitle ) );
            header->getWork()->setHasWorkNumber( !myScoreData.workNumber.empty() );
            header->getWork()->getWorkNumber()->setValue( core::XsString( myScoreData.workNumber ) );
            
            header->setHasMovementTitle( !myScoreData.movementTitle.empty() );
            header->getMovementTitle()->setValue( core::XsString( myScoreData.movementTitle ) );
            
            header->setHasMovementNumber( !myScoreData.movementNumber.empty() );
            header->getMovementNumber()->setValue( core::XsString( myScoreData.movementNumber ) );
            
            if( !myScoreData.composer.empty() )
            {
                auto composer = core::makeCreator();
                composer->getAttributes()->hasType = true;
                composer->getAttributes()->type = core::XsToken( "composer" );
                composer->setValue( core::XsString( myScoreData.composer ) );
                header->setHasIdentification( true );
                header->getIdentification()->addCreator( composer );
            }
            
            if( !myScoreData.lyricist.empty() )
            {
                auto lyricist = core::makeCreator();
                lyricist->getAttributes()->hasType = true;
                lyricist->getAttributes()->type = core::XsToken( "lyricist" );
                lyricist->setValue( core::XsString( myScoreData.lyricist ) );
                header->setHasIdentification( true );
                header->getIdentification()->addCreator( lyricist );
            }
            
            if( !myScoreData.copyright.empty() )
            {
                auto copyright = core::makeRights();
                copyright->getAttributes()->hasType = true;
                copyright->getAttributes()->type = core::XsToken( "copyright" );
                copyright->setValue( core::XsString( myScoreData.copyright ) );
                header->setHasIdentification( true );
                header->getIdentification()->addRights( copyright );
            }
            
            createEncoding( myScoreData.encoding, *header );
            addLayoutData( myScoreData.defaults, *header );
            createPageTextItems( myScoreData.pageTextItems, *header );
            
            using PartPair = std::pair<core::ScorePartPtr, core::PartwisePartPtr>;
            using PartPairs = std::vector<PartPair>;

            int partIndex = 0;
            PartPairs partPairs;
            for( const auto& partData : myScoreData.parts )
            {
                PartWriter partWriter{ partData, partIndex, myScoreData.ticksPerQuarter, *this };
                partPairs.emplace_back( std::make_pair( partWriter.getScorePart(), partWriter.getPartwisePart() ) );
                ++partIndex;
            }
            
            partIndex = 0;
            for( const auto& partPair : partPairs )
            {
                addScorePart( partIndex, partPair.first );
                addPartwisePart( partIndex, partPair.second );
                ++partIndex;
            }

            return myOutScorePartwise;
        }
        
        
        void ScoreWriter::addScorePart( int partIndex, const core::ScorePartPtr& scorePart ) const
        {
            auto header = myOutScorePartwise->getScoreHeaderGroup();
            auto& partList = *header->getPartList();
            
            auto reverseStopsCompare = [&]( api::PartGroupData& a, api::PartGroupData& b )
            {
                return a.number > b.number;
            };
            
            if ( partIndex == 0 )
            {
                if( partGroupStartExists( partIndex ) )
                {
                    const auto grps = findPartGroupsByStartIndex( partIndex );
                    
                    for( const auto& g : grps )
                    {
                        partList.addPartGroup( makePartGroupStart( g ) );
                    }
                }
                partList.setScorePart( scorePart );
                
                if( partGroupStopExists( partIndex ) )
                {
                    auto grps = findPartGroupsByStopIndex( partIndex );
                    std::sort( grps.begin(), grps.end(), reverseStopsCompare );
                    
                    for( const auto& g : grps )
                    {
                        partList.addPartGroupOrScorePart( makePartGroupOrScorePart( makePartGroupStop( g ) ) );
                    }
                }
            }
            else
            {
                if( partGroupStartExists( partIndex ) )
                {
                    const auto grps = findPartGroupsByStartIndex( partIndex );
                    
                    for( const auto& g : grps )
                    {
                        partList.addPartGroupOrScorePart( makePartGroupOrScorePart( makePartGroupStart( g ) ) );
                    }
                }
                partList.addPartGroupOrScorePart( makePartGroupOrScorePart( scorePart ) );
                
                if( partGroupStopExists( partIndex ) )
                {
                    auto grps = findPartGroupsByStopIndex( partIndex );
                    std::sort( grps.begin(), grps.end(), reverseStopsCompare );
                    
                    for( const auto& g : grps )
                    {
                        partList.addPartGroupOrScorePart( makePartGroupOrScorePart( makePartGroupStop( g ) ) );
                    }
                }
            }
        }
        
        
        void ScoreWriter::addPartwisePart( int partIndex, const core::PartwisePartPtr& partwisePart ) const
        {
            myOutScorePartwise->addPartwisePart( partwisePart );
            auto& partSet = myOutScorePartwise->getPartwisePartSet();
            
            if( partIndex == 0 && partSet.size() == 2 )
            {
                myOutScorePartwise->removePartwisePart( partSet.cbegin() );
            }
        }
        
        
        bool ScoreWriter::partGroupStartExists( int partIndex ) const
        {
            for( const auto& g : myScoreData.partGroups )
            {
                if( g.firstPartIndex == partIndex )
                {
                    return true;
                }
            }
            return false;
        }

        
        bool ScoreWriter::partGroupStopExists( int partIndex ) const
        {
            for( const auto& g : myScoreData.partGroups )
            {
                if( g.lastPartIndex == partIndex )
                {
                    return true;
                }
            }
            return false;
        }
        
        
        std::vector<api::PartGroupData> ScoreWriter::findPartGroupsByStartIndex( int partIndex ) const
        {
            std::vector<api::PartGroupData> outGroups;
            for( const auto& g : myScoreData.partGroups )
            {
                if( g.firstPartIndex == partIndex )
                {
                    outGroups.push_back( g );
                }
            }
            return outGroups;
        }
        
        
        std::vector<api::PartGroupData> ScoreWriter::findPartGroupsByStopIndex( int partIndex ) const
        {
            std::vector<api::PartGroupData> outGroups;
            for( const auto& g : myScoreData.partGroups )
            {
                if( g.lastPartIndex == partIndex )
                {
                    outGroups.push_back( g );
                }
            }
            return outGroups;
        }
        
        
        core::PartGroupPtr ScoreWriter::makePartGroupStart( const api::PartGroupData& apiGrp ) const
        {
            auto mxGrp = core::makePartGroup();
            mxGrp->getAttributes()->type = core::StartStop::start;

            if( apiGrp.number >= 0 )
            {
                mxGrp->getAttributes()->hasNumber = true;
                mxGrp->getAttributes()->number = std::to_string( apiGrp.number );
            }

            if( apiGrp.name.size() > 0 )
            {
                mxGrp->setHasGroupName( true );
                mxGrp->getGroupName()->setValue( core::XsString{ apiGrp.name} );
            }

            Converter converter;
            if( apiGrp.bracketType != api::BracketType::unspecified )
            {
                const auto symbol = converter.convert( apiGrp.bracketType );
                mxGrp->setHasGroupSymbol( true );
                mxGrp->getGroupSymbol()->setValue( symbol );
            }

            // TODO - make group barline configurable

            mxGrp->setHasGroupBarline( true );
            mxGrp->getGroupBarline()->setValue( core::GroupBarlineValue::yes );

            return mxGrp;
        }
        
        
        core::PartGroupPtr ScoreWriter::makePartGroupStop( const api::PartGroupData& apiGrp ) const
        {
            auto mxGrp = core::makePartGroup();
            mxGrp->getAttributes()->type = core::StartStop::stop;
            if( apiGrp.number >= 0 )
            {
                mxGrp->getAttributes()->hasNumber = true;
                mxGrp->getAttributes()->number = std::to_string( apiGrp.number );
            }
            return mxGrp;
        }
        
        
        core::PartGroupOrScorePartPtr ScoreWriter::makePartGroupOrScorePart( const core::PartGroupPtr& grp ) const
        {
            auto pgosp = core::makePartGroupOrScorePart();
            pgosp->setChoice( core::PartGroupOrScorePart::Choice::partGroup );
            pgosp->setPartGroup( grp );
            return pgosp;
        }
        
        
        core::PartGroupOrScorePartPtr ScoreWriter::makePartGroupOrScorePart( const core::ScorePartPtr& spr ) const
        {
            auto pgosp = core::makePartGroupOrScorePart();
            pgosp->setChoice( core::PartGroupOrScorePart::Choice::scorePart );
            pgosp->setScorePart( spr );
            return pgosp;
        }
        
        
        bool ScoreWriter::isStartOfSystem( int measureIndex ) const
        {
            const auto iter = myScoreData.xxbadnames.find( measureIndex );
            if( iter == std::cend( myScoreData.xxbadnames ) )
            {
                return false;
            }
            return iter->second.system.systemBreak == api::Bool::yes;
        }


        std::optional<api::PageData> ScoreWriter::findPageLayoutData( api::MeasureIndex measureIndex ) const
        {
            const auto iter = myScoreData.xxbadnames.find( measureIndex );
            if( iter == std::cend( myScoreData.xxbadnames ) )
            {
                return std::nullopt;
            }
            if( !iter->second.page.isUsed() )
            {
                return std::nullopt;
            }
            return iter->second.page;
        }
        
        
        api::SystemData ScoreWriter::getSystemData( int measureIndex ) const
        {
            const auto iter = myScoreData.xxbadnames.find( measureIndex );
            if( iter == std::cend( myScoreData.xxbadnames ) )
            {
                return api::SystemData{};
            }
            return iter->second.system;
        }
    }
}
