// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/api/MusicXml.h"
#include "mx/core/Attribution.h"
#include "mx/core/Error.h"
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
// The write side always emits version="4.0" unconditionally: echoing a
// declared "3.0" (or ScoreData::musicXmlVersion) from a 4.0 model was a
// fiction. Enforced here at the write boundary on a copy, so the owned
// document (and the getCoreDocument escape hatch) keeps what was parsed.
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
    return ApiError{mirrorToApiResultCode(error.code), error.path, error.message};
}

ApiError musicXmlInternalError(const char *function, const std::string &message)
{
    return ApiError{ResultCode::internalError, "", std::string{function} + ": " + message};
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
    // including for parsed documents (whose source never had the stamp).
    Impl(core::DocumentPtr inDocument, bool inWriteMxVersion)
        : document{std::move(inDocument)}, writeMxVersion{inWriteMxVersion}
    {
    }

    core::DocumentPtr document;
    bool writeMxVersion;
};

MusicXml::MusicXml(core::DocumentPtr &&coreDocument, bool writeMxVersion)
    : myImpl{new MusicXml::Impl{std::move(coreDocument), writeMxVersion}}
{
}

MusicXml::MusicXml(MusicXml &&other) noexcept : myImpl{std::move(other.myImpl)}
{
}

MusicXml &MusicXml::operator=(MusicXml &&other) noexcept
{
    myImpl = std::move(other.myImpl);
    return *this;
}

MusicXml::~MusicXml()
{
}

Result<MusicXml> MusicXml::fromFile(const std::string &filePath)
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
                return ApiError{ResultCode::ioError, filePath, loaded.description()};
            }
            if (musicXmlFileExtension(filePath) == "mxl")
            {
                std::stringstream ss;
                ss << "it looks like you are trying to parse a compressed musicxml file, which is currently "
                   << "unsupported. https://github.com/webern/mx/issues/66 (" << loaded.description() << ")";
                return ApiError{ResultCode::xmlSyntaxError, filePath, ss.str()};
            }
            return ApiError{ResultCode::xmlSyntaxError, filePath, loaded.description()};
        }

        auto parsed = core::parse(xdoc);
        if (!parsed)
        {
            return mirrorToApiError(parsed.error());
        }

        core::DocumentPtr mxdoc = std::make_shared<core::Document>(std::move(parsed).value());
        return MusicXml{std::move(mxdoc), true};
    }
    catch (const std::exception &e)
    {
        return musicXmlInternalError("MusicXml::fromFile", e.what());
    }
    catch (...)
    {
        return musicXmlInternalError("MusicXml::fromFile", "unknown exception");
    }
}

Result<MusicXml> MusicXml::fromStream(std::istream &stream)
{
    try
    {
        pugi::xml_document xdoc;
        const pugi::xml_parse_result loaded = xdoc.load(stream, pugi::parse_default | pugi::parse_doctype);
        if (!loaded)
        {
            return ApiError{ResultCode::xmlSyntaxError, "", loaded.description()};
        }

        auto parsed = core::parse(xdoc);
        if (!parsed)
        {
            return mirrorToApiError(parsed.error());
        }

        core::DocumentPtr mxdoc = std::make_shared<core::Document>(std::move(parsed).value());
        return MusicXml{std::move(mxdoc), true};
    }
    catch (const std::exception &e)
    {
        return musicXmlInternalError("MusicXml::fromStream", e.what());
    }
    catch (...)
    {
        return musicXmlInternalError("MusicXml::fromStream", "unknown exception");
    }
}

Result<void> MusicXml::writeToFile(const std::string &filePath) const
{
    try
    {
        if (!myImpl)
        {
            return musicXmlInternalError("MusicXml::writeToFile", "the document has been moved from");
        }

        pugi::xml_document xdoc;
        const core::Document toWrite = withWriteVersion(*myImpl->document);
        if (myImpl->writeMxVersion)
        {
            core::serializeWithAttribution(toWrite, xdoc);
        }
        else
        {
            core::serialize(toWrite, xdoc);
        }
        if (!xdoc.save_file(filePath.c_str(), "  "))
        {
            return ApiError{ResultCode::ioError, filePath, "writeToFile: could not write the file"};
        }
        return Result<void>{};
    }
    catch (const std::exception &e)
    {
        return musicXmlInternalError("MusicXml::writeToFile", e.what());
    }
    catch (...)
    {
        return musicXmlInternalError("MusicXml::writeToFile", "unknown exception");
    }
}

Result<void> MusicXml::writeToStream(std::ostream &stream) const
{
    try
    {
        if (!myImpl)
        {
            return musicXmlInternalError("MusicXml::writeToStream", "the document has been moved from");
        }

        pugi::xml_document xdoc;
        const core::Document toWrite = withWriteVersion(*myImpl->document);
        if (myImpl->writeMxVersion)
        {
            core::serializeWithAttribution(toWrite, xdoc);
        }
        else
        {
            core::serialize(toWrite, xdoc);
        }
        xdoc.save(stream, "  ");
        return Result<void>{};
    }
    catch (const std::exception &e)
    {
        return musicXmlInternalError("MusicXml::writeToStream", e.what());
    }
    catch (...)
    {
        return musicXmlInternalError("MusicXml::writeToStream", "unknown exception");
    }
}

const core::Document &MusicXml::getCoreDocument() const
{
    if (myImpl)
    {
        return *myImpl->document;
    }
    // a moved-from document holds nothing; reading it yields an empty core
    // document rather than a crash
    static const core::Document emptyDocument{};
    return emptyDocument;
}

Result<MusicXml> fromScore(const ScoreData &score)
{
    try
    {
        impl::ScoreWriter writer{score};
        core::ScorePartwise scorePartwise = writer.getScorePartwise();

        core::DocumentPtr mxdoc;
        if (score.musicXmlType == "timewise")
        {
            mxdoc = std::make_shared<core::Document>(impl::partwiseTimewise(scorePartwise));
        }
        else
        {
            mxdoc = std::make_shared<core::Document>(std::move(scorePartwise));
        }

        return MusicXml{std::move(mxdoc), score.encoding.writeMxVersion};
    }
    catch (const impl::WriteRefusal &refusal)
    {
        // Refuse, don't drop: the ScoreData describes something the core
        // model will not represent.
        return refusal.error();
    }
    catch (const std::exception &e)
    {
        return musicXmlInternalError("fromScore", e.what());
    }
    catch (...)
    {
        return musicXmlInternalError("fromScore", "unknown exception");
    }
}

Result<ScoreData> getScore(const MusicXml &document)
{
    try
    {
        if (!document.myImpl)
        {
            return musicXmlInternalError("getScore", "the document has been moved from");
        }

        const core::Document &coreDocument = *document.myImpl->document;

        // Convert a timewise document into a local partwise copy and read
        // that; the owned document is untouched.
        if (coreDocument.isScoreTimewise())
        {
            const core::ScorePartwise scorePartwise = impl::timewisePartwise(coreDocument.asScoreTimewise());
            impl::ScoreReader reader{scorePartwise};
            auto score = reader.getScoreData();
            score.musicXmlType = "timewise";
            return score;
        }

        impl::ScoreReader reader{coreDocument.asScorePartwise()};
        return reader.getScoreData();
    }
    catch (const std::exception &e)
    {
        return musicXmlInternalError("getScore", e.what());
    }
    catch (...)
    {
        return musicXmlInternalError("getScore", "unknown exception");
    }
}

Result<ScoreData> intoScore(MusicXml document)
{
    // the parameter owns the document; its destructor frees the underlying
    // tree when this function returns
    return getScore(document);
}
} // namespace api
} // namespace mx
