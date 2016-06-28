// MusicXML Class Library v0.1.1
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once
#include "mx/core/DocumentPartwise.h"

namespace mx
{
	namespace utility
	{
        using MxDoc = core::DocumentPartwisePtr;
        //using core::StringType;
        
        
        void setTitle(
            const MxDoc& doc,
            const core::StringType& value );
        
        
        core::StringType getTitle( const MxDoc& doc );
        
        
        void setMovementTitle(
            const MxDoc& doc,
            const core::StringType& value );
        
        
        core::StringType getMovementTitle( const MxDoc& doc );
    
        
        void setMovementNumber(
            const MxDoc& doc,
            const int value );
        
        
        int getMovementNumber( const MxDoc& doc );
        
        
        void setComposer(
            const MxDoc& doc,
            const core::StringType& value );
        
        
        core::StringType getComposer( const MxDoc& doc );
        
        
        void setCopyright(
            const MxDoc& doc,
            const core::StringType& value );
        
        
        core::StringType getCopyright( const MxDoc& doc );
        
        
        void setSoftware(
            const MxDoc& doc,
            const core::StringType& value );
        
        
        core::StringType getSoftware( const MxDoc& doc );
        
        struct PartGroupParams
        {
            core::StringType name;
            core::StringType displayName;
            core::StringType abbreviation;
            core::StringType displayAbbreviation;
            int number;
            bool isGroupBarline;
            core::GroupSymbolValue groupSymbol;
            PartGroupParams( const core::StringType& inName ) : name( inName ), number( -1 ), isGroupBarline( true ), groupSymbol( core::GroupSymbolValue::bracket ) {}
            PartGroupParams() : number( -1 ), isGroupBarline( true ), groupSymbol( core::GroupSymbolValue::bracket ) {}
        };
        
        void startPartGroup(
             const MxDoc& doc,
             const PartGroupParams& params );
        
        void stopPartGroup(
            const MxDoc& doc,
            int number);
        
        void stopPartGroup( const MxDoc& doc );
        
        struct PartParams
        {
            core::StringType uniqueId;
            core::StringType name;
            core::StringType abbreviation;
            core::StringType displayName;
            core::StringType displayAbbreviation;
            core::StringType instrumentName;
            core::StringType instrumentAbbreviation;
            core::StringType instrumentUniqueId;
            core::StringType virtualLibrary;
            core::StringType virtualInstrument;
            int midiBank;
            int midiChannel;
            int midiProgram;
            int midiVolume;
            int midiPan;
            core::StringType midiName;
            PartParams()
                : uniqueId( "X" )
                , midiBank( INT_MIN )
                , midiChannel(INT_MIN )
                , midiProgram( INT_MIN )
                , midiVolume( INT_MIN )
                , midiPan( INT_MIN )
            {}
        };
        
        
        core::PartwisePartPtr addPart( const MxDoc& doc, const PartParams& params );
        
        core::PartwiseMeasurePtr createMeasure( int measureNumber );
        
        void addTimeSignature( const core::PartwiseMeasurePtr& measure, int top, int bottom );
        
        void addStaveCount( const core::PartwiseMeasurePtr& measure, int staveCount );
        
        void addDivisions( const core::PartwiseMeasurePtr& measure, int divisions );
        
        int getDivisions( const core::PartwiseMeasurePtr& measure );
        
        void addKeySignature( const core::PartwiseMeasurePtr& measure );
        
        enum class UpDown
        {
            unspecified,
            up,
            down
        };
        
        struct NoteParams
        {
            core::NoteChoice::Choice noteChoice;
            bool isRest;
            bool isChord;
            int step; // 0 = c, 1 = d, etc.
            int alter;
            int octave;
            int staffNumber;
            int voiceNumber;
            UpDown stemDirection;
            core::NoteTypeValue durationType;
            int duration;
            int durationDots;
            std::vector<core::BeamValue> beams;
            bool showAccidental;
            NoteParams()
                : noteChoice( core::NoteChoice::Choice::normal )
                , isRest( false )
                , isChord( false )
                , step( 0 )
                , alter( 0 )
                , octave( 4 )
                , staffNumber( 1 )
                , voiceNumber( 1 )
                , stemDirection( UpDown::unspecified )
                , durationType( core::NoteTypeValue::quarter )
                , duration( 1 )
                , durationDots( 0 )
                , beams()
                , showAccidental( false )
            {}
        };
        
        core::MusicDataChoicePtr createNote( const NoteParams& params );
    }
}
