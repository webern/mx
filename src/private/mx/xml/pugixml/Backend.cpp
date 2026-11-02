// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/xml/pugixml/Backend.h"

#include <istream>
#include <ostream>

namespace mx::xml
{

// mx reads and writes the DOCTYPE, so every load keeps it as a node.
constexpr unsigned kPugiParseFlags = pugixml_ns::parse_default | pugixml_ns::parse_doctype;

ParseResult pugiParseResult(const pugixml_ns::xml_parse_result &result)
{
    ParseResult out;
    switch (result.status)
    {
    case pugixml_ns::status_ok:
        out.status = ParseStatus::ok;
        break;
    case pugixml_ns::status_file_not_found:
        out.status = ParseStatus::fileNotFound;
        break;
    case pugixml_ns::status_io_error:
        out.status = ParseStatus::ioError;
        break;
    case pugixml_ns::status_out_of_memory:
        out.status = ParseStatus::outOfMemory;
        break;
    default:
        out.status = ParseStatus::malformed;
        break;
    }
    out.offset = result.offset;
    out.description = result.description();
    return out;
}

unsigned pugiSaveFlags(SaveFlags flags)
{
    unsigned out = pugixml_ns::format_default;
    if (hasFlag(flags, SaveFlags::raw))
    {
        out |= pugixml_ns::format_raw;
    }
    if (hasFlag(flags, SaveFlags::noDeclaration))
    {
        out |= pugixml_ns::format_no_declaration;
    }
    return out;
}

ParseResult Backend::loadFile(DocumentHandle &document, const char *path)
{
    return pugiParseResult(document.load_file(path, kPugiParseFlags));
}

ParseResult Backend::loadString(DocumentHandle &document, std::string_view xml)
{
    return pugiParseResult(document.load_buffer(xml.data(), xml.size(), kPugiParseFlags));
}

ParseResult Backend::load(DocumentHandle &document, std::istream &in)
{
    return pugiParseResult(document.load(in, kPugiParseFlags));
}

bool Backend::saveFile(const DocumentHandle &document, const char *path, const char *indent, SaveFlags flags)
{
    return document.save_file(path, indent, pugiSaveFlags(flags));
}

void Backend::save(const DocumentHandle &document, std::ostream &out, const char *indent, SaveFlags flags)
{
    document.save(out, indent, pugiSaveFlags(flags));
}

} // namespace mx::xml
