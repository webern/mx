// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/api/DocumentManager.h"
#include "mx/core/Error.h"
#include "mx/core/generated/Document.h"
#include "mx/impl/ScoreConversions.h"
#include "mx/impl/ScoreReader.h"
#include "mx/impl/ScoreWriter.h"
#include "mx/impl/WriteRefusal.h"

#include "pugixml/pugixml.hpp"

#include <map>
#include <mutex>
#include <sstream>

#define LOCK_DOCUMENT_MANAGER std::lock_guard<std::mutex> lock(myImpl->myMutex);

namespace mx
{
namespace api
{
using DocumentMap = std::map<int, mx::core::DocumentPtr>;

namespace
{

// mx::api owns its error vocabulary; the core parse codes are mirrored
// one-to-one.
ResultCode mirror(core::ErrorCode code)
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

ApiError mirror(const core::Error &error)
{
    return ApiError{mirror(error.code), error.path, error.message};
}

ApiError internalError(const char *function, const std::string &message)
{
    return ApiError{ResultCode::internalError, "", std::string{function} + ": " + message};
}

std::string fileExtension(const std::string &filePath)
{
    const auto dotPos = filePath.find_last_of('.');
    if (dotPos == std::string::npos || dotPos == filePath.size() - 1)
    {
        return std::string{""};
    }
    return filePath.substr(dotPos + 1);
}

// The write side always emits version="4.0" unconditionally: echoing a
// declared "3.0" (or ScoreData::musicXmlVersion) from a 4.0 model was a
// fiction. Enforced here at the write boundary on a copy, so the stored
// document (and the getDocument escape hatch) keeps what was parsed.
core::Document withWriteVersion(const core::Document &stored)
{
    core::Document copy = stored;
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

} // namespace

class DocumentManager::Impl
{
  public:
    std::mutex myMutex;
    int myCurrentId;
    DocumentMap myMap;
    int myCurrentUniqueId;

    Impl() : myMutex{}, myCurrentId{0}, myMap{}, myCurrentUniqueId{1000000}
    {
    }
};

DocumentManager::DocumentManager() : myImpl{new DocumentManager::Impl()}
{
    myImpl->myCurrentId = 1;
}

DocumentManager::~DocumentManager()
{
}

DocumentManager &DocumentManager::getInstance()
{
    static DocumentManager instance;
    return instance;
}

Result<int> DocumentManager::createFromFile(const std::string &filePath)
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
            if (fileExtension(filePath) == "mxl")
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
            return mirror(parsed.error());
        }

        auto mxdoc = std::make_shared<core::Document>(std::move(parsed).value());

        LOCK_DOCUMENT_MANAGER
        myImpl->myMap[myImpl->myCurrentId] = std::move(mxdoc);
        return myImpl->myCurrentId++;
    }
    catch (const std::exception &e)
    {
        return internalError("createFromFile", e.what());
    }
    catch (...)
    {
        return internalError("createFromFile", "unknown exception");
    }
}

Result<int> DocumentManager::createFromStream(std::istream &stream)
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
            return mirror(parsed.error());
        }

        auto mxdoc = std::make_shared<core::Document>(std::move(parsed).value());

        LOCK_DOCUMENT_MANAGER
        myImpl->myMap[myImpl->myCurrentId] = std::move(mxdoc);
        return myImpl->myCurrentId++;
    }
    catch (const std::exception &e)
    {
        return internalError("createFromStream", e.what());
    }
    catch (...)
    {
        return internalError("createFromStream", "unknown exception");
    }
}

Result<int> DocumentManager::createFromScore(const ScoreData &score)
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

        LOCK_DOCUMENT_MANAGER
        myImpl->myMap[myImpl->myCurrentId] = std::move(mxdoc);
        return myImpl->myCurrentId++;
    }
    catch (const impl::WriteRefusal &refusal)
    {
        // Refuse, don't drop: the ScoreData describes something the core
        // model will not represent.
        return refusal.error();
    }
    catch (const std::exception &e)
    {
        return internalError("createFromScore", e.what());
    }
    catch (...)
    {
        return internalError("createFromScore", "unknown exception");
    }
}

