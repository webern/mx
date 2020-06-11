// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/DefaultsData.h"
#include "mx/api/EncodingData.h"
#include "mx/api/LayoutData.h"
#include "mx/api/PageData.h"
#include "mx/api/PageTextData.h"
#include "mx/api/PartData.h"
#include "mx/api/PartGroupData.h"
#include "mx/api/SystemData.h"

#include <map>
#include <vector>

namespace mx
{
    namespace api
    {
        enum class MusicXmlVersion
        {
             unspecified,
             ThreePointZero,
        };

        class ScoreData
        {
        public:
            ScoreData();
            MusicXmlVersion musicXmlVersion;
            std::string musicXmlType;
            std::string workTitle;
            std::string workNumber;
            std::string movementTitle;
            std::string movementNumber;
            std::string composer;
            std::string lyricist;
            std::string arranger;
            std::string publisher;
            std::string copyright;
            EncodingData encoding;
            std::vector<PageTextData> pageTextItems;

            /// Specifies the scaling factor (`tenths` and `millimeters`), the default page sizes and margins, and the
            /// default system spacing and margins. Corresponds to the `<defaults>` MusicXML element. (Note this field
            /// used to be named `layout` and was renamed to `defaults` in v0.5.0.)
            DefaultsData defaults;
            std::vector<PartData> parts;
            std::vector<PartGroupData> partGroups;
            int ticksPerQuarter;

            /// Specifies page breaks, system breaks, and changes to system and page layout. Global/default page and
            /// system layout settings should be set in `defaults`. MeasureIndex is the index of the measure in which
            /// the layout/break is applied. For example, a newPage at MeasureIndex 10 indicates that measure index 10
            /// will be the first measure on a new page. `layout` corresponds to the `<print>` MusicXML element. (Note:
            /// prior to v0.5.0 we have `std::set<SystemData> systems`. The `layout` map replaces `systems`.)
            std::map<MeasureIndex, LayoutData> layout;

            int getNumMeasures() const;
            int getNumStavesPerSystem() const;

            /// sorts all of the events, directions, etc.
            /// it is good to call this before writing to xml.
            void sort();
        };
        
        std::vector<PartData>::iterator findPart( std::vector<PartData>& inParts, const std::string& inPartId );
        
        MXAPI_EQUALS_BEGIN( ScoreData )
        MXAPI_EQUALS_MEMBER( musicXmlVersion )
        MXAPI_EQUALS_MEMBER( musicXmlType )
        MXAPI_EQUALS_MEMBER( workTitle )
        MXAPI_EQUALS_MEMBER( workNumber )
        MXAPI_EQUALS_MEMBER( movementTitle )
        MXAPI_EQUALS_MEMBER( movementNumber )
        MXAPI_EQUALS_MEMBER( composer )
        MXAPI_EQUALS_MEMBER( lyricist )
        MXAPI_EQUALS_MEMBER( arranger )
        MXAPI_EQUALS_MEMBER( publisher )
        MXAPI_EQUALS_MEMBER( copyright )
        MXAPI_EQUALS_MEMBER( encoding )
        MXAPI_EQUALS_MEMBER( pageTextItems )
        MXAPI_EQUALS_MEMBER( defaults )
        MXAPI_EQUALS_MEMBER( parts )
        MXAPI_EQUALS_MEMBER( partGroups )
        MXAPI_EQUALS_MEMBER( ticksPerQuarter )
        MXAPI_EQUALS_MEMBER( layout )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( ScoreData );
    }
}
