// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/EnumsBuiltin.h"

#include <iostream>
#include <optional>
#include <string>

namespace mx
{
    namespace core
    {
        /// AboveBelow /////////////////////////////////////////////////////////////////////////////
        ///
        /// The above-below type is used to indicate whether one element appears above or below
        /// another element.
        ///
        enum class AboveBelow
        {
            above = 0,
            below = 1
        };

        AboveBelow parseAboveBelow( const std::string& value );
        std::optional<AboveBelow> tryParseAboveBelow( const std::string& value );
        std::string toString( const AboveBelow value );
        std::ostream& toStream( std::ostream& os, const AboveBelow value );
        std::ostream& operator<<( std::ostream& os, const AboveBelow value );

        /// AccidentalValue ////////////////////////////////////////////////////////////////////////
        ///
        /// The accidental-value type represents notated accidentals supported by MusicXML. In the
        /// MusicXML 2.0 DTD this was a string with values that could be included. The XSD
        /// strengthens the data typing to an enumerated list. The quarter- and three-quarters-
        /// accidentals are Tartini-style quarter-tone accidentals. The -down and -up accidentals
        /// are quarter-tone accidentals that include arrows pointing down or up. The slash-
        /// accidentals are used in Turkish classical music. The numbered sharp and flat accidentals
        /// are superscripted versions of the accidental signs, used in Turkish folk music. The sori
        /// and koron accidentals are microtonal sharp and flat accidentals used in Iranian and
        /// Persian music.
        ///
        enum class AccidentalValue
        {
            sharp = 0,
            natural = 1,
            flat = 2,
            doubleSharp = 3,
            sharpSharp = 4,
            flatFlat = 5,
            naturalSharp = 6,
            naturalFlat = 7,
            quarterFlat = 8,
            quarterSharp = 9,
            threeQuartersFlat = 10,
            threeQuartersSharp = 11,
            sharpDown = 12,
            sharpUp = 13,
            naturalDown = 14,
            naturalUp = 15,
            flatDown = 16,
            flatUp = 17,
            tripleSharp = 18,
            tripleFlat = 19,
            slashQuarterSharp = 20,
            slashSharp = 21,
            slashFlat = 22,
            doubleSlashFlat = 23,
            sharp1 = 24,
            sharp2 = 25,
            sharp3 = 26,
            sharp5 = 27,
            flat1 = 28,
            flat2 = 29,
            flat3 = 30,
            flat4 = 31,
            sori = 32,
            koron = 33
        };

        AccidentalValue parseAccidentalValue( const std::string& value );
        std::optional<AccidentalValue> tryParseAccidentalValue( const std::string& value );
        std::string toString( const AccidentalValue value );
        std::ostream& toStream( std::ostream& os, const AccidentalValue value );
        std::ostream& operator<<( std::ostream& os, const AccidentalValue value );

        /// ArrowDirectionEnum /////////////////////////////////////////////////////////////////////
        ///
        /// The arrow-direction type represents the direction in which an arrow points, using
        /// Unicode arrow terminology.
        ///
        enum class ArrowDirectionEnum
        {
            left = 0,
            up = 1,
            right = 2,
            down = 3,
            northwest = 4,
            northeast = 5,
            southeast = 6,
            southwest = 7,
            leftRight = 8,
            upDown = 9,
            northwestSoutheast = 10,
            northeastSouthwest = 11,
            other = 12
        };

        ArrowDirectionEnum parseArrowDirectionEnum( const std::string& value );
        std::optional<ArrowDirectionEnum> tryParseArrowDirectionEnum( const std::string& value );
        std::string toString( const ArrowDirectionEnum value );
        std::ostream& toStream( std::ostream& os, const ArrowDirectionEnum value );
        std::ostream& operator<<( std::ostream& os, const ArrowDirectionEnum value );

        /// ArrowStyleEnum /////////////////////////////////////////////////////////////////////////
        ///
        /// The arrow-style type represents the style of an arrow, using Unicode arrow terminology.
        /// Filled and hollow arrows indicate polygonal single arrows. Paired arrows are duplicate
        /// single arrows in the same direction. Combined arrows apply to double direction arrows
        /// like left right, indicating that an arrow in one direction should be combined with an
        /// arrow in the other direction.
        ///
        enum class ArrowStyleEnum
        {
            single = 0,
            double_ = 1,
            filled = 2,
            hollow = 3,
            paired = 4,
            combined = 5,
            other = 6
        };

        ArrowStyleEnum parseArrowStyleEnum( const std::string& value );
        std::optional<ArrowStyleEnum> tryParseArrowStyleEnum( const std::string& value );
        std::string toString( const ArrowStyleEnum value );
        std::ostream& toStream( std::ostream& os, const ArrowStyleEnum value );
        std::ostream& operator<<( std::ostream& os, const ArrowStyleEnum value );

        /// BackwardForward ////////////////////////////////////////////////////////////////////////
        ///
        /// The backward-forward type is used to specify repeat directions. The start of the repeat
        /// has a forward direction while the end of the repeat has a backward direction.
        ///
        enum class BackwardForward
        {
            backward = 0,
            forward = 1
        };

        BackwardForward parseBackwardForward( const std::string& value );
        std::optional<BackwardForward> tryParseBackwardForward( const std::string& value );
        std::string toString( const BackwardForward value );
        std::ostream& toStream( std::ostream& os, const BackwardForward value );
        std::ostream& operator<<( std::ostream& os, const BackwardForward value );

        /// BarStyleEnum ///////////////////////////////////////////////////////////////////////////
        ///
        /// The bar-style type represents barline style information. Choices are regular, dotted,
        /// dashed, heavy, light-light, light-heavy, heavy-light, heavy-heavy, tick (a short stroke
        /// through the top line), short (a partial barline between the 2nd and 4th lines), and
        /// none.
        ///
        enum class BarStyleEnum
        {
            regular = 0,
            dotted = 1,
            dashed = 2,
            heavy = 3,
            lightLight = 4,
            lightHeavy = 5,
            heavyLight = 6,
            heavyHeavy = 7,
            tick = 8,
            short_ = 9,
            none = 10
        };

        BarStyleEnum parseBarStyleEnum( const std::string& value );
        std::optional<BarStyleEnum> tryParseBarStyleEnum( const std::string& value );
        std::string toString( const BarStyleEnum value );
        std::ostream& toStream( std::ostream& os, const BarStyleEnum value );
        std::ostream& operator<<( std::ostream& os, const BarStyleEnum value );

        /// BeamValue //////////////////////////////////////////////////////////////////////////////
        ///
        /// The beam-value type represents the type of beam associated with each of 8 beam levels
        /// (up to 1024th notes) available for each note.
        ///
        enum class BeamValue
        {
            begin = 0,
            continue_ = 1,
            end = 2,
            forwardHook = 3,
            backwardHook = 4
        };

        BeamValue parseBeamValue( const std::string& value );
        std::optional<BeamValue> tryParseBeamValue( const std::string& value );
        std::string toString( const BeamValue value );
        std::ostream& toStream( std::ostream& os, const BeamValue value );
        std::ostream& operator<<( std::ostream& os, const BeamValue value );

        /// BeaterValue ////////////////////////////////////////////////////////////////////////////
        ///
        /// The beater-value type represents pictograms for beaters, mallets, and sticks that do not
        /// have different materials represented in the pictogram. The finger and hammer values are
        /// in addition to Stone's list.
        ///
        enum class BeaterValue
        {
            bow = 0,
            chimeHammer = 1,
            coin = 2,
            finger = 3,
            fingernail = 4,
            fist = 5,
            guiroScraper = 6,
            hammer = 7,
            hand = 8,
            jazzStick = 9,
            knittingNeedle = 10,
            metalHammer = 11,
            snareStick = 12,
            spoonMallet = 13,
            triangleBeater = 14,
            triangleBeaterPlain = 15,
            wireBrush = 16
        };

        BeaterValue parseBeaterValue( const std::string& value );
        std::optional<BeaterValue> tryParseBeaterValue( const std::string& value );
        std::string toString( const BeaterValue value );
        std::ostream& toStream( std::ostream& os, const BeaterValue value );
        std::ostream& operator<<( std::ostream& os, const BeaterValue value );

        /// BreathMarkValue ////////////////////////////////////////////////////////////////////////
        ///
        /// The breath-mark-value type represents the symbol used for a breath mark.
        ///
        enum class BreathMarkValue
        {
            emptystring = 0,
            comma = 1,
            tick = 2
        };

        BreathMarkValue parseBreathMarkValue( const std::string& value );
        std::optional<BreathMarkValue> tryParseBreathMarkValue( const std::string& value );
        std::string toString( const BreathMarkValue value );
        std::ostream& toStream( std::ostream& os, const BreathMarkValue value );
        std::ostream& operator<<( std::ostream& os, const BreathMarkValue value );

        /// CancelLocation /////////////////////////////////////////////////////////////////////////
        ///
        /// The cancel-location type is used to indicate where a key signature cancellation appears
        /// relative to a new key signature: to the left, to the right, or before the barline and to
        /// the left. It is left by default. For mid-measure key elements, a cancel-location of
        /// before-barline should be treated like a cancel-location of left.
        ///
        enum class CancelLocation
        {
            left = 0,
            right = 1,
            beforeBarline = 2
        };

