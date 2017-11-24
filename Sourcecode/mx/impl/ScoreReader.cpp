// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/ScoreReader.h"
#include "mx/core/elements/Creator.h"
#include "mx/core/elements/Encoding.h"
#include "mx/core/elements/EncodingChoice.h"
#include "mx/core/elements/GroupAbbreviation.h"
#include "mx/core/elements/GroupAbbreviationDisplay.h"
#include "mx/core/elements/GroupName.h"
#include "mx/core/elements/GroupNameDisplay.h"
#include "mx/core/elements/GroupSymbol.h"
#include "mx/core/elements/Identification.h"
#include "mx/core/elements/Miscellaneous.h"
#include "mx/core/elements/MovementNumber.h"
#include "mx/core/elements/MovementTitle.h"
#include "mx/core/elements/Opus.h"
#include "mx/core/elements/PartGroup.h"
#include "mx/core/elements/PartGroupOrScorePart.h"
#include "mx/core/elements/PartList.h"
#include "mx/core/elements/PartwiseMeasure.h"
#include "mx/core/elements/PartwisePart.h"
#include "mx/core/elements/Relation.h"
#include "mx/core/elements/Rights.h"
#include "mx/core/elements/ScoreHeaderGroup.h"
#include "mx/core/elements/ScorePart.h"
#include "mx/core/elements/ScorePartwise.h"
#include "mx/core/elements/Source.h"
#include "mx/core/elements/Work.h"
#include "mx/core/elements/WorkNumber.h"
#include "mx/core/elements/WorkTitle.h"
#include "mx/impl/Converter.h"
#include "mx/impl/EncodingFunctions.h"
#include "mx/impl/LayoutFunctions.h"
#include "mx/impl/PageTextFunctions.h"
#include "mx/impl/PartReader.h"
#include "mx/impl/TimeReader.h"
#include "mx/utility/StringToInt.h"
#include "mx/core/elements/MusicDataGroup.h"
#include "mx/core/elements/MusicDataChoice.h"
#include "mx/core/elements/Print.h"
#include "mx/core/elements/LayoutGroup.h"
#include "mx/core/elements/MeasureLayout.h"
#include "mx/core/elements/MeasureNumbering.h"
#include "mx/core/elements/SystemLayout.h"
#include "mx/core/elements/SystemDistance.h"
#include "mx/core/elements/SystemDividers.h"
#include "mx/core/elements/SystemMargins.h"
#include "mx/core/elements/TopSystemDistance.h"
#include "mx/core/elements/LeftMargin.h"
#include "mx/core/elements/RightMargin.h"
#include "mx/core/elements/Properties.h"
#include "mx/core/elements/Divisions.h"
#include "mx/core/elements/MiscellaneousField.h"
#include "mx/impl/LcmGcd.h"

namespace mx
{
    namespace impl
    {
        ScoreReader::ScoreReader( const core::ScorePartwise& inScorePartwise )
        : myScorePartwise{ inScorePartwise }
        , myPartSet{ inScorePartwise.getPartwisePartSet() }
        , myHeaderGroup{ *inScorePartwise.getScoreHeaderGroup() }
        , myMutex{}
        , myOutScoreData{}
        , myPartGroupStack{}
        {
            
        }
        
        const core::PartwisePartPtr ScoreReader::findPartwisePart( const core::ScorePartPtr& scorePartPtr, const core::PartwisePartSet& partwiseParts ) const
        {
            mx::core::PartwisePartPtr outPartwisePartPtr = nullptr;
            
            auto lambda = [&]( const mx::core::PartwisePartPtr& partwisePartPtr )
            {
                const auto ppId = partwisePartPtr->getAttributes()->id.getValue();
                const auto spId = scorePartPtr->getAttributes()->id.getValue();
                return ppId == spId;
                
            };
            
            auto iter = std::find_if( partwiseParts.cbegin(), partwiseParts.cend(), lambda );
            
            if( iter != partwiseParts.cend() )
            {
                outPartwisePartPtr = *iter;
            }
            
            return outPartwisePartPtr;
        }
        
