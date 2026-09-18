// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/api/MusicXml.h"
#include "mx/core/Attribution.h"
#include "mx/core/Error.h"
#include "mx/core/IdIntegrity.h"
#include "mx/core/ParseContext.h"
#include "mx/core/generated/Document.h"
#include "mx/impl/ScoreConversions.h"
#include "mx/impl/ScoreReader.h"
#include "mx/impl/ScoreWriter.h"
#include "mx/impl/WriteRefusal.h"

#include "pugixml.hpp"

#include <sstream>

namespace mx
{
namespace api
{
// mx's data model is MusicXML 4.0, so every write states version="4.0" on
// the root element, whatever version a parsed document declared. The
// override happens here on a copy at the write boundary, so the owned
// document keeps what was parsed.
core::Document withWriteVersion(const core::Document &document)
{
    core::Document copy = document;
    if (copy.isScorePartwise())
    {
        core::ScorePartwise root = copy.asScorePartwise();
        root.setVersion(std::string{"4.0"});
        copy.setRoot(core::Document::Root{std::move(root)});
    }
    else
    {
        core::ScoreTimewise root = copy.asScoreTimewise();
        root.setVersion(std::string{"4.0"});
        copy.setRoot(core::Document::Root{std::move(root)});
    }
    return copy;
}

// mx::api owns its error vocabulary; the core parse codes are mirrored
// one-to-one.
ResultCode mirrorToApiResultCode(core::ErrorCode code)
{
    switch (code)
    {
    case core::ErrorCode::unknownElement:
        return ResultCode::unknownElement;
    case core::ErrorCode::unknownAttribute:
        return ResultCode::unknownAttribute;
    case core::ErrorCode::missingRequiredElement:
        return ResultCode::missingRequiredElement;
    case core::ErrorCode::missingRequiredAttribute:
        return ResultCode::missingRequiredAttribute;
    case core::ErrorCode::wrongElementOrder:
        return ResultCode::wrongElementOrder;
    case core::ErrorCode::tooManyElements:
        return ResultCode::tooManyElements;
    case core::ErrorCode::invalidDocument:
        return ResultCode::invalidDocument;
    case core::ErrorCode::unsupportedVersion:
        return ResultCode::unsupportedVersion;
    default:
        return ResultCode::internalError;
    }
}

ApiError mirrorToApiError(const core::Error &error)
{
    Location location;
    location.xmlPath = error.path;
    return ApiError{mirrorToApiResultCode(error.code), location, error.message};
}

DiagnosticCode mirrorToApiDiagnosticCode(core::DiagnosticCode code)
{
    switch (code)
    {
    case core::DiagnosticCode::invalidValue:
        return DiagnosticCode::invalidValue;
    case core::DiagnosticCode::valueAdjusted:
        return DiagnosticCode::valueAdjusted;
    case core::DiagnosticCode::missingValueDefaulted:
        return DiagnosticCode::missingValueDefaulted;
    case core::DiagnosticCode::duplicateId:
        return DiagnosticCode::duplicateId;
    case core::DiagnosticCode::danglingIdReference:
        return DiagnosticCode::danglingIdReference;
    }
    return DiagnosticCode::invalidValue;
}

// Every repair leaves a usable document, so each one is a warning.
core::DiagnosticHandler handlerReportingTo(Diagnostics &diagnostics)
{
    return [&diagnostics](const core::Diagnostic &diagnostic) {
        Location location;
        location.xmlPath = diagnostic.path;
        diagnostics.add(Diagnostic{Severity::warning, mirrorToApiDiagnosticCode(diagnostic.code), std::move(location),
                                   diagnostic.message});
    };
}

core::ParseContext parseContextReportingTo(Diagnostics &diagnostics)
{
    return core::ParseContext{handlerReportingTo(diagnostics)};
}

// Builds the error for a caught exception. Call it inside a catch block only:
// current_exception() keeps the exception alive in `cause`, so a caller can
// look at it or throw it again. Most entry points repeat the same three
// catches (out of memory, known exception, anything else).
ApiError caughtError(const char *function, ResultCode code, const std::string &message)
{
    return ApiError{code, Location{}, std::string{function} + ": " + message, std::current_exception()};
}

std::string musicXmlFileExtension(const std::string &filePath)
{
    const auto dotPos = filePath.find_last_of('.');
    if (dotPos == std::string::npos || dotPos == filePath.size() - 1)
    {
        return std::string{""};
    }
    return filePath.substr(dotPos + 1);
}

class MusicXml::Impl
{
  public:
    // writeMxVersion governs whether writeTo*() stamps mx's provenance
    // <software> (see EncodingData::writeMxVersion); it defaults true,
    // including for parsed documents (whose source did not have the stamp).
    Impl() = default;

    Impl(core::Document inDocument, bool inWriteMxVersion)
        : document{std::move(inDocument)}, writeMxVersion{inWriteMxVersion}
    {
    }

