// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/impl/PartFunctions.h"
#include "mx/impl/StaffFunctions.h"
#include "mx/utility/Throw.h"
#include "mx/core/elements/ScorePartwise.h"
#include "mx/core/elements/ScoreHeaderGroup.h"
#include "mx/core/elements/PartList.h"
#include "mx/core/elements/ScorePart.h"
#include "mx/core/elements/PartGroup.h"
#include "mx/core/elements/PartGroupOrScorePart.h"
#include "mx/core/elements/PartName.h"
#include "mx/core/elements/GroupName.h"
#include "mx/core/elements/GroupAbbreviation.h"
#include "mx/core/elements/GroupNameDisplay.h"
#include "mx/core/elements/GroupAbbreviationDisplay.h"
#include "mx/core/elements/DisplayTextOrAccidentalText.h"
#include "mx/core/elements/DisplayText.h"
#include "mx/core/elements/AccidentalText.h"
#include "mx/core/elements/PartNameDisplay.h"
#include "mx/core/elements/PartAbbreviationDisplay.h"
#include "mx/core/elements/PartAbbreviation.h"
#include "mx/core/elements/PartwisePart.h"

#include <list>
#include <sstream>
#include <algorithm>

namespace mx
{
    namespace impl
    {
        
        struct GroupEnd
        {
            core::PartGroupPtr partGroup;
            int absoluteIndex;
            int groupIndex;
            
            // if the group is a 'start' then it is attached to the following part
            // if the group is a 'stop' then it is attached to the preceding part
            int attachedPartIndex;
        };
        
        struct GroupHelper
        {
            GroupEnd start;
            GroupEnd stop;
            std::vector<GroupHelper> subGroups;
        };
        
        struct PartHelper
        {
            core::ScorePartPtr scorePart;
            int absoluteIndex;
            int partIndex;
        };
        
        void listPartsAndGroups( const core::PartList& inData, std::vector<GroupEnd>& outGroupEnds, std::vector<PartHelper>& outParts );
        std::vector<GroupHelper> aggregateGroups( const std::vector<GroupEnd>& inGroupEnds );
        std::string extractDisplayText( const core::DisplayTextOrAccidentalTextSet& items );
        
