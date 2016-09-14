// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#include "mx/impl/ScoreFunctions.h"
#include "mx/core/elements/Creator.h"
#include "mx/core/elements/MovementNumber.h"
#include "mx/core/elements/MovementTitle.h"
#include "mx/core/elements/Rights.h"
#include "mx/core/elements/ScoreHeaderGroup.h"
#include "mx/core/elements/Work.h"
#include "mx/core/elements/WorkNumber.h"
#include "mx/core/elements/WorkTitle.h"
#include "mx/impl/EncodingFunctions.h"
#include "mx/impl/LayoutFunctions.h"
#include "mx/impl/PageTextFunctions.h"
#include "mx/impl/PartFunctions.h"
#include "mx/impl/TimeFunctions.h"

using namespace mx::core;
using namespace mx::api;

namespace mx
{
    namespace impl
    {
        DocumentPtr createDocument( const api::ScoreData& score )
        {
            auto mxDoc = makeDocument();
            auto mxScore = mxDoc->getScorePartwise();
            auto header = mxScore->getScoreHeaderGroup();
            
            header->setHasWork( !score.workTitle.empty() || !score.workNumber.empty() );
            header->getWork()->setHasWorkTitle( !score.workTitle.empty() );
            header->getWork()->getWorkTitle()->setValue( core::XsString( score.workTitle ) );
            header->getWork()->setHasWorkNumber( !score.workNumber.empty() );
            header->getWork()->getWorkNumber()->setValue( core::XsString( score.workNumber ) );

            header->setHasMovementTitle( !score.movementTitle.empty() );
            header->getMovementTitle()->setValue( core::XsString( score.movementTitle ) );
            
            header->setHasMovementNumber( !score.movementNumber.empty() );
            header->getMovementNumber()->setValue( core::XsString( score.movementNumber ) );
            
            if( !score.composer.empty() )
            {
                auto composer = core::makeCreator();
                composer->getAttributes()->hasType = true;
                composer->getAttributes()->type = core::XsToken( "composer" );
                composer->setValue( core::XsString( score.composer ) );
                header->setHasIdentification( true );
                header->getIdentification()->addCreator( composer );
            }
            
            if( !score.lyricist.empty() )
            {
                auto lyricist = core::makeCreator();
                lyricist->getAttributes()->hasType = true;
                lyricist->getAttributes()->type = core::XsToken( "lyricist" );
                lyricist->setValue( core::XsString( score.lyricist ) );
                header->setHasIdentification( true );
                header->getIdentification()->addCreator( lyricist );
            }
            
            if( !score.copyright.empty() )
            {
                auto copyright = core::makeRights();
                copyright->getAttributes()->hasType = true;
                copyright->getAttributes()->type = core::XsToken( "copyright" );
                copyright->setValue( core::XsString( score.copyright ) );
                header->setHasIdentification( true );
                header->getIdentification()->addRights( copyright );
            }
            
            createEncoding( score.encoding, *header );
            addLayoutData( score.layout, *header );
            createPageTextItems( score.pageTextItems, *header );
            createMxFromPartData( score, *mxScore );

            if( score.musicXmlType == "timewise" )
            {
                mxDoc->convertContents();
            }
            return mxDoc;
        }


        api::ScoreData createScore( const core::Document& document )
        {
            if( document.getChoice() == DocumentChoice::timewise )
            {
                MX_THROW( "this function only supports partwise documents" );
            }
            
            auto scorePartwise = document.getScorePartwise();
            auto header = scorePartwise->getScoreHeaderGroup();

            api::ScoreData score;
            impl::TimeFunctions timeFunc;
            score.ticksPerQuarter = timeFunc.findMaxDivisionsPerQuarter( *scorePartwise );

            if( header->getHasWork() && header->getWork()->getHasWorkTitle() )
            {
                score.workTitle = header->getWork()->getWorkTitle()->getValue().getValue();
            }
            
            if( header->getHasWork() && header->getWork()->getHasWorkNumber() )
            {
                score.workNumber = header->getWork()->getWorkNumber()->getValue().getValue();
            }
            
            if( header->getHasMovementTitle() )
            {
                score.movementTitle = header->getMovementTitle()->getValue().getValue();
            }
            
            if( header->getHasMovementNumber() )
            {
                score.movementNumber = header->getMovementNumber()->getValue().getValue();
            }
            
            bool isComposerFound = false;
            bool isCopyrightFound = false;
            
            if( header->getHasIdentification() )
            {
                
                for( auto i : header->getIdentification()->getCreatorSet() )
                {
                    auto a = i->getAttributes();
                    if( a->type.getValue() == "composer" && !isComposerFound )
                    {
                        score.composer = i->getValue().getValue();
                        isComposerFound = true;
                    }
                    
                    if( !isComposerFound && !a->hasType )
                    {
                        score.composer = i->getValue().getValue();
                    }
                    
                    if( a->type.getValue() == "lyricist" )
                    {
                        score.lyricist = i->getValue().getValue();
                        isComposerFound = true;
                    }
                }
                
                for( auto r : header->getIdentification()->getRightsSet() )
                {
                    auto a = r->getAttributes();
                    if( a->hasType && a->type.getValue() == "copyright" && !isCopyrightFound )
                    {
                        score.copyright = r->getValue().getValue();
                        isCopyrightFound = true;
                    }
                    
                    if( !isCopyrightFound && !a->hasType )
                    {
                        score.copyright = r->getValue().getValue();
                    }
                }
            }
            
            if( header->getHasIdentification() && header->getIdentification()->getHasEncoding() )
            {
                score.encoding = createEncoding( *header->getIdentification()->getEncoding() );
            }
            
            if( header->getHasDefaults() )
            {
                score.layout = createLayout( *header );
            }
            
            createPageTextItems( *header, score.pageTextItems );
            
            createPartDataFromMx( *document.getScorePartwise(), score );
            return score;
        }
    }
}
