// Copyright (c) Matthew James Briggs

#pragma once

#ifndef EZXML_XML_FORWARD_DECLARE_ELEMENT
#define EZXML_XML_FORWARD_DECLARE_ELEMENT \
namespace ezxml \
{ \
    class XElement; \
    using XElementPtr ## Ptr = std::shared_ptr<XElement>; \
} \

#endif