        CancelLocation parseCancelLocation( const std::string& value );
        std::optional<CancelLocation> tryParseCancelLocation( const std::string& value );
        std::string toString( const CancelLocation value );
        std::ostream& toStream( std::ostream& os, const CancelLocation value );
        std::ostream& operator<<( std::ostream& os, const CancelLocation value );

        /// CircularArrowEnum //////////////////////////////////////////////////////////////////////
        ///
        /// The circular-arrow type represents the direction in which a circular arrow points, using
        /// Unicode arrow terminology.
        ///
        enum class CircularArrowEnum
        {
            clockwise = 0,
            anticlockwise = 1
        };

        CircularArrowEnum parseCircularArrowEnum( const std::string& value );
        std::optional<CircularArrowEnum> tryParseCircularArrowEnum( const std::string& value );
        std::string toString( const CircularArrowEnum value );
        std::ostream& toStream( std::ostream& os, const CircularArrowEnum value );
        std::ostream& operator<<( std::ostream& os, const CircularArrowEnum value );

        /// ClefSign ///////////////////////////////////////////////////////////////////////////////
        ///
        /// The clef-sign element represents the different clef symbols. The jianpu sign indicates
        /// that the music that follows should be in jianpu numbered notation, just as the TAB sign
        /// indicates that the music that follows should be in tablature notation. Unlike TAB, a
        /// jianpu sign does not correspond to a visual clef notation.
        ///
        enum class ClefSign
        {
            g = 0,
            f = 1,
            c = 2,
            percussion = 3,
            tab = 4,
            jianpu = 5,
            none = 6
        };

        ClefSign parseClefSign( const std::string& value );
        std::optional<ClefSign> tryParseClefSign( const std::string& value );
        std::string toString( const ClefSign value );
        std::ostream& toStream( std::ostream& os, const ClefSign value );
        std::ostream& operator<<( std::ostream& os, const ClefSign value );

        /// CssFontSize ////////////////////////////////////////////////////////////////////////////
        ///
        /// The css-font-size type includes the CSS font sizes used as an alternative to a numeric
        /// point size.
        ///
        enum class CssFontSize
        {
            xxSmall = 0,
            xSmall = 1,
            small = 2,
            medium = 3,
            large = 4,
            xLarge = 5,
            xxLarge = 6
        };

        CssFontSize parseCssFontSize( const std::string& value );
        std::optional<CssFontSize> tryParseCssFontSize( const std::string& value );
        std::string toString( const CssFontSize value );
        std::ostream& toStream( std::ostream& os, const CssFontSize value );
        std::ostream& operator<<( std::ostream& os, const CssFontSize value );

        /// DegreeSymbolValue //////////////////////////////////////////////////////////////////////
        ///
        /// The degree-symbol-value type indicates indicates that a symbol should be used in
        /// specifying the degree.
        ///
        enum class DegreeSymbolValue
        {
            major = 0,
            minor = 1,
            augmented = 2,
            diminished = 3,
            halfDiminished = 4
        };

        DegreeSymbolValue parseDegreeSymbolValue( const std::string& value );
        std::optional<DegreeSymbolValue> tryParseDegreeSymbolValue( const std::string& value );
        std::string toString( const DegreeSymbolValue value );
        std::ostream& toStream( std::ostream& os, const DegreeSymbolValue value );
        std::ostream& operator<<( std::ostream& os, const DegreeSymbolValue value );

        /// DegreeTypeValue ////////////////////////////////////////////////////////////////////////
        ///
        /// The degree-type-value type indicates whether the current degree element is an addition,
        /// alteration, or subtraction to the kind of the current chord in the harmony element.
        ///
        enum class DegreeTypeValue
        {
            add = 0,
            alter = 1,
            subtract = 2
        };

        DegreeTypeValue parseDegreeTypeValue( const std::string& value );
        std::optional<DegreeTypeValue> tryParseDegreeTypeValue( const std::string& value );
        std::string toString( const DegreeTypeValue value );
        std::ostream& toStream( std::ostream& os, const DegreeTypeValue value );
        std::ostream& operator<<( std::ostream& os, const DegreeTypeValue value );

        /// DistanceTypeEnum ///////////////////////////////////////////////////////////////////////
        ///
        /// The distance-type defines what type of distance is being defined in a distance element.
        /// Values include beam and hyphen. This is left as a string so that other application-
        /// specific types can be defined, but it is made a separate type so that it can be
        /// redefined more strictly.
        ///
        enum class DistanceTypeEnum
        {
            beam = 0,
            hyphen = 1,
            other = 2
        };

        DistanceTypeEnum parseDistanceTypeEnum( const std::string& value );
        std::optional<DistanceTypeEnum> tryParseDistanceTypeEnum( const std::string& value );
        std::string toString( const DistanceTypeEnum value );
        std::ostream& toStream( std::ostream& os, const DistanceTypeEnum value );
        std::ostream& operator<<( std::ostream& os, const DistanceTypeEnum value );

        class DistanceType
        {
        public:
            explicit DistanceType( const DistanceTypeEnum value );
            explicit DistanceType( const std::string& value );
            DistanceType();
            DistanceTypeEnum getValue() const;
            std::string getValueString() const;
            void setValue( const DistanceTypeEnum value );
            void setValue( const std::string& value );
        private:
            DistanceTypeEnum myEnum;
            std::string myCustomValue;
        };

        DistanceType parseDistanceType( const std::string& value );
        std::string toString( const DistanceType& value );
        std::ostream& toStream( std::ostream& os, const DistanceType& value );
        std::ostream& operator<<( std::ostream& os, const DistanceType& value );

        /// DynamicsEnum ///////////////////////////////////////////////////////////////////////////
        ///
        /// Dynamics can be associated either with a note or a general musical direction. To avoid
        /// inconsistencies between and amongst the letter abbreviations for dynamics (what is sf
        /// vs. sfz, standing alone or with a trailing dynamic that is not always piano), we use the
        /// actual letters as the names of these dynamic elements. The other-dynamics element allows
        /// other dynamic marks that are not covered here, but many of those should perhaps be
        /// included in a more general musical direction element. Dynamics elements may also be
        /// combined to create marks not covered by a single element, such as sfmp.
        ///    
        /// These letter dynamic symbols are separated from crescendo, decrescendo, and wedge
        /// indications. Dynamic representation is inconsistent in scores. Many things are assumed
        /// by the composer and left out, such as returns to original dynamics. Systematic
        /// representations are quite complex: for example, Humdrum has at least 3 representation
        /// formats related to dynamics. The MusicXML format captures what is in the score, but does
        /// not try to be optimal for analysis or synthesis of dynamics.
        ///
        enum class DynamicsEnum
        {
            p = 0,
            pp = 1,
            ppp = 2,
            pppp = 3,
            ppppp = 4,
            pppppp = 5,
            f = 6,
            ff = 7,
            fff = 8,
            ffff = 9,
            fffff = 10,
            ffffff = 11,
            mp = 12,
            mf = 13,
            sf = 14,
            sfp = 15,
            sfpp = 16,
            fp = 17,
            rf = 18,
            rfz = 19,
            sfz = 20,
            sffz = 21,
            fz = 22,
            otherDynamics = 23
        };

        DynamicsEnum parseDynamicsEnum( const std::string& value );
        std::optional<DynamicsEnum> tryParseDynamicsEnum( const std::string& value );
        std::string toString( const DynamicsEnum value );
        std::ostream& toStream( std::ostream& os, const DynamicsEnum value );
        std::ostream& operator<<( std::ostream& os, const DynamicsEnum value );

        class DynamicsValue
        {
        public:
            explicit DynamicsValue( const DynamicsEnum value );
            explicit DynamicsValue( const std::string& value );
            DynamicsValue();
            DynamicsEnum getValue() const;
            std::string getValueString() const;
            void setValue( const DynamicsEnum value );
            void setValue( const std::string& value );
        private:
            DynamicsEnum myEnum;
            std::string myCustomValue;
        };

        DynamicsValue parseDynamicsValue( const std::string& value );
        std::string toString( const DynamicsValue& value );
        std::ostream& toStream( std::ostream& os, const DynamicsValue& value );
        std::ostream& operator<<( std::ostream& os, const DynamicsValue& value );

        /// EffectEnum /////////////////////////////////////////////////////////////////////////////
        ///
        /// The effect type represents pictograms for sound effect percussion instruments. The
        /// cannon value is in addition to Stone's list.
        ///
        enum class EffectEnum
        {
            anvil = 0,
            autoHorn = 1,
            birdWhistle = 2,
            cannon = 3,
            duckCall = 4,
            gunShot = 5,
            klaxonHorn = 6,
            lionsRoar = 7,
            policeWhistle = 8,
            siren = 9,
            slideWhistle = 10,
            thunderSheet = 11,
            windMachine = 12,
            windWhistle = 13
        };

        EffectEnum parseEffectEnum( const std::string& value );
        std::optional<EffectEnum> tryParseEffectEnum( const std::string& value );
        std::string toString( const EffectEnum value );
        std::ostream& toStream( std::ostream& os, const EffectEnum value );
        std::ostream& operator<<( std::ostream& os, const EffectEnum value );

