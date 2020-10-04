// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"

namespace mx
{
    namespace api
    {
        /// TransposeData is used to instruments for whom the music has been transposed. The
        /// transposition is specified by naming the directional diatonic interval between the
        /// written pitch and the sounding pitch. For example when C4 is written for Bb Clarinet,
        /// the sounding pitch is Bb3 which is a major second lower. The transposition for Bb
        /// Clarinet is thus 'down a major second'. There may be cases where such a transposition
        /// would produce a painful key signature, and engravers may choose to notate the Bb
        /// Clarinet in A# instead. In that case the interval is 'down a diminished third'.
        ///
        /// In order to distinguish between intervals that are enharmonically equivalent (having
        /// the same number of semitones), two values are needed. TransposeData encodes the
        /// diatonic interval by providing the number of chromatic steps and the number of diatonic
        /// steps where the number of 'diatonic' steps is the number of steps via letter names (or
        /// piano white notes).
        ///
        /// Some examples may help. Clarinet in Bb transposes down a major second, which is two
        /// semitones, so `chromatic = -2`. In order to distinguish this from, say A#, or Cbb, we
        /// need additional information, so we specify `diatonic = -1`. That is, we want the letter
        /// to be B, and B is one letter lower than C.
        ///
        /// Here is a table of some common transpositions and their TransposeData equivalents.
        /// |---------------------|--------------------|----------|---------|
        /// | Name                | Interval           | Chromatic| Diatonic|
        /// |---------------------|--------------------|----------|---------|
        /// | Glockenspiel        | Up two Octaves     | 24       | 14      |
        /// | Piccolo             | Up an Octave       | 12       | 7       |
        /// | Clarient in Bb      | Down a Major Second| -2       | -1      |
        /// | French Horn in F    | Down a Fifth       | -7       | -4      |
        /// | Alto Saxophone in Eb| Down a Major Sixth | -9       | -5      |
        /// | Bass Clarinet in Bb | Down a Major Ninth | -14      | -8      |
        /// |---------------------|--------------------|----------|---------|
        ///
        /// Note that MusicXML encodes this slightly differently. In MusicXML required (but not
        /// enforced) that chromatic should be >= -11 and <= 11, and that diatonic should be
        /// <= -6 and >= 6. An additional `<octave-offset>` element is used for intervals beyond
        /// that range. Additionally, the `<diatonic>` element is optional in MusicXML, but is
        /// required in TransposeData.
        class TransposeData
        {
        public:
            TransposeData()
            : TransposeData{ 0, 0 }
            {

            }

            TransposeData( int inChromatic, int inHarmonic )
            : chromatic{ inChromatic }
            , diatonic{ inHarmonic }
            {

            }

            /// The number of semitones in the interval.
            int chromatic;

            /// The number of diatonic steps in the interval
            int diatonic;

            /// If both chromatic and diatonic are zero, then TransposeData is unused (i.e. it need
            /// not be encoded in the MusicXML output).
            inline bool isUsed() const { return chromatic != 0 || diatonic != 0; }
        };

        MXAPI_EQUALS_BEGIN( TransposeData )
            MXAPI_EQUALS_MEMBER( chromatic )
            MXAPI_EQUALS_MEMBER( diatonic )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( TransposeData );
    }
}
