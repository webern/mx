// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/api/SoundID.h"
#include "mx/core/Enums.h"
#include "mx/impl/Converter.h"

namespace mx
{
    namespace api
    {
        std::string SoundIDToString( SoundID inSoundID )
        {
            if( inSoundID == SoundID::unspecified )
            {
                return "unspecified";
            }
            else if( inSoundID == SoundID::errorBadString )
            {
                return "error.bad.string";
            }

            impl::Converter c;
            return core::PlaybackSoundToString( c.convert( inSoundID ) );
        }


        SoundID SoundIDFromString( const std::string& inString )
        {
            SoundID result = SoundID::keyboardPiano;

            try
            {
                const auto p = core::PlaybackSoundFromString( inString );
                impl::Converter c;
                result = c.convert( p );
            }
            catch (...)
            {
                result = SoundID::errorBadString;
            }

            return result;
        }
    }
}
