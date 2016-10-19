// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include "mx/api/DirectionData.h"
#include "mx/impl/Cursor.h"

#include <mutex>
#include <vector>

namespace mx
{
	namespace core
	{
		class Direction;
        class DirectionType;
        using DirectionTypePtr = std::shared_ptr<DirectionType>;
        using DirectionTypeSet = std::vector<DirectionTypePtr>;
	}
    namespace impl
    {
    	class DirectionReader
    	{
    	public:
            DirectionReader( const core::Direction& inDirection, const core::DirectionType& inDirectionType, Cursor inCursor );
            bool getIsMark() const;
            int getStaffIndex() const;
            api::MarkData getMarkData() const;
            
    	private:
            const core::Direction& myDirection;
            const core::DirectionType& myDirectionType;
            const Cursor myCursor;
            int myStaffIndex;
            bool myIsMark;
            bool myIsSpanner;
            bool myIsSpecial;
            bool myIsInvalid;
            api::MarkData myOutMarkData;
            
    	private:
            void initialize();
            void parseStaffIndex();
            void parseRehearsal( const core::DirectionType& directionType ) const;
            void parseSegno( const core::DirectionType& directionType ) const;
            void parseWords( const core::DirectionType& directionType ) const;
            void parseCoda( const core::DirectionType& directionType ) const;
            void parseWedge( const core::DirectionType& directionType ) const;
            void parseDynamics( const core::DirectionType& directionType ) const;
            void parseDashes( const core::DirectionType& directionType ) const;
            void parseBracket( const core::DirectionType& directionType ) const;
            void parsePedal( const core::DirectionType& directionType ) const;
            void parseMetronome( const core::DirectionType& directionType ) const;
            void parseOctaveShift( const core::DirectionType& directionType ) const;
            void parseHarpPedals( const core::DirectionType& directionType ) const;
            void parseDamp( const core::DirectionType& directionType ) const;
            void parseDampAll( const core::DirectionType& directionType ) const;
            void parseEyeglasses( const core::DirectionType& directionType ) const;
            void parseStringMute( const core::DirectionType& directionType ) const;
            void parseScordatura( const core::DirectionType& directionType ) const;
            void parseImage( const core::DirectionType& directionType ) const;
            void parsePrincipalVoice( const core::DirectionType& directionType ) const;
            void parseAccordionRegistration( const core::DirectionType& directionType ) const;
            void parsePercussion( const core::DirectionType& directionType ) const;
            void parseOtherDirection( const core::DirectionType& directionType ) const;
    	};
	}
}
