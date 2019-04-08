// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"

namespace mx
{
    namespace api
    {
        enum class AppearanceType
        {
            /// The line-width-type defines what type of line is being defined
            /// in a line-width element. Values include beam, bracket, dashes,
            /// enclosure, ending, extend, heavy barline, leger, light barline,
            /// octave shift, pedal, slur middle, slur tip, staff, stem, tie middle,
            /// tie tip, tuplet bracket, and wedge. This is left as a string so that
            /// other application-specific types can be defined, but it is made a
            /// separate type so that it can be redefined more strictly.
            LineWidth,

            /// The note-size-type type indicates the type of note being defined
            /// by a note-size element. The grace type is used for notes of cue
            /// size that that include a grace element. The cue type is used for
            /// all other notes with cue size, whether defined explicitly or
            /// implicitly via a cue element. The large type is used for notes of
            /// large size.
            NoteSize,

            /// The distance-type defines what type of distance is being defined
            /// in a distance element. Values include beam and hyphen. This is
            /// left as a string so that other application-specific types can be
            /// defined, but it is made a separate type so that it can be redefined
            /// more strictly.
            Distance,

            /// Anything goes
            OtherAppearance,
        };

        class AppearanceData
        {
        public:
            AppearanceData();

        public:
            AppearanceType appearanceType;
            std::string appearanceSubType;
            long double value;
        };

        MXAPI_EQUALS_BEGIN( AppearanceData )
        MXAPI_EQUALS_MEMBER( appearanceType )
        MXAPI_EQUALS_MEMBER( appearanceSubType )
        MXAPI_EQUALS_MEMBER( value )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( AppearanceData );
        
    }
}
