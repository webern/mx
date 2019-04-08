// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

namespace mx
{
    namespace utility
    {
        template<typename INPUT, typename OUTPUT>
        inline OUTPUT roundTo( INPUT inValue )
        {
            return static_cast<OUTPUT>( std::ceil( inValue - static_cast<INPUT>( 0.5 ) ) );
        }
    }
}