        void listPartsAndGroups( const core::PartList& inData, std::vector<GroupEnd>& outGroupEnds, std::vector<PartHelper>& outParts )
        {
            int absoluteIndex = 0;
            int groupIndex = 0;
            int partIndex = 0;
            for( const auto& grp : inData.getPartGroupSet() )
            {
                auto initGrpHelper = GroupEnd{};
                initGrpHelper.partGroup = grp;
                initGrpHelper.absoluteIndex = absoluteIndex;
                initGrpHelper.groupIndex = groupIndex;
                initGrpHelper.attachedPartIndex = ( grp->getAttributes()->type == core::StartStop::start ) ? partIndex : ( partIndex - 1 );
                outGroupEnds.emplace_back( std::move( initGrpHelper ) );
                ++groupIndex;
                ++absoluteIndex;
            }
            
            auto initPartHelper = PartHelper{};
            initPartHelper.scorePart = inData.getScorePart();
            initPartHelper.absoluteIndex = absoluteIndex;
            initPartHelper.partIndex = partIndex;
            outParts.emplace_back( std::move( initPartHelper ) );
            ++partIndex;
            ++absoluteIndex;
            
            for( const auto& c : inData.getPartGroupOrScorePartSet() )
            {
                if( c->getChoice() == core::PartGroupOrScorePart::Choice::partGroup )
                {
                    const auto groupPtr = c->getPartGroup();
                    auto grpHelper = GroupEnd{};
                    grpHelper.partGroup = groupPtr;
                    grpHelper.absoluteIndex = absoluteIndex;
                    grpHelper.groupIndex = groupIndex;
                    grpHelper.attachedPartIndex = ( groupPtr->getAttributes()->type == core::StartStop::start ) ? partIndex : ( partIndex - 1 );
                    outGroupEnds.emplace_back( std::move( grpHelper ) );
                    ++groupIndex;
                    ++absoluteIndex;
                }
                else
                {
                    auto partHelper = PartHelper{};
                    partHelper.scorePart = c->getScorePart();
                    partHelper.absoluteIndex = absoluteIndex;
                    partHelper.partIndex = partIndex;
                    outParts.emplace_back( std::move( partHelper ) );
                    ++partIndex;
                    ++absoluteIndex;
                }
            }
        }
        
        
        std::vector<GroupHelper> aggregateGroups( const std::vector<GroupEnd>& inGroupEnds )
        {
            std::vector<GroupHelper> outHelpers;
            std::list<GroupHelper> helperStack;
            for( const auto& groupEnd : inGroupEnds )
            {
                const auto& attr = *groupEnd.partGroup->getAttributes();
                const std::string mxNum = attr.hasNumber ? "" : attr.number.getValue();
                
                if( attr.type == core::StartStop::start )
                {
                    GroupHelper newHelper;
                    newHelper.start = groupEnd;
                    helperStack.push_front( std::move( newHelper ) );
                }
                else if ( attr.type == core::StartStop::stop && !helperStack.empty() )
                {
                    auto existingHelperIt = helperStack.end();
                    
                    if( mxNum.empty() )
                    {
                        existingHelperIt = helperStack.begin();

                    }
                    else
                    {
                        auto predicate = [&mxNum] ( const GroupHelper& gi )
                        {
                            return gi.stop.partGroup->getAttributes()->hasNumber && ( gi.stop.partGroup->getAttributes()->number.getValue() == mxNum );
                        };
                        existingHelperIt = std::find_if( helperStack.begin(), helperStack.end(), predicate );
                    }
                    
                    if( existingHelperIt == helperStack.cend() )
                    {
                        continue;
                    }
                    
                    existingHelperIt->stop = groupEnd;
                    outHelpers.emplace_back( *existingHelperIt );
                    helperStack.erase( existingHelperIt );
                }
            }
            
            auto comparison = []( const GroupHelper& a, const GroupHelper& b )
            {
                
                if( a.start.attachedPartIndex < b.start.attachedPartIndex ) { return true; }
                if( a.start.attachedPartIndex > b.start.attachedPartIndex ) { return false; }

                // a.start.attachedPartIndex == b.start.attachedPartIndex
                if( a.stop.attachedPartIndex < b.stop.attachedPartIndex ) { return false; }
                if( a.stop.attachedPartIndex > b.stop.attachedPartIndex ) { return true; }

                // a.stop.attachedPartIndex == b.stop.attachedPartIndex
                if( a.start.partGroup->getAttributes()->hasNumber &&
                    b.start.partGroup->getAttributes()->hasNumber &&
                    ( a.start.partGroup->getAttributes()->number.getValue() < b.start.partGroup->getAttributes()->number.getValue() ) )
                {
                    return true;
                }

                return false;
            };
            
            std::sort( outHelpers.begin(), outHelpers.end(), comparison );
            
            return outHelpers;
        }
        