        ScoreReader::ReconciledParts ScoreReader::reconcileParts( const core::ScorePartwise& inScorePartwise ) const
        {
            ReconciledParts outParts;
            int partIndex = 0;
            
            const auto& partList = *inScorePartwise.getScoreHeaderGroup()->getPartList();
            const auto& partwiseParts = inScorePartwise.getPartwisePartSet();
            
            for( const auto& preliminaryGroup : partList.getPartGroupSet() )
            {
                handlePartGroup(partIndex, preliminaryGroup );
            }
            
            auto scorePart = partList.getScorePart();
            auto partwisePart = findPartwisePart( scorePart, partwiseParts );
            if( scorePart && partwisePart )
            {
                outParts.emplace_back( ReconciledPart{ scorePart, partwisePart } );
            }
            
            ++partIndex;
            
            for( const auto& pgosp : partList.getPartGroupOrScorePartSet() )
            {
                if( pgosp->getChoice() == mx::core::PartGroupOrScorePart::Choice::scorePart )
                {
                    scorePart = pgosp->getScorePart();
                    partwisePart = findPartwisePart( scorePart, partwiseParts );
                    
                    if( scorePart && partwisePart )
                    {
                        outParts.emplace_back( ReconciledPart{ scorePart, partwisePart } );
                    }
                    ++partIndex;
                }
                else if( pgosp->getChoice() == mx::core::PartGroupOrScorePart::Choice::partGroup )
                {
                    handlePartGroup( partIndex, pgosp->getPartGroup() );
                }
            }
            
            
            auto sortingCompare = [&]( api::PartGroupData& a, api::PartGroupData b )
            {
                if( a.firstPartIndex < b.firstPartIndex )
                {
                    return true;
                }
                else if ( a.firstPartIndex > b.firstPartIndex )
                {
                    return false;
                }
                else if ( a.number < b.number )
                {
                    return true;
                }
                else if ( a.number > b.number )
                {
                    return false;
                }
                return false;
            };
            
            std::sort( myOutScoreData.partGroups.begin(), myOutScoreData.partGroups.end(), sortingCompare );
            
            return outParts;
        }
        
        api::ScoreData ScoreReader::getScoreData() const
        {
            std::lock_guard<std::mutex> lock{ myMutex };
            myOutScoreData = api::ScoreData{};
            myPartGroupStack = std::list<api::PartGroupData>{};
            
            myOutScoreData.ticksPerQuarter = findMaxDivisionsPerQuarter();

            if( myHeaderGroup.getHasWork() && myHeaderGroup.getWork()->getHasWorkTitle() )
            {
                myOutScoreData.workTitle = myHeaderGroup.getWork()->getWorkTitle()->getValue().getValue();
            }
            
            if( myHeaderGroup.getHasWork() && myHeaderGroup.getWork()->getHasWorkNumber() )
            {
                myOutScoreData.workNumber = myHeaderGroup.getWork()->getWorkNumber()->getValue().getValue();
            }
            
            if( myHeaderGroup.getHasMovementTitle() )
            {
                myOutScoreData.movementTitle = myHeaderGroup.getMovementTitle()->getValue().getValue();
            }
            
            if( myHeaderGroup.getHasMovementNumber() )
            {
                myOutScoreData.movementNumber = myHeaderGroup.getMovementNumber()->getValue().getValue();
            }
            
            bool isComposerFound = false;
            bool isCopyrightFound = false;
            bool isArrangerFound = false;
            bool isLyricistFound = false;
            bool isPublisherFound = false;
            
            if( myHeaderGroup.getHasIdentification() )
            {
                
                for( auto i : myHeaderGroup.getIdentification()->getCreatorSet() )
                {
                    auto a = i->getAttributes();
                    if( a->type.getValue() == "composer" && !isComposerFound )
                    {
                        myOutScoreData.composer = i->getValue().getValue();
                        isComposerFound = true;
                    }
                    
                    if( !isComposerFound && !a->hasType )
                    {
                        myOutScoreData.composer = i->getValue().getValue();
                    }
                    
                    if( a->type.getValue() == "lyricist" )
                    {
                        myOutScoreData.lyricist = i->getValue().getValue();
                        isLyricistFound = true;
                    }
                    
                    if( a->type.getValue() == "arranger" )
                    {
                        myOutScoreData.lyricist = i->getValue().getValue();
                        isArrangerFound = true;
                    }
                    
                    if( a->type.getValue() == "publisher" )
                    {
                        myOutScoreData.lyricist = i->getValue().getValue();
                        isPublisherFound = true;
                    }
                }
                
                for( auto r : myHeaderGroup.getIdentification()->getRightsSet() )
                {
                    auto a = r->getAttributes();
                    if( a->hasType && a->type.getValue() == "copyright" && !isCopyrightFound )
                    {
                        myOutScoreData.copyright = r->getValue().getValue();
                        isCopyrightFound = true;
                    }
                    
                    if( !isCopyrightFound && !a->hasType )
                    {
                        myOutScoreData.copyright = r->getValue().getValue();
                    }
                }
                api::EncodingData encodingData;
                
                if( myHeaderGroup.getHasIdentification() && myHeaderGroup.getIdentification()->getHasEncoding() )
                {
                    encodingData = createEncoding( *myHeaderGroup.getIdentification()->getEncoding() );
                }
                
                if( myHeaderGroup.getIdentification()->getHasMiscellaneous() )
                {
                    for( const auto m : myHeaderGroup.getIdentification()->getMiscellaneous()->getMiscellaneousFieldSet() )
                    {
                        std::string key;
                        if( m->getAttributes()->hasName )
                        {
                            key = m->getAttributes()->name.getValue();
                        }
                        encodingData.miscelaneousFields.emplace_back( m->getAttributes()->name.getValue(), m->getValue().getValue() );
                    }
                }
                
                myOutScoreData.encoding = std::move(encodingData);
            }
            
            if( myHeaderGroup.getHasDefaults() )
            {
                myOutScoreData.layout = createLayout( myHeaderGroup );
            }

            createPageTextItems( myHeaderGroup, myOutScoreData.pageTextItems );
            
            auto partMap = reconcileParts( myScorePartwise );
            for( const auto& reconciledPart : partMap )
            {
                PartReader reader{ *reconciledPart.first, *reconciledPart.second, myOutScoreData.ticksPerQuarter };
                myOutScoreData.parts.emplace_back( reader.getPartData() );
            }
            
            scanForSystemInfo();
            
            api::ScoreData temp{ std::move( myOutScoreData ) };
            myOutScoreData = api::ScoreData{};
            return temp;
        }
        