Result<void> DocumentManager::writeToFile(int documentId, const std::string &filePath) const
{
    try
    {
        LOCK_DOCUMENT_MANAGER

        const DocumentMap::const_iterator it = myImpl->myMap.find(documentId);
        if (it == myImpl->myMap.cend())
        {
            return ApiError{ResultCode::badDocumentId, "", "writeToFile: bad document id"};
        }

        pugi::xml_document xdoc;
        core::serialize(withWriteVersion(*it->second), xdoc);
        if (!xdoc.save_file(filePath.c_str(), "  "))
        {
            return ApiError{ResultCode::ioError, filePath, "writeToFile: could not write the file"};
        }
        return Result<void>{};
    }
    catch (const std::exception &e)
    {
        return internalError("writeToFile", e.what());
    }
    catch (...)
    {
        return internalError("writeToFile", "unknown exception");
    }
}

Result<void> DocumentManager::writeToStream(int documentId, std::ostream &stream) const
{
    try
    {
        LOCK_DOCUMENT_MANAGER

        const DocumentMap::const_iterator it = myImpl->myMap.find(documentId);
        if (it == myImpl->myMap.cend())
        {
            return ApiError{ResultCode::badDocumentId, "", "writeToStream: bad document id"};
        }

        pugi::xml_document xdoc;
        core::serialize(withWriteVersion(*it->second), xdoc);
        xdoc.save(stream, "  ");
        return Result<void>{};
    }
    catch (const std::exception &e)
    {
        return internalError("writeToStream", e.what());
    }
    catch (...)
    {
        return internalError("writeToStream", "unknown exception");
    }
}

Result<ScoreData> DocumentManager::getData(int documentId) const
{
    try
    {
        LOCK_DOCUMENT_MANAGER

        const DocumentMap::const_iterator it = myImpl->myMap.find(documentId);
        if (it == myImpl->myMap.cend())
        {
            return ApiError{ResultCode::badDocumentId, "", "getData: bad document id"};
        }

        // Convert into a local partwise copy and read that; the stored
        // document is untouched. The old mutate-and-restore dance (convert
        // the stored document to partwise, read, convert back) is gone.
        if (it->second->isScoreTimewise())
        {
            const core::ScorePartwise scorePartwise = impl::timewisePartwise(it->second->asScoreTimewise());
            impl::ScoreReader reader{scorePartwise};
            auto score = reader.getScoreData();
            score.musicXmlType = "timewise";
            return score;
        }

        impl::ScoreReader reader{it->second->asScorePartwise()};
        return reader.getScoreData();
    }
    catch (const std::exception &e)
    {
        return internalError("getData", e.what());
    }
    catch (...)
    {
        return internalError("getData", "unknown exception");
    }
}

void DocumentManager::destroyDocument(int documentId)
{
    LOCK_DOCUMENT_MANAGER
    const DocumentMap::const_iterator it = myImpl->myMap.find(documentId);

    if (it == myImpl->myMap.cend())
    {
        return;
    }

    myImpl->myMap.erase(it);
}

mx::core::DocumentPtr DocumentManager::getDocument(int documentId) const
{
    LOCK_DOCUMENT_MANAGER
    const DocumentMap::const_iterator it = myImpl->myMap.find(documentId);

    if (it == myImpl->myMap.cend())
    {
        return mx::core::DocumentPtr{};
    }

    return it->second;
}

int DocumentManager::getUniqueId()
{
    LOCK_DOCUMENT_MANAGER
    int returnValue = myImpl->myCurrentUniqueId;
    ++myImpl->myCurrentUniqueId;
    return returnValue;
}
} // namespace api
} // namespace mx
