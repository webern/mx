// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ApiCommon.h"
#include "mx/api/SystemLayoutData.h"
#include "mx/api/MeasureData.h"

#include <map>

namespace mx
{
    namespace api
    {
        /// XXBadName represents the Page and System layout instructions that are found in the <print> element.
        /// These are held in the ScoreData in a map where the key is the measure index. For example, to start a new
        /// page at measure index 39: score.print.emplace( 39, XXBadName{ true, true } );
        /// Where the constructor XXBadName{ true, true } is a convenience constructor indicating that both a new system
        /// and a new page are indicated.
        class XXBadName
        {
        public:
            /// System information, such as whether a system break should occur. Note: all members are optional.
            SystemData system;

            /// Page information, such as whether a page break should occur. Note: all members are optional.
            PageData page;

            /// Convenience constructor for the use case where we only care about specifying system breaks and page
            /// breaks. That is, if you do not need to fuss with margins or spacing and only want to indicate the start
            /// of a new system or page, you can use this constructor. The first bool indicates that a new system should
            /// start. The second bool indicates that a new page should also start. For example:
            /// XXBadName{ true } produces <print new-system="yes" />
            /// XXBadName{ true, true } produces <print new-system="yes" new-page="yes" />
            inline explicit XXBadName( bool inNewSystem, bool inNewPage = false )
                : system{ inNewSystem ? Bool::yes : Bool::unspecified }
                , page{ inNewPage ? Bool::yes : Bool::unspecified }
            {

            }

            /// The default constructor does not specify any page or system details. i.e. a default constructed object
            /// will not produce a <print> element.
            inline XXBadName()
                : XXBadName{ SystemData{}, PageData{} }
            {

            }

            /// The explicit constructor takes fully constructed members.
            inline explicit XXBadName( SystemData inSystem, PageData inPage )
                : system{ std::move( inSystem ) }
                , page{ std::move( inPage ) }
            {

            }

            /// Tells us whether any members have been specified or not.
            inline bool isUsed() const { return system.isUsed() || page.isUsed(); }
        };

        MXAPI_EQUALS_BEGIN( XXBadName )
            MXAPI_EQUALS_MEMBER( system )
            MXAPI_EQUALS_MEMBER( page )
        MXAPI_EQUALS_END;
        MXAPI_NOT_EQUALS_AND_VECTORS( XXBadName );

        inline bool operator==( const std::map<MeasureIndex, XXBadName>& a, const std::map<MeasureIndex, XXBadName>& b )
        {
            if( a.size() != b.size() )
            {
                return false;
            }
            auto ai = a.cbegin();
            auto bi = b.cbegin();
            const auto aend = a.cend();
            const auto bend = b.cend();
            for( ; ai != aend && bi != bend; ++ai, ++bi )
            {
                if( ai->first != bi->first )
                {
                    return false;
                }
                if( ai->second != bi->second )
                {
                    return false;
                }
            }
            return true;
        }
    }
}