        void ScoreReader::handlePartGroup( int partIndex, const core::PartGroupPtr& inPartGroup ) const
        {
            const auto startStop = inPartGroup->getAttributes()->type;
            if ( startStop == mx::core::StartStop::start )
            {
                startPartGroup( partIndex, inPartGroup );
            }
            else
            {
                stopPartGroup( partIndex, inPartGroup );
            }
        }
        
        
        void ScoreReader::startPartGroup( int partIndex, const core::PartGroupPtr& inPartGroup ) const
        {
            api::PartGroupData grpData;
            grpData.number = parsePartGroupNumber( inPartGroup );
            
            if( inPartGroup->getHasGroupName() )
            {
                grpData.name = inPartGroup->getGroupName()->getValue().getValue();
            }

            if( inPartGroup->getHasGroupAbbreviation() )
            {
                grpData.abbreviation = inPartGroup->getGroupAbbreviation()->getValue().getValue();
            }

            if (inPartGroup->getHasGroupSymbol() )
            {
                Converter c;
                grpData.bracketType = c.convert( inPartGroup->getGroupSymbol()->getValue() );
            }

            grpData.firstPartIndex = partIndex;
            
            // TODO - group name display
            // TODO - group abbreviation display
            // TODO - barline, etc

            myPartGroupStack.push_front( grpData );
        }
        
        
        void ScoreReader::stopPartGroup( int partIndex, const core::PartGroupPtr& inPartGroup ) const
        {
            if( myPartGroupStack.empty() )
            {
                return;
            }
            const int partGroupNumber = parsePartGroupNumber( inPartGroup );

            api::PartGroupData grpData;
            
            if( groupNumberExistsOnStack( partGroupNumber ) )
            {
                grpData = popGroupFromStack( partGroupNumber );
            }
            else
            {
                grpData = popMostRecentGroupFromStack();
            }
            
            grpData.lastPartIndex = partIndex > 0 ? partIndex - 1 : partIndex;
            
            myOutScoreData.partGroups.emplace_back( std::move( grpData ) );
        }
        
        
        bool ScoreReader::groupNumberExistsOnStack( int groupNumber ) const
        {
            for( const auto& g : myPartGroupStack )
            {
                if( g.number == groupNumber )
                {
                    return true;
                }
            }
            return false;
        }
        
        
        api::PartGroupData ScoreReader::popGroupFromStack( int groupNumber ) const
        {
            auto lambda = [&]( const api::PartGroupData& grp )
            {
                return grp.number == groupNumber;
            };

            auto iter = std::find_if( myPartGroupStack.cbegin(), myPartGroupStack.cend(), lambda );
            
            if( iter == myPartGroupStack.cend() )
            {
                MX_BUG;
            }
            auto outGroup = *iter;
            myPartGroupStack.erase( iter );
            return outGroup;
        }
        
        
        api::PartGroupData ScoreReader::popMostRecentGroupFromStack() const
        {
            if( myPartGroupStack.empty() )
            {
                MX_BUG;
            }
            
            auto iter = myPartGroupStack.cbegin();
            auto outGroup = *iter;
            myPartGroupStack.erase( iter );
            return outGroup;
        }
        
        
        int ScoreReader::parsePartGroupNumber( const core::PartGroupPtr& inPartGroup ) const
        {
            int num = -1;
            
            if( inPartGroup->getAttributes()->hasNumber )
            {
                
                const auto str = inPartGroup->getAttributes()->number.getValue();
                bool isGroupNumberValid = utility::stringToInt( str, num );
                
                // TODO - support non-numeric group numbers if someone complains
                MX_ASSERT( isGroupNumberValid );
            }
            
            return num;
        }
        
