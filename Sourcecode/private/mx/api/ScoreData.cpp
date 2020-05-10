// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/api/ScoreData.h"

#include <vector>
#include <set>

namespace mx
{
    namespace api
    {

        ScoreData::ScoreData()
        : musicXmlVersion{ api::MusicXmlVersion::unspecified }
        , musicXmlType{ "partwise" }
        , workTitle{}
        , workNumber{}
        , movementTitle{}
        , movementNumber{}
        , composer{}
        , lyricist{}
        , copyright{}
        , encoding{}
        , pageTextItems{}
        , layout{}
        , parts{}
        , partGroups{}
        , ticksPerQuarter{ DEFAULT_TICKS_PER_QUARTER }
        {

        }



        int ScoreData::getNumMeasures() const
        {
            int numMeasures = 0;

            for( const auto& part : parts )
            {
                int temp = static_cast<int>( part.measures.size() );
                if( temp > numMeasures )
                {
                    numMeasures = temp;
                }
            }

            return numMeasures;
        }



        int ScoreData::getNumStavesPerSystem() const
        {
            int numStaves = 0;

            for( const auto& p : parts )
            {
                numStaves += p.getNumStaves();
            }

            return numStaves;
        }


        void ScoreData::sort()
        {
            for( auto& part : parts )
            {
                for( auto& measure : part.measures )
                {
                    for ( auto& staff : measure.staves )
                    {

                        const auto clefCompare = [&]( ClefData& inLeft, ClefData& inRight )
                        {
                            return inLeft.tickTimePosition < inRight.tickTimePosition;
                        };

                        std::sort( std::begin( staff.clefs ), std::end( staff.clefs ), clefCompare );

                        const auto directionCompare = [&]( DirectionData& inLeft, DirectionData& inRight )
                        {
                            return inLeft.tickTimePosition < inRight.tickTimePosition;
                        };

                        std::sort( std::begin( staff.directions ), std::end( staff.directions ), directionCompare );

                        for ( auto& voice : staff.voices )
                        {
                            const auto noteCompare = [&]( NoteData& inLeft, NoteData& inRight )
                            {
                                return inLeft.tickTimePosition < inRight.tickTimePosition;
                            };

                            std::sort( std::begin( voice.second.notes ), std::end( voice.second.notes ), noteCompare );
                        }
                    }
                }
            }
        }

        auto findSupports( const ScoreData& inScore, const std::string& inElement, const std::string& inAttribute )
        {
            const auto finder = [&](const SupportedItem& item)
            {
                return item.elementName == inElement && item.attributeName == inAttribute;
            };

            return std::find_if( std::cbegin( inScore.encoding.supportedItems ), std::cend( inScore.encoding.supportedItems ), finder );
        }

        auto supportExists( const ScoreData& inScore, const std::string& inElement, const std::string& inAttribute )
        {
            return findSupports( inScore, inElement, inAttribute ) != std::cend( inScore.encoding.supportedItems );
        }

        void ScoreData::autoSetSupports()
        {
            if( !supportExists( *this, "print", "new-system" ) )
            {
                if( !systems.empty() )
                {
                    auto s = SupportedItem{};
                    s.elementName = "print";
                    s.attributeName = "new-system";
                    s.specificValue = "yes";
                    s.isSupported = true;
                }
            }

            if( !supportExists( *this, "beam", "" ) )
            {
                bool isBeamFound = false;
                for( const auto& part : parts )
                {
                    for( const auto& measure : part.measures )
                    {
                        for( const auto& staff : measure.staves )
                        {
                            for( const auto& voice : staff.voices )
                            {
                                for( const auto& note : voice.second.notes )
                                {
                                    if( !note.beams.empty() )
                                    {
                                        isBeamFound = true;
                                        break;
                                    }
                                }
                            }
                        }
                    }

                    if( isBeamFound )
                    {
                        auto s = SupportedItem{};
                        s.elementName = "beam";
                        s.isSupported = true;
                    }
                }
            }
        }


        void ScoreData::autoSetNotSupports()
        {
            if( !supportExists( *this, "print", "new-system" ) )
            {
                if( systems.empty() )
                {
                    auto s = SupportedItem{};
                    s.elementName = "print";
                    s.attributeName = "new-system";
                    s.specificValue = ""; // ?
                    s.isSupported = false;
                }
            }

            if( !supportExists( *this, "beam", "" ) )
            {
                bool isBeamFound = false;
                for( const auto& part : parts )
                {
                    for( const auto& measure : part.measures )
                    {
                        for( const auto& staff : measure.staves )
                        {
                            for( const auto& voice : staff.voices )
                            {
                                for( const auto& note : voice.second.notes )
                                {
                                    if( !note.beams.empty() )
                                    {
                                        isBeamFound = true;
                                        break;
                                    }
                                }
                            }
                        }
                    }

                    if( !isBeamFound )
                    {
                        auto s = SupportedItem{};
                        s.elementName = "beam";
                        s.isSupported = false;
                    }
                }
            }
        }

        std::vector<PartData>::iterator findPart( std::vector<PartData>& inParts, const std::string& inPartId )
        {
            auto predicate = [&inPartId] ( const PartData& partData )
            {
                return inPartId == partData.uniqueId;
            };

            auto it = std::find_if( inParts.begin(), inParts.end(), predicate );
            return it;
        }
    }
}
