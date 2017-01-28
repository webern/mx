// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/InvertedMordentAttributes.h"
#include "mx/core/FromXElement.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        InvertedMordentAttributes::InvertedMordentAttributes()
        :defaultX()
        ,defaultY()
        ,relativeX()
        ,relativeY()
        ,fontFamily()
        ,fontStyle( FontStyle::normal )
        ,fontSize( CssFontSize::medium )
        ,fontWeight( FontWeight::normal )
        ,color()
        ,placement( AboveBelow::below )
        ,startNote( StartNote::main )
        ,trillStep( TrillStep::half )
        ,twoNoteTurn( TwoNoteTurn::none )
        ,accelerate( YesNo::no )
        ,beats()
        ,secondBeat()
        ,lastBeat()
        ,long_( YesNo::no )
        ,approach( AboveBelow::below )
        ,departure( AboveBelow::below )
        ,hasDefaultX( false )
        ,hasDefaultY( false )
        ,hasRelativeX( false )
        ,hasRelativeY( false )
        ,hasFontFamily( false )
        ,hasFontStyle( false )
        ,hasFontSize( false )
        ,hasFontWeight( false )
        ,hasColor( false )
        ,hasPlacement( false )
        ,hasStartNote( false )
        ,hasTrillStep( false )
        ,hasTwoNoteTurn( false )
        ,hasAccelerate( false )
        ,hasBeats( false )
        ,hasSecondBeat( false )
        ,hasLastBeat( false )
        ,hasLong( false )
        ,hasApproach( false )
        ,hasDeparture( false )
        {}


        bool InvertedMordentAttributes::hasValues() const
        {
            return hasDefaultX ||
            hasDefaultY ||
            hasRelativeX ||
            hasRelativeY ||
            hasFontFamily ||
            hasFontStyle ||
            hasFontSize ||
            hasFontWeight ||
            hasColor ||
            hasPlacement ||
            hasStartNote ||
            hasTrillStep ||
            hasTwoNoteTurn ||
            hasAccelerate ||
            hasBeats ||
            hasSecondBeat ||
            hasLastBeat ||
            hasLong ||
            hasApproach ||
            hasDeparture;
        }


        std::ostream& InvertedMordentAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, defaultX, "default-x", hasDefaultX );
                streamAttribute( os, defaultY, "default-y", hasDefaultY );
                streamAttribute( os, relativeX, "relative-x", hasRelativeX );
                streamAttribute( os, relativeY, "relative-y", hasRelativeY );
                streamAttribute( os, fontFamily, "font-family", hasFontFamily );
                streamAttribute( os, fontStyle, "font-style", hasFontStyle );
                streamAttribute( os, fontSize, "font-size", hasFontSize );
                streamAttribute( os, fontWeight, "font-weight", hasFontWeight );
                streamAttribute( os, color, "color", hasColor );
                streamAttribute( os, placement, "placement", hasPlacement );
                streamAttribute( os, startNote, "start-note", hasStartNote );
                streamAttribute( os, trillStep, "trill-step", hasTrillStep );
                streamAttribute( os, twoNoteTurn, "two-note-turn", hasTwoNoteTurn );
                streamAttribute( os, accelerate, "accelerate", hasAccelerate );
                streamAttribute( os, beats, "beats", hasBeats );
                streamAttribute( os, secondBeat, "second-beat", hasSecondBeat );
                streamAttribute( os, lastBeat, "last-beat", hasLastBeat );
                streamAttribute( os, long_, "long", hasLong );
                streamAttribute( os, approach, "approach", hasApproach );
                streamAttribute( os, departure, "departure", hasDeparture );
            }
            return os;
        }


        bool InvertedMordentAttributes::fromXElement( std::ostream& message, xml::XElement& xelement )
        {
            const char* const className = "InvertedMordentAttributes";
            bool isSuccess = true;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, defaultX, hasDefaultX, "default-x" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, defaultY, hasDefaultY, "default-y" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, relativeX, hasRelativeX, "relative-x" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, relativeY, hasRelativeY, "relative-y" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, fontFamily, hasFontFamily, "font-family" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, fontStyle, hasFontStyle, "font-style", &parseFontStyle ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, fontSize, hasFontSize, "font-size" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, fontWeight, hasFontWeight, "font-weight", &parseFontWeight ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, color, hasColor, "color" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, placement, hasPlacement, "placement", &parseAboveBelow ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, startNote, hasStartNote, "start-note", &parseStartNote ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, trillStep, hasTrillStep, "trill-step", &parseTrillStep ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, twoNoteTurn, hasTwoNoteTurn, "two-note-turn", &parseTwoNoteTurn ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, accelerate, hasAccelerate, "accelerate", &parseYesNo ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, beats, hasBeats, "beats" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, secondBeat, hasSecondBeat, "second-beat" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, lastBeat, hasLastBeat, "last-beat" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, long_, hasLong , "long", &parseYesNo ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, approach, hasApproach, "approach", &parseAboveBelow ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, departure, hasDeparture, "departure", &parseAboveBelow ) ) { continue; }
            }
        
        
            MX_RETURN_IS_SUCCESS;
        }

    }
}
