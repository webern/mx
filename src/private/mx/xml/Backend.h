// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// Chooses the XML backend. The header named by MX_XML_BACKEND_HEADER must
// define `struct mx::xml::Backend` satisfying the XmlBackend concept in
// Concept.h. Without a definition from the build, mx uses its pugixml
// adapter, which in turn compiles against mx's vendored copy of pugixml.
// See docs/ai/design/xml-backend.md.

#pragma once

#ifndef MX_XML_BACKEND_HEADER
#define MX_XML_BACKEND_HEADER "mx/xml/pugixml/Backend.h"
#endif

#include MX_XML_BACKEND_HEADER