        /// EnclosureShape /////////////////////////////////////////////////////////////////////////
        ///
        /// The enclosure-shape type describes the shape and presence / absence of an enclosure
        /// around text or symbols. A bracket enclosure is similar to a rectangle with the bottom
        /// line missing, as is common in jazz notation.
        ///
        enum class EnclosureShape
        {
            rectangle = 0,
            square = 1,
            oval = 2,
            circle = 3,
            bracket = 4,
            triangle = 5,
            diamond = 6,
            none = 7
        };

        EnclosureShape parseEnclosureShape( const std::string& value );
        std::optional<EnclosureShape> tryParseEnclosureShape( const std::string& value );
        std::string toString( const EnclosureShape value );
        std::ostream& toStream( std::ostream& os, const EnclosureShape value );
        std::ostream& operator<<( std::ostream& os, const EnclosureShape value );

        /// Fan ////////////////////////////////////////////////////////////////////////////////////
        ///
        /// The fan type represents the type of beam fanning present on a note, used to represent
        /// accelerandos and ritardandos.
        ///
        enum class Fan
        {
            accel = 0,
            rit = 1,
            none = 2
        };

        Fan parseFan( const std::string& value );
        std::optional<Fan> tryParseFan( const std::string& value );
        std::string toString( const Fan value );
        std::ostream& toStream( std::ostream& os, const Fan value );
        std::ostream& operator<<( std::ostream& os, const Fan value );

        /// FermataShape ///////////////////////////////////////////////////////////////////////////
        ///
        /// The fermata-shape type represents the shape of the fermata sign. The empty value is
        /// equivalent to the normal value.
        ///
        enum class FermataShape
        {
            normal = 0,
            angled = 1,
            square = 2,
            emptystring = 3
        };

        FermataShape parseFermataShape( const std::string& value );
        std::optional<FermataShape> tryParseFermataShape( const std::string& value );
        std::string toString( const FermataShape value );
        std::ostream& toStream( std::ostream& os, const FermataShape value );
        std::ostream& operator<<( std::ostream& os, const FermataShape value );

        /// FontStyle //////////////////////////////////////////////////////////////////////////////
        ///
        /// The font-style type represents a simplified version of the CSS font-style property.
        ///
        enum class FontStyle
        {
            normal = 0,
            italic = 1
        };

        FontStyle parseFontStyle( const std::string& value );
        std::optional<FontStyle> tryParseFontStyle( const std::string& value );
        std::string toString( const FontStyle value );
        std::ostream& toStream( std::ostream& os, const FontStyle value );
        std::ostream& operator<<( std::ostream& os, const FontStyle value );

        /// FontWeight /////////////////////////////////////////////////////////////////////////////
        ///
        /// The font-weight type represents a simplified version of the CSS font-weight property.
        ///
        enum class FontWeight
        {
            normal = 0,
            bold = 1
        };

        FontWeight parseFontWeight( const std::string& value );
        std::optional<FontWeight> tryParseFontWeight( const std::string& value );
        std::string toString( const FontWeight value );
        std::ostream& toStream( std::ostream& os, const FontWeight value );
        std::ostream& operator<<( std::ostream& os, const FontWeight value );

        /// GlassEnum //////////////////////////////////////////////////////////////////////////////
        ///
        /// The glass type represents pictograms for glass percussion instruments.
        ///
        enum class GlassEnum
        {
            windChimes = 0
        };

        GlassEnum parseGlassEnum( const std::string& value );
        std::optional<GlassEnum> tryParseGlassEnum( const std::string& value );
        std::string toString( const GlassEnum value );
        std::ostream& toStream( std::ostream& os, const GlassEnum value );
        std::ostream& operator<<( std::ostream& os, const GlassEnum value );

        /// GroupBarlineValue //////////////////////////////////////////////////////////////////////
        ///
        /// The group-barline-value type indicates if the group should have common barlines.
        ///
        enum class GroupBarlineValue
        {
            yes = 0,
            no = 1,
            mensurstrich = 2
        };

        GroupBarlineValue parseGroupBarlineValue( const std::string& value );
        std::optional<GroupBarlineValue> tryParseGroupBarlineValue( const std::string& value );
        std::string toString( const GroupBarlineValue value );
        std::ostream& toStream( std::ostream& os, const GroupBarlineValue value );
        std::ostream& operator<<( std::ostream& os, const GroupBarlineValue value );

        /// GroupSymbolValue ///////////////////////////////////////////////////////////////////////
        ///
        /// The group-symbol-value type indicates how the symbol for a group is indicated in the
        /// score. The default value is none.
        ///
        enum class GroupSymbolValue
        {
            none = 0,
            brace = 1,
            line = 2,
            bracket = 3,
            square = 4
        };

        GroupSymbolValue parseGroupSymbolValue( const std::string& value );
        std::optional<GroupSymbolValue> tryParseGroupSymbolValue( const std::string& value );
        std::string toString( const GroupSymbolValue value );
        std::ostream& toStream( std::ostream& os, const GroupSymbolValue value );
        std::ostream& operator<<( std::ostream& os, const GroupSymbolValue value );

        /// HandbellValue //////////////////////////////////////////////////////////////////////////
        ///
        /// The handbell-value type represents the type of handbell technique being notated.
        ///
        enum class HandbellValue
        {
            damp = 0,
            echo = 1,
            gyro = 2,
            handMartellato = 3,
            malletLift = 4,
            malletTable = 5,
            martellato = 6,
            martellatoLift = 7,
            mutedMartellato = 8,
            pluckLift = 9,
            swing = 10
        };

        HandbellValue parseHandbellValue( const std::string& value );
        std::optional<HandbellValue> tryParseHandbellValue( const std::string& value );
        std::string toString( const HandbellValue value );
        std::ostream& toStream( std::ostream& os, const HandbellValue value );
        std::ostream& operator<<( std::ostream& os, const HandbellValue value );

        /// HarmonyType ////////////////////////////////////////////////////////////////////////////
        ///
        /// The harmony-type type differentiates different types of harmonies when alternate
        /// harmonies are possible. Explicit harmonies have all note present in the music; implied
        /// have some notes missing but implied; alternate represents alternate analyses.
        ///
        enum class HarmonyType
        {
            explicit_ = 0,
            implied = 1,
            alternate = 2
        };

        HarmonyType parseHarmonyType( const std::string& value );
        std::optional<HarmonyType> tryParseHarmonyType( const std::string& value );
        std::string toString( const HarmonyType value );
        std::ostream& toStream( std::ostream& os, const HarmonyType value );
        std::ostream& operator<<( std::ostream& os, const HarmonyType value );

        /// HoleClosedLocation /////////////////////////////////////////////////////////////////////
        ///
        /// The hole-closed-location type indicates which portion of the hole is filled in when the
        /// corresponding hole-closed-value is half.
        ///
        enum class HoleClosedLocation
        {
            right = 0,
            bottom = 1,
            left = 2,
            top = 3
        };

        HoleClosedLocation parseHoleClosedLocation( const std::string& value );
        std::optional<HoleClosedLocation> tryParseHoleClosedLocation( const std::string& value );
        std::string toString( const HoleClosedLocation value );
        std::ostream& toStream( std::ostream& os, const HoleClosedLocation value );
        std::ostream& operator<<( std::ostream& os, const HoleClosedLocation value );

        /// HoleClosedValue ////////////////////////////////////////////////////////////////////////
        ///
        /// The hole-closed-value type represents whether the hole is closed, open, or half-open.
        ///
        enum class HoleClosedValue
        {
            yes = 0,
            no = 1,
            half = 2
        };

        HoleClosedValue parseHoleClosedValue( const std::string& value );
        std::optional<HoleClosedValue> tryParseHoleClosedValue( const std::string& value );
        std::string toString( const HoleClosedValue value );
        std::ostream& toStream( std::ostream& os, const HoleClosedValue value );
        std::ostream& operator<<( std::ostream& os, const HoleClosedValue value );