    // The natural zero: a default-constructed ScorePartwise, i.e. a valid,
    // empty document.
    core::Document document;
    bool writeMxVersion = true;
};

MusicXml::MusicXml() : myImpl{new MusicXml::Impl{}}
{
}

MusicXml::MusicXml(core::Document document, bool writeMxVersion)
    : myImpl{new MusicXml::Impl{std::move(document), writeMxVersion}}
{
}

// The move operations never leave a null pimpl behind. A moved-from MusicXml
// holds a valid, empty document, so it is safe to read and write. The
// constructor allocates the empty replacement first, so it is not noexcept;
// the assignment just swaps and is.
MusicXml::MusicXml(MusicXml &&other) : myImpl{std::make_unique<MusicXml::Impl>()}
{
    std::swap(myImpl, other.myImpl);
}

MusicXml &MusicXml::operator=(MusicXml &&other) noexcept
{
    if (this != &other)
    {
        std::swap(myImpl, other.myImpl);
    }
    return *this;
}

MusicXml::~MusicXml()
{
}

Result<MusicXml> MusicXml::fromFile(const std::string &filePath)
{
    Diagnostics diagnostics;
    return fromFile(filePath, diagnostics);
}

Result<MusicXml> MusicXml::fromFile(const std::string &filePath, Diagnostics &diagnostics)
{
    try
    {
        pugi::xml_document xdoc;
        const pugi::xml_parse_result loaded =
            xdoc.load_file(filePath.c_str(), pugi::parse_default | pugi::parse_doctype);
        if (!loaded)
        {
            if (loaded.status == pugi::status_file_not_found || loaded.status == pugi::status_io_error ||
                loaded.status == pugi::status_out_of_memory)
            {
                return ApiError{ResultCode::ioError, Location{},
                                "could not read '" + filePath + "' (" + loaded.description() + ")"};
            }
            Location location;
            location.byteOffset = loaded.offset;
            if (musicXmlFileExtension(filePath) == "mxl")
            {
                std::stringstream ss;
                ss << "it looks like you are trying to parse a compressed musicxml file, which is currently "
                   << "unsupported. https://github.com/webern/mx/issues/66 (" << loaded.description() << ")";
                return ApiError{ResultCode::xmlSyntaxError, location, ss.str()};
            }
            return ApiError{ResultCode::xmlSyntaxError, location, loaded.description()};
        }

        auto parsed = core::parse(xdoc, parseContextReportingTo(diagnostics));
        if (!parsed)
        {
            return mirrorToApiError(parsed.error());
        }

        // After the parse, so that a repaired id is reported before any
        // collision the repair caused.
        core::checkIds(xdoc, handlerReportingTo(diagnostics));

        return MusicXml{core::Document{std::move(parsed).value()}, true};
    }
    catch (const std::bad_alloc &)
    {
        return caughtError("MusicXml::fromFile", ResultCode::outOfMemory, "out of memory");
    }
    catch (const std::exception &e)
    {
        return caughtError("MusicXml::fromFile", ResultCode::internalError, e.what());
    }
    catch (...)
    {
        return caughtError("MusicXml::fromFile", ResultCode::internalError, "unknown exception");
    }
}

Result<MusicXml> MusicXml::fromStream(std::istream &stream)
{
    Diagnostics diagnostics;
    return fromStream(stream, diagnostics);
}

Result<MusicXml> MusicXml::fromStream(std::istream &stream, Diagnostics &diagnostics)
{
    try
    {
        pugi::xml_document xdoc;
        const pugi::xml_parse_result loaded = xdoc.load(stream, pugi::parse_default | pugi::parse_doctype);
        if (!loaded)
        {
            Location location;
            location.byteOffset = loaded.offset;
            return ApiError{ResultCode::xmlSyntaxError, location, loaded.description()};
        }

        auto parsed = core::parse(xdoc, parseContextReportingTo(diagnostics));
        if (!parsed)
        {
            return mirrorToApiError(parsed.error());
        }

        // After the parse, so that a repaired id is reported before any
        // collision the repair caused.
        core::checkIds(xdoc, handlerReportingTo(diagnostics));

        return MusicXml{core::Document{std::move(parsed).value()}, true};
    }
    catch (const std::bad_alloc &)
    {
        return caughtError("MusicXml::fromStream", ResultCode::outOfMemory, "out of memory");
    }
    catch (const std::exception &e)
    {
        return caughtError("MusicXml::fromStream", ResultCode::internalError, e.what());
    }
    catch (...)
    {
        return caughtError("MusicXml::fromStream", ResultCode::internalError, "unknown exception");
    }
}

Result<void> MusicXml::writeToFile(const std::string &filePath) const
{
    Diagnostics diagnostics;
    return writeToFile(filePath, diagnostics);
}

Result<void> MusicXml::writeToFile(const std::string &filePath, Diagnostics &diagnostics) const
{
    try
    {
        pugi::xml_document xdoc;
        const core::Document toWrite = withWriteVersion(myImpl->document);
        if (myImpl->writeMxVersion)
        {
            core::serializeWithAttribution(toWrite, xdoc);
        }
        else
        {
            core::serialize(toWrite, xdoc);
        }
        core::repairIds(xdoc, handlerReportingTo(diagnostics));
        if (!xdoc.save_file(filePath.c_str(), "  "))
        {
            return ApiError{ResultCode::ioError, Location{}, "writeToFile: could not write '" + filePath + "'"};
        }
        return Result<void>{};
    }
    catch (const std::bad_alloc &)
    {
        return caughtError("MusicXml::writeToFile", ResultCode::outOfMemory, "out of memory");
    }
    catch (const std::exception &e)
    {
        return caughtError("MusicXml::writeToFile", ResultCode::internalError, e.what());
    }
    catch (...)
    {
        return caughtError("MusicXml::writeToFile", ResultCode::internalError, "unknown exception");
    }
}

Result<void> MusicXml::writeToStream(std::ostream &stream) const
{
    Diagnostics diagnostics;
    return writeToStream(stream, diagnostics);
}

Result<void> MusicXml::writeToStream(std::ostream &stream, Diagnostics &diagnostics) const
{
    try
    {
        pugi::xml_document xdoc;
        const core::Document toWrite = withWriteVersion(myImpl->document);
        if (myImpl->writeMxVersion)
        {
            core::serializeWithAttribution(toWrite, xdoc);
        }
        else
        {
            core::serialize(toWrite, xdoc);
        }
        core::repairIds(xdoc, handlerReportingTo(diagnostics));
        xdoc.save(stream, "  ");
        return Result<void>{};
    }
    catch (const std::bad_alloc &)
    {
        return caughtError("MusicXml::writeToStream", ResultCode::outOfMemory, "out of memory");
    }
    catch (const std::exception &e)
    {
        return caughtError("MusicXml::writeToStream", ResultCode::internalError, e.what());
    }
    catch (...)
    {
        return caughtError("MusicXml::writeToStream", ResultCode::internalError, "unknown exception");
    }
}

MusicXml MusicXml::clone() const
{
    MusicXml cloned{};
    cloned.myImpl->document = myImpl->document;
    cloned.myImpl->writeMxVersion = myImpl->writeMxVersion;
    return cloned;
}

core::Document &MusicXml::getCoreDocument()
{
    return myImpl->document;
}

const core::Document &MusicXml::getCoreDocument() const
{
    return myImpl->document;
}

Result<MusicXml> fromScore(const ScoreData &score)
{
    Diagnostics diagnostics;
    return fromScore(score, diagnostics);
}

Result<MusicXml> fromScore(const ScoreData &score, Diagnostics &diagnostics)
{
    try
    {
        const impl::DiagnosticsContext context{diagnostics};
        impl::ScoreWriter writer{score, context};
        core::ScorePartwise scorePartwise = writer.getScorePartwise();

        if (score.musicXmlType == "timewise")
        {
            return MusicXml{core::Document{impl::partwiseTimewise(scorePartwise, context)},
                            score.encoding.writeMxVersion};
        }

        return MusicXml{core::Document{std::move(scorePartwise)}, score.encoding.writeMxVersion};
    }
    catch (const impl::WriteRefusal &refusal)
    {
        // Refuse, don't drop: the ScoreData describes something the core
        // model will not represent.
        return refusal.error();
    }
    catch (const std::bad_alloc &)
    {
        return caughtError("fromScore", ResultCode::outOfMemory, "out of memory");
    }
    catch (const std::exception &e)
    {
        return caughtError("fromScore", ResultCode::internalError, e.what());
    }
    catch (...)
    {
        return caughtError("fromScore", ResultCode::internalError, "unknown exception");
    }
}

Result<ScoreData> getScore(const MusicXml &document)
{
    Diagnostics diagnostics;
    return getScore(document, diagnostics);
}

Result<ScoreData> getScore(const MusicXml &document, Diagnostics &diagnostics)
{
    try
    {
        const core::Document &coreDocument = document.myImpl->document;

        // Convert a timewise document into a local partwise copy and read
        // that; the owned document is untouched.
        if (coreDocument.isScoreTimewise())
        {
            const impl::DiagnosticsContext context{diagnostics};
            const core::ScorePartwise scorePartwise = impl::timewisePartwise(coreDocument.asScoreTimewise(), context);
            impl::ScoreReader reader{scorePartwise, context};
            auto score = reader.getScoreData();
            score.musicXmlType = "timewise";
            return score;
        }

        impl::ScoreReader reader{coreDocument.asScorePartwise(), impl::DiagnosticsContext{diagnostics}};
        return reader.getScoreData();
    }
    catch (const std::bad_alloc &)
    {
        return caughtError("getScore", ResultCode::outOfMemory, "out of memory");
    }
    catch (const std::exception &e)
    {
        return caughtError("getScore", ResultCode::internalError, e.what());
    }
    catch (...)
    {
        return caughtError("getScore", ResultCode::internalError, "unknown exception");
    }
}

Result<ScoreData> intoScore(MusicXml document)
{
    Diagnostics diagnostics;
    return intoScore(std::move(document), diagnostics);
}

Result<ScoreData> intoScore(MusicXml document, Diagnostics &diagnostics)
{
    // the parameter owns the document; its destructor frees the underlying
    // tree when this function returns
    return getScore(document, diagnostics);
}
} // namespace api
} // namespace mx
