// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/PageTextData.h"

namespace mx
{
    namespace core
    {
        class ScoreHeaderGroup;
    }

    namespace impl
    {
        void createPageTextItems( const std::vector<api::PageTextData>& inPageTextItems, core::ScoreHeaderGroup& outHeader );
        void createPageTextItems( const core::ScoreHeaderGroup& inHeader, std::vector<api::PageTextData>& outPageTextItems );
    }
}