        /// KindValue //////////////////////////////////////////////////////////////////////////////
        ///
        /// A kind-value indicates the type of chord. Degree elements can then add, subtract, or
        /// alter from these starting points. Values include:
        ///    
        /// Triads:
        ///     major (major third, perfect fifth)
        ///     minor (minor third, perfect fifth)
        ///     augmented (major third, augmented fifth)
        ///     diminished (minor third, diminished fifth)
        /// Sevenths:
        ///     dominant (major triad, minor seventh)
        ///     major-seventh (major triad, major seventh)
        ///     minor-seventh (minor triad, minor seventh)
        ///     diminished-seventh (diminished triad, diminished seventh)
        ///     augmented-seventh (augmented triad, minor seventh)
        ///     half-diminished (diminished triad, minor seventh)
        ///     major-minor (minor triad, major seventh)
        /// Sixths:
        ///     major-sixth (major triad, added sixth)
        ///     minor-sixth (minor triad, added sixth)
        /// Ninths:
        ///     dominant-ninth (dominant-seventh, major ninth)
        ///     major-ninth (major-seventh, major ninth)
        ///     minor-ninth (minor-seventh, major ninth)
        /// 11ths (usually as the basis for alteration):
        ///     dominant-11th (dominant-ninth, perfect 11th)
        ///     major-11th (major-ninth, perfect 11th)
        ///     minor-11th (minor-ninth, perfect 11th)
        /// 13ths (usually as the basis for alteration):
        ///     dominant-13th (dominant-11th, major 13th)
        ///     major-13th (major-11th, major 13th)
        ///     minor-13th (minor-11th, major 13th)
        /// Suspended:
        ///     suspended-second (major second, perfect fifth)
        ///     suspended-fourth (perfect fourth, perfect fifth)
        /// Functional sixths:
        ///     Neapolitan
        ///     Italian
        ///     French
        ///     German
        /// Other:
        ///     pedal (pedal-point bass)
        ///     power (perfect fifth)
        ///     Tristan
        ///    
        /// The "other" kind is used when the harmony is entirely composed of add elements. The
        /// "none" kind is used to explicitly encode absence of chords or functional harmony.
        ///
        enum class KindValue
        {
            major = 0,
            minor = 1,
            augmented = 2,
            diminished = 3,
            dominant = 4,
            majorSeventh = 5,
            minorSeventh = 6,
            diminishedSeventh = 7,
            augmentedSeventh = 8,
            halfDiminished = 9,
            majorMinor = 10,
            majorSixth = 11,
            minorSixth = 12,
            dominantNinth = 13,
            majorNinth = 14,
            minorNinth = 15,
            dominant11th = 16,
            major11th = 17,
            minor11th = 18,
            dominant13th = 19,
            major13th = 20,
            minor13th = 21,
            suspendedSecond = 22,
            suspendedFourth = 23,
            neapolitan = 24,
            italian = 25,
            french = 26,
            german = 27,
            pedal = 28,
            power = 29,
            tristan = 30,
            other = 31,
            none = 32
        };

        KindValue parseKindValue( const std::string& value );
        std::optional<KindValue> tryParseKindValue( const std::string& value );
        std::string toString( const KindValue value );
        std::ostream& toStream( std::ostream& os, const KindValue value );
        std::ostream& operator<<( std::ostream& os, const KindValue value );

        /// LeftCenterRight ////////////////////////////////////////////////////////////////////////
        ///
        /// The left-center-right type is used to define horizontal alignment and text
        /// justification.
        ///
        enum class LeftCenterRight
        {
            left = 0,
            center = 1,
            right = 2
        };

        LeftCenterRight parseLeftCenterRight( const std::string& value );
        std::optional<LeftCenterRight> tryParseLeftCenterRight( const std::string& value );
        std::string toString( const LeftCenterRight value );
        std::ostream& toStream( std::ostream& os, const LeftCenterRight value );
        std::ostream& operator<<( std::ostream& os, const LeftCenterRight value );

        /// LeftRight //////////////////////////////////////////////////////////////////////////////
        ///
        /// The left-right type is used to indicate whether one element appears to the left or the
        /// right of another element.
        ///
        enum class LeftRight
        {
            left = 0,
            right = 1
        };

        LeftRight parseLeftRight( const std::string& value );
        std::optional<LeftRight> tryParseLeftRight( const std::string& value );
        std::string toString( const LeftRight value );
        std::ostream& toStream( std::ostream& os, const LeftRight value );
        std::ostream& operator<<( std::ostream& os, const LeftRight value );

        /// LineEnd ////////////////////////////////////////////////////////////////////////////////
        ///
        /// The line-end type specifies if there is a jog up or down (or both), an arrow, or nothing
        /// at the start or end of a bracket.
        ///
        enum class LineEnd
        {
            up = 0,
            down = 1,
            both = 2,
            arrow = 3,
            none = 4
        };

        LineEnd parseLineEnd( const std::string& value );
        std::optional<LineEnd> tryParseLineEnd( const std::string& value );
        std::string toString( const LineEnd value );
        std::ostream& toStream( std::ostream& os, const LineEnd value );
        std::ostream& operator<<( std::ostream& os, const LineEnd value );

        /// LineShape //////////////////////////////////////////////////////////////////////////////
        ///
        /// The line-shape type distinguishes between straight and curved lines.
        ///
        enum class LineShape
        {
            straight = 0,
            curved = 1
        };

        LineShape parseLineShape( const std::string& value );
        std::optional<LineShape> tryParseLineShape( const std::string& value );
        std::string toString( const LineShape value );
        std::ostream& toStream( std::ostream& os, const LineShape value );
        std::ostream& operator<<( std::ostream& os, const LineShape value );

        /// LineType ///////////////////////////////////////////////////////////////////////////////
        ///
        /// The line-type type distinguishes between solid, dashed, dotted, and wavy lines.
        ///
        enum class LineType
        {
            solid = 0,
            dashed = 1,
            dotted = 2,
            wavy = 3
        };

        LineType parseLineType( const std::string& value );
        std::optional<LineType> tryParseLineType( const std::string& value );
        std::string toString( const LineType value );
        std::ostream& toStream( std::ostream& os, const LineType value );
        std::ostream& operator<<( std::ostream& os, const LineType value );

        /// LineWidthTypeEnum //////////////////////////////////////////////////////////////////////
        ///
        /// The line-width-type defines what type of line is being defined in a line-width element.
        /// Values include beam, bracket, dashes, enclosure, ending, extend, heavy barline, leger,
        /// light barline, octave shift, pedal, slur middle, slur tip, staff, stem, tie middle, tie
        /// tip, tuplet bracket, and wedge. This is left as a string so that other application-
        /// specific types can be defined, but it is made a separate type so that it can be
        /// redefined more strictly.
        ///
        enum class LineWidthTypeEnum
        {
            beam = 0,
            bracket = 1,
            dashes = 2,
            enclosure = 3,
            ending = 4,
            extend = 5,
            heavyBarline = 6,
            leger = 7,
            lightBarline = 8,
            octaveShift = 9,
            pedal = 10,
            slurMiddle = 11,
            slurTip = 12,
            staff = 13,
            stem = 14,
            tieMiddle = 15,
            tieTip = 16,
            tupletBracket = 17,
            wedge = 18,
            other = 19
        };

        LineWidthTypeEnum parseLineWidthTypeEnum( const std::string& value );
        std::optional<LineWidthTypeEnum> tryParseLineWidthTypeEnum( const std::string& value );
        std::string toString( const LineWidthTypeEnum value );
        std::ostream& toStream( std::ostream& os, const LineWidthTypeEnum value );
        std::ostream& operator<<( std::ostream& os, const LineWidthTypeEnum value );

        class LineWidthType
        {
        public:
            explicit LineWidthType( const LineWidthTypeEnum value );
            explicit LineWidthType( const std::string& value );
            LineWidthType();
            LineWidthTypeEnum getValue() const;
            std::string getValueString() const;
            void setValue( const LineWidthTypeEnum value );
            void setValue( const std::string& value );
        private:
            LineWidthTypeEnum myEnum;
            std::string myCustomValue;
        };

        LineWidthType parseLineWidthType( const std::string& value );
        std::string toString( const LineWidthType& value );
        std::ostream& toStream( std::ostream& os, const LineWidthType& value );
        std::ostream& operator<<( std::ostream& os, const LineWidthType& value );

        /// MarginType /////////////////////////////////////////////////////////////////////////////
        ///
        /// The margin-type type specifies whether margins apply to even page, odd pages, or both.
        ///
        enum class MarginType
        {
            odd = 0,
            even = 1,
            both = 2
        };

        MarginType parseMarginType( const std::string& value );
        std::optional<MarginType> tryParseMarginType( const std::string& value );
        std::string toString( const MarginType value );
        std::ostream& toStream( std::ostream& os, const MarginType value );
        std::ostream& operator<<( std::ostream& os, const MarginType value );

        /// MeasureNumberingValue //////////////////////////////////////////////////////////////////
        ///
        /// The measure-numbering-value type describes how measure numbers are displayed on this
        /// part: no numbers, numbers every measure, or numbers every system.
        ///
        enum class MeasureNumberingValue
        {
            none = 0,
            measure = 1,
            system = 2
        };

        MeasureNumberingValue parseMeasureNumberingValue( const std::string& value );
        std::optional<MeasureNumberingValue> tryParseMeasureNumberingValue( const std::string& value );
        std::string toString( const MeasureNumberingValue value );
        std::ostream& toStream( std::ostream& os, const MeasureNumberingValue value );
        std::ostream& operator<<( std::ostream& os, const MeasureNumberingValue value );

        /// MembraneEnum ///////////////////////////////////////////////////////////////////////////
        ///
        /// The membrane type represents pictograms for membrane percussion instruments. The goblet
        /// drum value is in addition to Stone's list.
        ///
        enum class MembraneEnum
        {
            bassDrum = 0,
            bassDrumOnSide = 1,
            bongos = 2,
            congaDrum = 3,
            gobletDrum = 4,
            militaryDrum = 5,
            snareDrum = 6,
            snareDrumSnaresOff = 7,
            tambourine = 8,
            tenorDrum = 9,
            timbales = 10,
            tomtom = 11
        };

        MembraneEnum parseMembraneEnum( const std::string& value );
        std::optional<MembraneEnum> tryParseMembraneEnum( const std::string& value );
        std::string toString( const MembraneEnum value );
        std::ostream& toStream( std::ostream& os, const MembraneEnum value );
        std::ostream& operator<<( std::ostream& os, const MembraneEnum value );

