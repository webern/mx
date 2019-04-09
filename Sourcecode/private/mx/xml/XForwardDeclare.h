// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

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