        std::string extractDisplayText( const core::DisplayTextOrAccidentalTextSet& items )
        {
            std::stringstream ss;
            for( const auto& c : items )
            {
                if( c->getChoice() == core::DisplayTextOrAccidentalText::Choice::displayText )
                {
                    ss << c->getDisplayText()->getValue().getValue();
                }
                else if( c->getChoice() == core::DisplayTextOrAccidentalText::Choice::accidentalText )
                {
                    if( c->getAccidentalText()->getValue() == core::AccidentalValue::flat )
                    {
                        ss << "b"; // TODO - support accidental text correctly
                    }
                    else if( c->getAccidentalText()->getValue() == core::AccidentalValue::flat )
                    {
                        ss << "#";
                    }
                }
            }
            return ss.str();
        }
        
        
        void createMxFromPartData( const api::ScoreData& inScoreData, core::ScorePartwise& outMx )
        {
            for( const auto& p : inScoreData.parts )
            {
                // TODO - everything
            }
        }
        
        
        void createPartDataFromMx( const core::ScorePartwise& inMx, api::ScoreData& outScoreData )
        {
            std::list<api::PartGroupData> groupStack;
            int index = 0;
            const auto& header = *inMx.getScoreHeaderGroup();
            const auto& partDeclarations = *header.getPartList();
            
            std::vector<GroupEnd> groupEnds;
            std::vector<PartHelper> parts;
            listPartsAndGroups( *inMx.getScoreHeaderGroup()->getPartList(), groupEnds, parts );
            auto groups = aggregateGroups( groupEnds );
            
            for( const auto& g : groups )
            {
                const auto& mxPartGroup = *g.start.partGroup;
                auto partGroupData = api::PartGroupData{};
                partGroupData.firstPartIndex = g.start.attachedPartIndex;
                partGroupData.lastPartIndex = g.stop.attachedPartIndex;
                partGroupData.name = mxPartGroup.getHasGroupName() ? mxPartGroup.getGroupName()->getValue().getValue() : "";
                
                if( mxPartGroup.getHasGroupNameDisplay() )
                {
                    partGroupData.displayName = extractDisplayText( mxPartGroup.getGroupNameDisplay()->getDisplayTextOrAccidentalTextSet() );
                }
                
                if( mxPartGroup.getHasGroupAbbreviationDisplay() )
                {
                    partGroupData.displayAbbreviation = extractDisplayText( mxPartGroup.getGroupAbbreviationDisplay()->getDisplayTextOrAccidentalTextSet() );
                }
                
                partGroupData.name = mxPartGroup.getHasGroupName() ? mxPartGroup.getGroupName()->getValue().getValue() : "";
                partGroupData.abbreviation = mxPartGroup.getHasGroupAbbreviation() ? mxPartGroup.getGroupAbbreviation()->getValue().getValue() : "";
                // TODO - group-symbol
                // TODO - group barline
                // TODO - group-bracket
                // TODO - group-time
                outScoreData.partGroups.emplace_back( std::move( partGroupData ) );
            }
            
            for( const auto& partHelper : parts )
            {
                const auto& mxPart = *partHelper.scorePart;
                const auto& attr = *mxPart.getAttributes();
                auto partData = api::PartData{};
                partData.uniqueId = attr.id.getValue();
                partData.name = mxPart.getPartName()->getValue().getValue();
                
                if( mxPart.getHasPartNameDisplay() )
                {
                    partData.displayName = extractDisplayText( mxPart.getPartNameDisplay()->getDisplayTextOrAccidentalTextSet() );
                }
                
                if( mxPart.getHasPartAbbreviation() )
                {
                    partData.abbreviation = mxPart.getPartAbbreviation()->getValue().getValue();
                }
                
                if( mxPart.getHasPartAbbreviationDisplay() )
                {
                    partData.displayAbbreviation = extractDisplayText( mxPart.getPartNameDisplay()->getDisplayTextOrAccidentalTextSet() );
                }
                
                // TODO - InstrumentName, MIDI stuff, Virtual Library stuff, etc
                const auto& mxPartwiseParts = inMx.getPartwisePartSet();
                for( const auto& mxMusicContainer : mxPartwiseParts )
                {
                    auto iter = api::findPart( outScoreData.parts, mxMusicContainer->getAttributes()->id.getValue() );
                    
                    if( iter == outScoreData.parts.end() )
                    {
                        continue;
                    }
                    
                    auto& partData = *iter;
                    createStavesFromMx( mxMusicContainer->getPartwiseMeasureSet(), partData.staves );
                }
                
                outScoreData.parts.emplace_back( std::move( partData ) );
                
            }
        }
    }
}