        /// MetalEnum //////////////////////////////////////////////////////////////////////////////
        ///
        /// The metal type represents pictograms for metal percussion instruments. The hi-hat value
        /// refers to a pictogram like Stone's high-hat cymbals but without the long vertical line
        /// at the bottom.
        ///
        enum class MetalEnum
        {
            almglocken = 0,
            bell = 1,
            bellPlate = 2,
            brakeDrum = 3,
            chineseCymbal = 4,
            cowbell = 5,
            crashCymbals = 6,
            crotale = 7,
            cymbalTongs = 8,
            domedGong = 9,
            fingerCymbals = 10,
            flexatone = 11,
            gong = 12,
            hiHat = 13,
            highHatCymbals = 14,
            handbell = 15,
            sistrum = 16,
            sizzleCymbal = 17,
            sleighBells = 18,
            suspendedCymbal = 19,
            tamTam = 20,
            triangle = 21,
            vietnameseHat = 22
        };

        MetalEnum parseMetalEnum( const std::string& value );
        std::optional<MetalEnum> tryParseMetalEnum( const std::string& value );
        std::string toString( const MetalEnum value );
        std::ostream& toStream( std::ostream& os, const MetalEnum value );
        std::ostream& operator<<( std::ostream& os, const MetalEnum value );

        /// ModeEnum ///////////////////////////////////////////////////////////////////////////////
        ///
        /// The mode type is used to specify major/minor and other mode distinctions. Valid mode
        /// values include major, minor, dorian, phrygian, lydian, mixolydian, aeolian, ionian,
        /// locrian, and none.
        ///
        enum class ModeEnum
        {
            major = 0,
            minor = 1,
            dorian = 2,
            phrygian = 3,
            lydian = 4,
            mixolydian = 5,
            aeolian = 6,
            ionian = 7,
            locrian = 8,
            none = 9,
            other = 10
        };

        ModeEnum parseModeEnum( const std::string& value );
        std::optional<ModeEnum> tryParseModeEnum( const std::string& value );
        std::string toString( const ModeEnum value );
        std::ostream& toStream( std::ostream& os, const ModeEnum value );
        std::ostream& operator<<( std::ostream& os, const ModeEnum value );

        class ModeValue
        {
        public:
            explicit ModeValue( const ModeEnum value );
            explicit ModeValue( const std::string& value );
            ModeValue();
            ModeEnum getValue() const;
            std::string getValueString() const;
            void setValue( const ModeEnum value );
            void setValue( const std::string& value );
        private:
            ModeEnum myEnum;
            std::string myCustomValue;
        };

        ModeValue parseModeValue( const std::string& value );
        std::string toString( const ModeValue& value );
        std::ostream& toStream( std::ostream& os, const ModeValue& value );
        std::ostream& operator<<( std::ostream& os, const ModeValue& value );

        /// MuteEnum ///////////////////////////////////////////////////////////////////////////////
        ///
        /// The mute type represents muting for different instruments, including brass, winds, and
        /// strings. The on and off values are used for undifferentiated mutes. The remaining values
        /// represent specific mutes.
        ///
        enum class MuteEnum
        {
            on = 0,
            off = 1,
            straight = 2,
            cup = 3,
            harmonNoStem = 4,
            harmonStem = 5,
            bucket = 6,
            plunger = 7,
            hat = 8,
            solotone = 9,
            practice = 10,
            stopMute = 11,
            stopHand = 12,
            echo = 13,
            palm = 14
        };

        MuteEnum parseMuteEnum( const std::string& value );
        std::optional<MuteEnum> tryParseMuteEnum( const std::string& value );
        std::string toString( const MuteEnum value );
        std::ostream& toStream( std::ostream& os, const MuteEnum value );
        std::ostream& operator<<( std::ostream& os, const MuteEnum value );

        /// NoteSizeType ///////////////////////////////////////////////////////////////////////////
        ///
        /// The note-size-type type indicates the type of note being defined by a note-size element.
        /// The grace type is used for notes of cue size that that include a grace element. The cue
        /// type is used for all other notes with cue size, whether defined explicitly or implicitly
        /// via a cue element. The large type is used for notes of large size.
        ///
        enum class NoteSizeType
        {
            cue = 0,
            grace = 1,
            large = 2
        };

        NoteSizeType parseNoteSizeType( const std::string& value );
        std::optional<NoteSizeType> tryParseNoteSizeType( const std::string& value );
        std::string toString( const NoteSizeType value );
        std::ostream& toStream( std::ostream& os, const NoteSizeType value );
        std::ostream& operator<<( std::ostream& os, const NoteSizeType value );

        /// NoteTypeValue //////////////////////////////////////////////////////////////////////////
        ///
        /// The note-type type is used for the MusicXML type element and represents the graphic note
        /// type, from 1024th (shortest) to maxima (longest).
        ///
        enum class NoteTypeValue
        {
            oneThousandTwentyFourth = 0,
            fiveHundredTwelfth = 1,
            twoHundredFifthySixth = 2,
            oneHundredTwentyEighth = 3,
            sixtyFourth = 4,
            thirtySecond = 5,
            sixteenth = 6,
            eighth = 7,
            quarter = 8,
            half = 9,
            whole = 10,
            breve = 11,
            long_ = 12,
            maxima = 13
        };

        NoteTypeValue parseNoteTypeValue( const std::string& value );
        std::optional<NoteTypeValue> tryParseNoteTypeValue( const std::string& value );
        std::string toString( const NoteTypeValue value );
        std::ostream& toStream( std::ostream& os, const NoteTypeValue value );
        std::ostream& operator<<( std::ostream& os, const NoteTypeValue value );

        /// NoteheadValue //////////////////////////////////////////////////////////////////////////
        ///
        /// The notehead type indicates shapes other than the open and closed ovals associated with
        /// note durations. The values do, re, mi, fa, fa up, so, la, and ti correspond to Aikin's
        /// 7-shape system.  The fa up shape is typically used with upstems; the fa shape is
        /// typically used with downstems or no stems.
        ///
        /// The arrow shapes differ from triangle and inverted triangle by being centered on the
        /// stem. Slashed and back slashed notes include both the normal notehead and a slash. The
        /// triangle shape has the tip of the triangle pointing up; the inverted triangle shape has
        /// the tip of the triangle pointing down. The left triangle shape is a right triangle with
        /// the hypotenuse facing up and to the left.
        ///
        enum class NoteheadValue
        {
            slash = 0,
            triangle = 1,
            diamond = 2,
            square = 3,
            cross = 4,
            x = 5,
            circleX = 6,
            invertedTriangle = 7,
            arrowDown = 8,
            arrowUp = 9,
            slashed = 10,
            backSlashed = 11,
            normal = 12,
            cluster = 13,
            circleDot = 14,
            leftTriangle = 15,
            rectangle = 16,
            none = 17,
            do_ = 18,
            re = 19,
            mi = 20,
            fa = 21,
            faUp = 22,
            so = 23,
            la = 24,
            ti = 25
        };

        NoteheadValue parseNoteheadValue( const std::string& value );
        std::optional<NoteheadValue> tryParseNoteheadValue( const std::string& value );
        std::string toString( const NoteheadValue value );
        std::ostream& toStream( std::ostream& os, const NoteheadValue value );
        std::ostream& operator<<( std::ostream& os, const NoteheadValue value );

        /// OnOff //////////////////////////////////////////////////////////////////////////////////
        ///
        /// The on-off type is used for notation elements such as string mutes.
        ///
        enum class OnOff
        {
            on = 0,
            off = 1
        };

        OnOff parseOnOff( const std::string& value );
        std::optional<OnOff> tryParseOnOff( const std::string& value );
        std::string toString( const OnOff value );
        std::ostream& toStream( std::ostream& os, const OnOff value );
        std::ostream& operator<<( std::ostream& os, const OnOff value );

        /// OverUnder //////////////////////////////////////////////////////////////////////////////
        ///
        /// The over-under type is used to indicate whether the tips of curved lines such as slurs
        /// and ties are overhand (tips down) or underhand (tips up).
        ///
        enum class OverUnder
        {
            over = 0,
            under = 1
        };

        OverUnder parseOverUnder( const std::string& value );
        std::optional<OverUnder> tryParseOverUnder( const std::string& value );
        std::string toString( const OverUnder value );
        std::ostream& toStream( std::ostream& os, const OverUnder value );
        std::ostream& operator<<( std::ostream& os, const OverUnder value );

        /// PitchedEnum ////////////////////////////////////////////////////////////////////////////
        ///
        /// The pitched type represents pictograms for pitched percussion instruments. The chimes
        /// and tubular chimes values distinguish the single-line and double-line versions of the
        /// pictogram. The mallet value is in addition to Stone's list.
        ///
        enum class PitchedEnum
        {
            chimes = 0,
            glockenspiel = 1,
            mallet = 2,
            marimba = 3,
            tubularChimes = 4,
            vibraphone = 5,
            xylophone = 6
        };

        PitchedEnum parsePitchedEnum( const std::string& value );
        std::optional<PitchedEnum> tryParsePitchedEnum( const std::string& value );
        std::string toString( const PitchedEnum value );
        std::ostream& toStream( std::ostream& os, const PitchedEnum value );
        std::ostream& operator<<( std::ostream& os, const PitchedEnum value );