        void ScoreReader::scanForSystemInfo() const
        {
            // scan only the top part looking for system layout information
            if( myScorePartwise.getPartwisePartSet().size() == 0 )
            {
                return;
            }
            
            const auto& topPart = *myScorePartwise.getPartwisePartSet().front();
            
            int measureIndex = 0;
            for( const auto& m : topPart.getPartwiseMeasureSet() )
            {
                const auto& measure = *m;
                for( const auto& mdc : measure.getMusicDataGroup()->getMusicDataChoiceSet() )
                {
                    if( !( mdc->getChoice() == core::MusicDataChoice::Choice::print ) )
                    {
                        continue;
                    }
                    const auto& print = *mdc->getPrint();
                    const auto& layoutGroup = *print.getLayoutGroup();
                    if( layoutGroup.getHasSystemLayout() )
                    {
                        auto systemData = api::SystemData{};
                        systemData.measureIndex = measureIndex;
                        const auto& systemLayout = *layoutGroup.getSystemLayout();
                        if( systemLayout.getHasSystemMargins() )
                        {
                            const auto& margins = *systemLayout.getSystemMargins();
                            systemData.isMarginSpecified = true;
                            systemData.leftMargin = static_cast<int>( std::ceil( margins.getLeftMargin()->getValue().getValue() - 0.5 ) );
                            systemData.rightMargin = static_cast<int>( std::ceil( margins.getRightMargin()->getValue().getValue() - 0.5 ) );
                        }
                        
                        if( systemLayout.getHasSystemDistance() )
                        {
                            systemData.isSystemDistanceSpecified = true;
                            systemData.systemDistance = static_cast<int>( std::ceil( systemLayout.getSystemDistance()->getValue().getValue() - 0.5 ) );
                        }
                        
                        if( systemLayout.getHasTopSystemDistance() )
                        {
                            systemData.isTopSystemDistanceSpecified = true;
                            systemData.topSystemDistance = static_cast<int>( std::ceil( systemLayout.getTopSystemDistance()->getValue().getValue() - 0.5 ) );
                        }
                        
                        if( myOutScoreData.systems.size() == 0 && systemData.measureIndex > 0 )
                        {
                            // ensure we have one of these for the first system even if it was
                            // missing from the MusicXML file
                            auto firstSystem = api::SystemData{};
                            firstSystem.measureIndex = 0;
                            myOutScoreData.systems.emplace( std::move( firstSystem ) );
                        }
                        myOutScoreData.systems.emplace( std::move( systemData ) );
                    }
                }
                ++measureIndex;
            }
        }
        
        
        int ScoreReader::findMaxDivisionsPerQuarter() const
        {
            std::set<int> foundDivisions;
            
            for( const auto& pp : myScorePartwise.getPartwisePartSet() )
            {
                for( const auto& m : pp->getPartwiseMeasureSet() )
                {
                    for( const auto& mdc : m->getMusicDataGroup()->getMusicDataChoiceSet() )
                    {
                        if( mdc->getChoice() != core::MusicDataChoice::Choice::properties )
                        {
                            continue;
                        }
                        
                        const auto& props = *mdc->getProperties();
                        
                        if( !props.getHasDivisions() )
                        {
                            continue;
                        }
                        
                        const auto& divisions = *props.getDivisions();
                        const auto tempDiv = divisions.getValue().getValue();
                        const int tempDivInt = static_cast<int>( std::ceil( tempDiv - 0.5 ) );
                        if( tempDivInt > 0 )
                        {
                            foundDivisions.insert( tempDivInt );
                        }
                    }
                }
            }
            
            return mx::impl::leastCommonMultiple( foundDivisions );
        }
        
    }
}
