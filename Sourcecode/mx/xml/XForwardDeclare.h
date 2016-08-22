// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

// TODO - delete this file if it is not used

#ifndef MX_XML_FORWARD_DECLARE_ELEMENT
#define MX_XML_FORWARD_DECLARE_ELEMENT \
namespace mx \
{ \
	namespace xml \
	{ \
		class XElement; \
		using XElementPtr ## Ptr = std::shared_ptr<XElement>; \
	} \
} \

#endif