        /// PrincipalVoiceSymbol ///////////////////////////////////////////////////////////////////
        ///
        /// The principal-voice-symbol type represents the type of symbol used to indicate the start
        /// of a principal or secondary voice. The "plain" value represents a plain square bracket.
        /// The value of "none" is used for analysis markup when the principal-voice element does
        /// not have a corresponding appearance in the score.
        ///
        enum class PrincipalVoiceSymbol
        {
            hauptstimme = 0,
            nebenstimme = 1,
            plain = 2,
            none = 3
        };

        PrincipalVoiceSymbol parsePrincipalVoiceSymbol( const std::string& value );
        std::optional<PrincipalVoiceSymbol> tryParsePrincipalVoiceSymbol( const std::string& value );
        std::string toString( const PrincipalVoiceSymbol value );
        std::ostream& toStream( std::ostream& os, const PrincipalVoiceSymbol value );
        std::ostream& operator<<( std::ostream& os, const PrincipalVoiceSymbol value );

        /// RightLeftMiddle ////////////////////////////////////////////////////////////////////////
        ///
        /// The right-left-middle type is used to specify barline location.
        ///
        enum class RightLeftMiddle
        {
            right = 0,
            left = 1,
            middle = 2
        };

        RightLeftMiddle parseRightLeftMiddle( const std::string& value );
        std::optional<RightLeftMiddle> tryParseRightLeftMiddle( const std::string& value );
        std::string toString( const RightLeftMiddle value );
        std::ostream& toStream( std::ostream& os, const RightLeftMiddle value );
        std::ostream& operator<<( std::ostream& os, const RightLeftMiddle value );

        /// SemiPitchedEnum ////////////////////////////////////////////////////////////////////////
        ///
        /// The semi-pitched type represents categories of indefinite pitch for percussion
        /// instruments.
        ///
        enum class SemiPitchedEnum
        {
            high = 0,
            mediumHigh = 1,
            medium = 2,
            mediumLow = 3,
            low = 4,
            veryLow = 5
        };

        SemiPitchedEnum parseSemiPitchedEnum( const std::string& value );
        std::optional<SemiPitchedEnum> tryParseSemiPitchedEnum( const std::string& value );
        std::string toString( const SemiPitchedEnum value );
        std::ostream& toStream( std::ostream& os, const SemiPitchedEnum value );
        std::ostream& operator<<( std::ostream& os, const SemiPitchedEnum value );

        /// ShowFrets //////////////////////////////////////////////////////////////////////////////
        ///
        /// The show-frets type indicates whether to show tablature frets as numbers (0, 1, 2) or
        /// letters (a, b, c). The default choice is numbers.
        ///
        enum class ShowFrets
        {
            numbers = 0,
            letters = 1
        };

        ShowFrets parseShowFrets( const std::string& value );
        std::optional<ShowFrets> tryParseShowFrets( const std::string& value );
        std::string toString( const ShowFrets value );
        std::ostream& toStream( std::ostream& os, const ShowFrets value );
        std::ostream& operator<<( std::ostream& os, const ShowFrets value );

        /// ShowTuplet /////////////////////////////////////////////////////////////////////////////
        ///
        /// The show-tuplet type indicates whether to show a part of a tuplet relating to the
        /// tuplet-actual element, both the tuplet-actual and tuplet-normal elements, or neither.
        ///
        enum class ShowTuplet
        {
            actual = 0,
            both = 1,
            none = 2
        };

        ShowTuplet parseShowTuplet( const std::string& value );
        std::optional<ShowTuplet> tryParseShowTuplet( const std::string& value );
        std::string toString( const ShowTuplet value );
        std::ostream& toStream( std::ostream& os, const ShowTuplet value );
        std::ostream& operator<<( std::ostream& os, const ShowTuplet value );

        /// StaffTypeEnum //////////////////////////////////////////////////////////////////////////
        ///
        /// The staff-type value can be ossia, cue, editorial, regular, or alternate. An alternate
        /// staff indicates one that shares the same musical data as the prior staff, but displayed
        /// differently (e.g., treble and bass clef, standard notation and tab).
        ///
        enum class StaffTypeEnum
        {
            ossia = 0,
            cue = 1,
            editorial = 2,
            regular = 3,
            alternate = 4
        };

        StaffTypeEnum parseStaffTypeEnum( const std::string& value );
        std::optional<StaffTypeEnum> tryParseStaffTypeEnum( const std::string& value );
        std::string toString( const StaffTypeEnum value );
        std::ostream& toStream( std::ostream& os, const StaffTypeEnum value );
        std::ostream& operator<<( std::ostream& os, const StaffTypeEnum value );

        /// StartNote //////////////////////////////////////////////////////////////////////////////
        ///
        /// The start-note type describes the starting note of trills and mordents for playback,
        /// relative to the current note.
        ///
        enum class StartNote
        {
            upper = 0,
            main = 1,
            below = 2
        };

        StartNote parseStartNote( const std::string& value );
        std::optional<StartNote> tryParseStartNote( const std::string& value );
        std::string toString( const StartNote value );
        std::ostream& toStream( std::ostream& os, const StartNote value );
        std::ostream& operator<<( std::ostream& os, const StartNote value );

        /// StartStop //////////////////////////////////////////////////////////////////////////////
        ///
        /// The start-stop type is used for an attribute of musical elements that can either start
        /// or stop, such as tuplets.
        ///                                    
        /// The values of start and stop refer to how an element appears in musical score order, not
        /// in MusicXML document order. An element with a stop attribute may precede the
        /// corresponding element with a start attribute within a MusicXML document. This is
        /// particularly common in multi-staff music. For example, the stopping point for a tuplet
        /// may appear in staff 1 before the starting point for the tuplet appears in staff 2 later
        /// in the document.
        ///
        enum class StartStop
        {
            start = 0,
            stop = 1
        };

        StartStop parseStartStop( const std::string& value );
        std::optional<StartStop> tryParseStartStop( const std::string& value );
        std::string toString( const StartStop value );
        std::ostream& toStream( std::ostream& os, const StartStop value );
        std::ostream& operator<<( std::ostream& os, const StartStop value );

        /// StartStopChangeContinue ////////////////////////////////////////////////////////////////
        ///
        /// The start-stop-change-continue type is used to distinguish types of pedal directions.
        ///
        enum class StartStopChangeContinue
        {
            start = 0,
            stop = 1,
            change = 2,
            continue_ = 3
        };

        StartStopChangeContinue parseStartStopChangeContinue( const std::string& value );
        std::optional<StartStopChangeContinue> tryParseStartStopChangeContinue( const std::string& value );
        std::string toString( const StartStopChangeContinue value );
        std::ostream& toStream( std::ostream& os, const StartStopChangeContinue value );
        std::ostream& operator<<( std::ostream& os, const StartStopChangeContinue value );

        /// StartStopContinue //////////////////////////////////////////////////////////////////////
        ///
        /// The start-stop-continue type is used for an attribute of musical elements that can
        /// either start or stop, but also need to refer to an intermediate point in the symbol, as
        /// for complex slurs or for formatting of symbols across system breaks.
        ///                        
        /// The values of start, stop, and continue refer to how an element appears in musical score
        /// order, not in MusicXML document order. An element with a stop attribute may precede the
        /// corresponding element with a start attribute within a MusicXML document. This is
        /// particularly common in multi-staff music. For example, the stopping point for a slur may
        /// appear in staff 1 before the starting point for the slur appears in staff 2 later in the
        /// document.
        ///
        enum class StartStopContinue
        {
            start = 0,
            stop = 1,
            continue_ = 2
        };

        StartStopContinue parseStartStopContinue( const std::string& value );
        std::optional<StartStopContinue> tryParseStartStopContinue( const std::string& value );
        std::string toString( const StartStopContinue value );
        std::ostream& toStream( std::ostream& os, const StartStopContinue value );
        std::ostream& operator<<( std::ostream& os, const StartStopContinue value );

        /// StartStopDiscontinue ///////////////////////////////////////////////////////////////////
        ///
        /// The start-stop-discontinue type is used to specify ending types. Typically, the start
        /// type is associated with the left barline of the first measure in an ending. The stop and
        /// discontinue types are associated with the right barline of the last measure in an
        /// ending. Stop is used when the ending mark concludes with a downward jog, as is typical
        /// for first endings. Discontinue is used when there is no downward jog, as is typical for
        /// second endings that do not conclude a piece.
        ///
        enum class StartStopDiscontinue
        {
            start = 0,
            stop = 1,
            discontinue = 2
        };

        StartStopDiscontinue parseStartStopDiscontinue( const std::string& value );
        std::optional<StartStopDiscontinue> tryParseStartStopDiscontinue( const std::string& value );
        std::string toString( const StartStopDiscontinue value );
        std::ostream& toStream( std::ostream& os, const StartStopDiscontinue value );
        std::ostream& operator<<( std::ostream& os, const StartStopDiscontinue value );

        /// StartStopSingle ////////////////////////////////////////////////////////////////////////
        ///
        /// The start-stop-single type is used for an attribute of musical elements that can be used
        /// for either multi-note or single-note musical elements, as for tremolos.
        ///
        enum class StartStopSingle
        {
            start = 0,
            stop = 1,
            single = 2
        };

