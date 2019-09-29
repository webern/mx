// Copyright (c) Matthew James Briggs

#pragma once

#include "ezxml/XDoc.h"
#include <memory>

namespace ezxml
{
    class XFactory
    {
    public:
        static XDocPtr makeXDoc();
        static XElementPtr makeXElement();
    };
}
