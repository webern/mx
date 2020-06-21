// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/SoundAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
    namespace core
    {
        SoundAttributes::SoundAttributes()
        :tempo()
        ,dynamics()
        ,dacapo( YesNo::no )
        ,segno()
        ,dalsegno()
        ,coda()
        ,tocoda()
        ,divisions()
        ,forwardRepeat( YesNo::no )
        ,fine()
        ,timeOnly()
        ,pizzicato( YesNo::no )
        ,pan()
        ,elevation()
        ,damperPedal()
        ,softPedal()
        ,sostenutoPedal()
        ,hasTempo( false )
        ,hasDynamics( false )
        ,hasDacapo( false )
        ,hasSegno( false )
        ,hasDalsegno( false )
        ,hasCoda( false )
        ,hasTocoda( false )
        ,hasDivisions( false )
        ,hasForwardRepeat( false )
        ,hasFine( false )
        ,hasTimeOnly( false )
        ,hasPizzicato( false )
        ,hasPan( false )
        ,hasElevation( false )
        ,hasDamperPedal( false )
        ,hasSoftPedal( false )
        ,hasSostenutoPedal( false )
        {}


        bool SoundAttributes::hasValues() const
        {
            return hasTempo ||
            hasDynamics ||
            hasDacapo ||
            hasSegno ||
            hasDalsegno ||
            hasCoda ||
            hasTocoda ||
            hasDivisions ||
            hasForwardRepeat ||
            hasFine ||
            hasTimeOnly ||
            hasPizzicato ||
            hasPan ||
            hasElevation ||
            hasDamperPedal ||
            hasSoftPedal ||
            hasSostenutoPedal;
        }


        std::ostream& SoundAttributes::toStream( std::ostream& os ) const
        {
            if ( hasValues() )
            {
                streamAttribute( os, tempo, "tempo", hasTempo );
                streamAttribute( os, dynamics, "dynamics", hasDynamics );
                streamAttribute( os, dacapo, "dacapo", hasDacapo );
                streamAttribute( os, segno, "segno", hasSegno );
                streamAttribute( os, dalsegno, "dalsegno", hasDalsegno );
                streamAttribute( os, coda, "coda", hasCoda );
                streamAttribute( os, tocoda, "tocoda", hasTocoda );
                streamAttribute( os, divisions, "divisions", hasDivisions );
                streamAttribute( os, forwardRepeat, "forward-repeat", hasForwardRepeat );
                streamAttribute( os, fine, "fine", hasFine );
                streamAttribute( os, timeOnly, "time-only", hasTimeOnly );
                streamAttribute( os, pizzicato, "pizzicato", hasPizzicato );
                streamAttribute( os, pan, "pan", hasPan );
                streamAttribute( os, elevation, "elevation", hasElevation );
                streamAttribute( os, damperPedal, "damper-pedal", hasDamperPedal );
                streamAttribute( os, softPedal, "soft-pedal", hasSoftPedal );
                streamAttribute( os, sostenutoPedal, "sostenuto-pedal", hasSostenutoPedal );
            }
            return os;
        }


        bool SoundAttributes::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement )
        {
            const char* const className = "SoundAttributes";
            bool isSuccess = true;
        
            auto it = xelement.attributesBegin();
            auto endIter = xelement.attributesEnd();
        
            for( ; it != endIter; ++it )
            {
                if( parseAttribute( message, it, className, isSuccess, tempo, hasTempo, "tempo" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, dynamics, hasDynamics, "dynamics" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, dacapo, hasDacapo, "dacapo", &parseYesNo ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, segno, hasSegno, "segno" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, dalsegno, hasDalsegno, "dalsegno" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, coda, hasCoda, "coda" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, tocoda, hasTocoda, "tocoda" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, divisions, hasDivisions, "divisions" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, forwardRepeat, hasForwardRepeat, "forward-repeat", &parseYesNo ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, fine, hasFine, "fine" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, timeOnly, hasTimeOnly, "time-only" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, pizzicato, hasPizzicato, "pizzicato", &parseYesNo ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, pan, hasPan, "pan" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, elevation, hasElevation, "elevation" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, damperPedal, hasDamperPedal, "damper-pedal" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, softPedal, hasSoftPedal, "soft-pedal" ) ) { continue; }
                if( parseAttribute( message, it, className, isSuccess, sostenutoPedal, hasSostenutoPedal, "sostenuto-pedal" ) ) { continue; }
            }
        
        
            MX_RETURN_IS_SUCCESS;
        }

    }
}