        StartStopSingle parseStartStopSingle( const std::string& value );
        std::optional<StartStopSingle> tryParseStartStopSingle( const std::string& value );
        std::string toString( const StartStopSingle value );
        std::ostream& toStream( std::ostream& os, const StartStopSingle value );
        std::ostream& operator<<( std::ostream& os, const StartStopSingle value );

        /// StemValue //////////////////////////////////////////////////////////////////////////////
        ///
        /// The stem type represents the notated stem direction.
        ///
        enum class StemValue
        {
            down = 0,
            up = 1,
            double_ = 2,
            none = 3
        };

        StemValue parseStemValue( const std::string& value );
        std::optional<StemValue> tryParseStemValue( const std::string& value );
        std::string toString( const StemValue value );
        std::ostream& toStream( std::ostream& os, const StemValue value );
        std::ostream& operator<<( std::ostream& os, const StemValue value );

        /// StepEnum ///////////////////////////////////////////////////////////////////////////////
        ///
        /// The step type represents a step of the diatonic scale, represented using the English
        /// letters A through G.
        ///
        enum class StepEnum
        {
            a = 0,
            b = 1,
            c = 2,
            d = 3,
            e = 4,
            f = 5,
            g = 6
        };

        StepEnum parseStepEnum( const std::string& value );
        std::optional<StepEnum> tryParseStepEnum( const std::string& value );
        std::string toString( const StepEnum value );
        std::ostream& toStream( std::ostream& os, const StepEnum value );
        std::ostream& operator<<( std::ostream& os, const StepEnum value );

        /// StickLocationEnum //////////////////////////////////////////////////////////////////////
        ///
        /// The stick-location type represents pictograms for the location of sticks, beaters, or
        /// mallets on cymbals, gongs, drums, and other instruments.
        ///
        enum class StickLocationEnum
        {
            center = 0,
            rim = 1,
            cymbalBell = 2,
            cymbalEdge = 3
        };

        StickLocationEnum parseStickLocationEnum( const std::string& value );
        std::optional<StickLocationEnum> tryParseStickLocationEnum( const std::string& value );
        std::string toString( const StickLocationEnum value );
        std::ostream& toStream( std::ostream& os, const StickLocationEnum value );
        std::ostream& operator<<( std::ostream& os, const StickLocationEnum value );

        /// StickMaterialEnum //////////////////////////////////////////////////////////////////////
        ///
        /// The stick-material type represents the material being displayed in a stick pictogram.
        ///
        enum class StickMaterialEnum
        {
            soft = 0,
            medium = 1,
            hard = 2,
            shaded = 3,
            x = 4
        };

        StickMaterialEnum parseStickMaterialEnum( const std::string& value );
        std::optional<StickMaterialEnum> tryParseStickMaterialEnum( const std::string& value );
        std::string toString( const StickMaterialEnum value );
        std::ostream& toStream( std::ostream& os, const StickMaterialEnum value );
        std::ostream& operator<<( std::ostream& os, const StickMaterialEnum value );

        /// StickTypeEnum //////////////////////////////////////////////////////////////////////////
        ///
        /// The stick-type type represents the shape of pictograms where the material
        ///     in the stick, mallet, or beater is represented in the pictogram.
        ///
        enum class StickTypeEnum
        {
            bassDrum = 0,
            doubleBassDrum = 1,
            timpani = 2,
            xylophone = 3,
            yarn = 4
        };

        StickTypeEnum parseStickTypeEnum( const std::string& value );
        std::optional<StickTypeEnum> tryParseStickTypeEnum( const std::string& value );
        std::string toString( const StickTypeEnum value );
        std::ostream& toStream( std::ostream& os, const StickTypeEnum value );
        std::ostream& operator<<( std::ostream& os, const StickTypeEnum value );

        /// SyllabicEnum ///////////////////////////////////////////////////////////////////////////
        ///
        /// Lyric hyphenation is indicated by the syllabic type. The single, begin, end, and middle
        /// values represent single-syllable words, word-beginning syllables, word-ending syllables,
        /// and mid-word syllables, respectively.
        ///
        enum class SyllabicEnum
        {
            single = 0,
            begin = 1,
            end = 2,
            middle = 3
        };

        SyllabicEnum parseSyllabicEnum( const std::string& value );
        std::optional<SyllabicEnum> tryParseSyllabicEnum( const std::string& value );
        std::string toString( const SyllabicEnum value );
        std::ostream& toStream( std::ostream& os, const SyllabicEnum value );
        std::ostream& operator<<( std::ostream& os, const SyllabicEnum value );

        /// SymbolSize /////////////////////////////////////////////////////////////////////////////
        ///
        /// The symbol-size type is used to indicate full vs. cue-sized vs. oversized symbols. The
        /// large value for oversized symbols was added in version 1.1.
        ///
        enum class SymbolSize
        {
            full = 0,
            cue = 1,
            large = 2
        };

        SymbolSize parseSymbolSize( const std::string& value );
        std::optional<SymbolSize> tryParseSymbolSize( const std::string& value );
        std::string toString( const SymbolSize value );
        std::ostream& toStream( std::ostream& os, const SymbolSize value );
        std::ostream& operator<<( std::ostream& os, const SymbolSize value );

        /// TextDirection //////////////////////////////////////////////////////////////////////////
        ///
        /// The text-direction type is used to adjust and override the Unicode bidirectional text
        /// algorithm, similar to the W3C Internationalization Tag Set recommendation. Values are
        /// ltr (left-to-right embed), rtl (right-to-left embed), lro (left-to-right bidi-override),
        /// and rlo (right-to-left bidi-override). The default value is ltr. This type is typically
        /// used by applications that store text in left-to-right visual order rather than logical
        /// order. Such applications can use the lro value to better communicate with other
        /// applications that more fully support bidirectional text.
        ///
        enum class TextDirection
        {
            ltr = 0,
            rtl = 1,
            lro = 2,
            rlo = 3
        };

        TextDirection parseTextDirection( const std::string& value );
        std::optional<TextDirection> tryParseTextDirection( const std::string& value );
        std::string toString( const TextDirection value );
        std::ostream& toStream( std::ostream& os, const TextDirection value );
        std::ostream& operator<<( std::ostream& os, const TextDirection value );

        /// TimeRelationEnum ///////////////////////////////////////////////////////////////////////
        ///
        /// The time-relation type indicates the symbol used to represent the interchangeable aspect
        /// of dual time signatures.
        ///
        enum class TimeRelationEnum
        {
            parentheses = 0,
            bracket = 1,
            equals = 2,
            slash = 3,
            space = 4,
            hyphen = 5
        };

        TimeRelationEnum parseTimeRelationEnum( const std::string& value );
        std::optional<TimeRelationEnum> tryParseTimeRelationEnum( const std::string& value );
        std::string toString( const TimeRelationEnum value );
        std::ostream& toStream( std::ostream& os, const TimeRelationEnum value );
        std::ostream& operator<<( std::ostream& os, const TimeRelationEnum value );

        /// TimeSeparator //////////////////////////////////////////////////////////////////////////
        ///
        /// The time-separator type indicates how to display the arrangement between the beats and
        /// beat-type values in a time signature. The default value is none. The horizontal,
        /// diagonal, and vertical values represent horizontal, diagonal lower-left to upper-right,
        /// and vertical lines respectively. For these values, the beats and beat-type values are
        /// arranged on either side of the separator line. The none value represents no separator
        /// with the beats and beat-type arranged vertically. The adjacent value represents no
        /// separator with the beats and beat-type arranged horizontally.
        ///
        enum class TimeSeparator
        {
            none = 0,
            horizontal = 1,
            diagonal = 2,
            vertical = 3,
            adjacent = 4
        };

        TimeSeparator parseTimeSeparator( const std::string& value );
        std::optional<TimeSeparator> tryParseTimeSeparator( const std::string& value );
        std::string toString( const TimeSeparator value );
        std::ostream& toStream( std::ostream& os, const TimeSeparator value );
        std::ostream& operator<<( std::ostream& os, const TimeSeparator value );

        /// TimeSymbol /////////////////////////////////////////////////////////////////////////////
        ///
        /// The time-symbol type indicates how to display a time signature. The normal value is the
        /// usual fractional display, and is the implied symbol type if none is specified. Other
        /// options are the common and cut time symbols, as well as a single number with an implied
        /// denominator. The note symbol indicates that the beat-type should be represented with the
        /// corresponding downstem note rather than a number. The dotted-note symbol indicates that
        /// the beat-type should be represented with a dotted downstem note that corresponds to
        /// three times the beat-type value, and a numerator that is one third the beats value.
        ///
        enum class TimeSymbol
        {
            common = 0,
            cut = 1,
            singleNumber = 2,
            note = 3,
            dottedNote = 4,
            normal = 5
        };

        TimeSymbol parseTimeSymbol( const std::string& value );
        std::optional<TimeSymbol> tryParseTimeSymbol( const std::string& value );
        std::string toString( const TimeSymbol value );
        std::ostream& toStream( std::ostream& os, const TimeSymbol value );
        std::ostream& operator<<( std::ostream& os, const TimeSymbol value );

        /// TipDirection ///////////////////////////////////////////////////////////////////////////
        ///
        /// The tip-direction type represents the direction in which the tip of a stick or beater
        /// points, using Unicode arrow terminology.
        ///
        enum class TipDirection
        {
            up = 0,
            down = 1,
            left = 2,
            right = 3,
            northwest = 4,
            northeast = 5,
            southeast = 6,
            southwest = 7
        };

        TipDirection parseTipDirection( const std::string& value );
        std::optional<TipDirection> tryParseTipDirection( const std::string& value );
        std::string toString( const TipDirection value );
        std::ostream& toStream( std::ostream& os, const TipDirection value );
        std::ostream& operator<<( std::ostream& os, const TipDirection value );

        /// TopBottom //////////////////////////////////////////////////////////////////////////////
        ///
        /// The top-bottom type is used to indicate the top or bottom part of a vertical shape like
        /// non-arpeggiate.
        ///
        enum class TopBottom
        {
            top = 0,
            bottom = 1
        };

        TopBottom parseTopBottom( const std::string& value );
        std::optional<TopBottom> tryParseTopBottom( const std::string& value );
        std::string toString( const TopBottom value );
        std::ostream& toStream( std::ostream& os, const TopBottom value );
        std::ostream& operator<<( std::ostream& os, const TopBottom value );

        /// TrillStep //////////////////////////////////////////////////////////////////////////////
        ///
        /// The trill-step type describes the alternating note of trills and mordents for playback,
        /// relative to the current note.
        ///
        enum class TrillStep
        {
            whole = 0,
            half = 1,
            unison = 2
        };

        TrillStep parseTrillStep( const std::string& value );
        std::optional<TrillStep> tryParseTrillStep( const std::string& value );
        std::string toString( const TrillStep value );
        std::ostream& toStream( std::ostream& os, const TrillStep value );
        std::ostream& operator<<( std::ostream& os, const TrillStep value );

        /// TwoNoteTurn ////////////////////////////////////////////////////////////////////////////
        ///
        /// The two-note-turn type describes the ending notes of trills and mordents for playback,
        /// relative to the current note.
        ///
        enum class TwoNoteTurn
        {
            whole = 0,
            half = 1,
            none = 2
        };

        TwoNoteTurn parseTwoNoteTurn( const std::string& value );
        std::optional<TwoNoteTurn> tryParseTwoNoteTurn( const std::string& value );
        std::string toString( const TwoNoteTurn value );
        std::ostream& toStream( std::ostream& os, const TwoNoteTurn value );
        std::ostream& operator<<( std::ostream& os, const TwoNoteTurn value );

        /// UpDown /////////////////////////////////////////////////////////////////////////////////
        ///
        /// The up-down type is used for the direction of arrows and other pointed symbols like
        /// vertical accents, indicating which way the tip is pointing.
        ///
        enum class UpDown
        {
            up = 0,
            down = 1
        };

        UpDown parseUpDown( const std::string& value );
        std::optional<UpDown> tryParseUpDown( const std::string& value );
        std::string toString( const UpDown value );
        std::ostream& toStream( std::ostream& os, const UpDown value );
        std::ostream& operator<<( std::ostream& os, const UpDown value );

        /// UpDownStopContinue /////////////////////////////////////////////////////////////////////
        ///
        /// The up-down-stop-continue type is used for octave-shift elements, indicating the
        /// direction of the shift from their true pitched values because of printing difficulty.
        ///
        enum class UpDownStopContinue
        {
            up = 0,
            down = 1,
            stop = 2,
            continue_ = 3
        };

        UpDownStopContinue parseUpDownStopContinue( const std::string& value );
        std::optional<UpDownStopContinue> tryParseUpDownStopContinue( const std::string& value );
        std::string toString( const UpDownStopContinue value );
        std::ostream& toStream( std::ostream& os, const UpDownStopContinue value );
        std::ostream& operator<<( std::ostream& os, const UpDownStopContinue value );

        /// UpDownNone /////////////////////////////////////////////////////////////////////
        ///
        /// The up-down-none type is used for octave-shift elements, indicating the
        /// direction of the shift from their true pitched values because of printing difficulty.
        ///
        enum class UpDownNone
        {
            up = 0,
            down = 1,
            none = 2
        };

        UpDownNone parseUpDownNone( const std::string& value );
        std::optional<UpDownNone> tryParseUpDownNone( const std::string& value );
        std::string toString( const UpDownNone value );
        std::ostream& toStream( std::ostream& os, const UpDownNone value );
        std::ostream& operator<<( std::ostream& os, const UpDownNone value );
    
        /// UprightInverted ////////////////////////////////////////////////////////////////////////
        ///
        /// The upright-inverted type describes the appearance of a fermata element. The value is
        /// upright if not specified.
        ///
        enum class UprightInverted
        {
            upright = 0,
            inverted = 1
        };

        UprightInverted parseUprightInverted( const std::string& value );
        std::optional<UprightInverted> tryParseUprightInverted( const std::string& value );
        std::string toString( const UprightInverted value );
        std::ostream& toStream( std::ostream& os, const UprightInverted value );
        std::ostream& operator<<( std::ostream& os, const UprightInverted value );

        /// Valign /////////////////////////////////////////////////////////////////////////////////
        ///
        /// The valign type is used to indicate vertical alignment to the top, middle, bottom, or
        /// baseline of the text. Defaults are implementation-dependent.
        ///
        enum class Valign
        {
            top = 0,
            middle = 1,
            bottom = 2,
            baseline = 3
        };

        Valign parseValign( const std::string& value );
        std::optional<Valign> tryParseValign( const std::string& value );
        std::string toString( const Valign value );
        std::ostream& toStream( std::ostream& os, const Valign value );
        std::ostream& operator<<( std::ostream& os, const Valign value );

        /// ValignImage ////////////////////////////////////////////////////////////////////////////
        ///
        /// The valign-image type is used to indicate vertical alignment for images and graphics, so
        /// it does not include a baseline value. Defaults are implementation-dependent.
        ///
        enum class ValignImage
        {
            top = 0,
            middle = 1,
            bottom = 2
        };

        ValignImage parseValignImage( const std::string& value );
        std::optional<ValignImage> tryParseValignImage( const std::string& value );
        std::string toString( const ValignImage value );
        std::ostream& toStream( std::ostream& os, const ValignImage value );
        std::ostream& operator<<( std::ostream& os, const ValignImage value );

        /// WedgeType //////////////////////////////////////////////////////////////////////////////
        ///
        /// The wedge type is crescendo for the start of a wedge that is closed at the left side,
        /// diminuendo for the start of a wedge that is closed on the right side, and stop for the
        /// end of a wedge. The continue type is used for formatting wedges over a system break, or
        /// for other situations where a single wedge is divided into multiple segments.
        ///
        enum class WedgeType
        {
            crescendo = 0,
            diminuendo = 1,
            stop = 2,
            continue_ = 3
        };

        WedgeType parseWedgeType( const std::string& value );
        std::optional<WedgeType> tryParseWedgeType( const std::string& value );
        std::string toString( const WedgeType value );
        std::ostream& toStream( std::ostream& os, const WedgeType value );
        std::ostream& operator<<( std::ostream& os, const WedgeType value );

        /// Winged /////////////////////////////////////////////////////////////////////////////////
        ///
        /// The winged attribute indicates whether the repeat has winged extensions that appear
        /// above and below the barline. The straight and curved values represent single wings,
        /// while the double-straight and double-curved values represent double wings. The none
        /// value indicates no wings and is the default.
        ///
        enum class Winged
        {
            none = 0,
            straight = 1,
            curved = 2,
            doubleStraight = 3,
            doubleCurved = 4
        };

        Winged parseWinged( const std::string& value );
        std::optional<Winged> tryParseWinged( const std::string& value );
        std::string toString( const Winged value );
        std::ostream& toStream( std::ostream& os, const Winged value );
        std::ostream& operator<<( std::ostream& os, const Winged value );

        /// WoodEnum ///////////////////////////////////////////////////////////////////////////////
        ///
        /// The wood type represents pictograms for wood percussion instruments. The maraca and
        /// maracas values distinguish the one- and two-maraca versions of the pictogram. The
        /// vibraslap and castanets values are in addition to Stone's list.
        ///
        enum class WoodEnum
        {
            boardClapper = 0,
            cabasa = 1,
            castanets = 2,
            claves = 3,
            guiro = 4,
            logDrum = 5,
            maraca = 6,
            maracas = 7,
            ratchet = 8,
            sandpaperBlocks = 9,
            slitDrum = 10,
            templeBlock = 11,
            vibraslap = 12,
            woodBlock = 13
        };

        WoodEnum parseWoodEnum( const std::string& value );
        std::optional<WoodEnum> tryParseWoodEnum( const std::string& value );
        std::string toString( const WoodEnum value );
        std::ostream& toStream( std::ostream& os, const WoodEnum value );
        std::ostream& operator<<( std::ostream& os, const WoodEnum value );

        /// YesNo //////////////////////////////////////////////////////////////////////////////////
        ///
        /// The yes-no type is used for boolean-like attributes. We cannot use W3C XML Schema
        /// booleans due to their restrictions on expression of boolean values.
        ///
        enum class YesNo
        {
            yes = 0,
            no = 1
        };

        YesNo parseYesNo( const std::string& value );
        std::optional<YesNo> tryParseYesNo( const std::string& value );
        std::string toString( const YesNo value );
        std::ostream& toStream( std::ostream& os, const YesNo value );
        std::ostream& operator<<( std::ostream& os, const YesNo value );
    }
}
