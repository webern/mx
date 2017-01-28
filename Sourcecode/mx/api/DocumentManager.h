// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/ScoreData.h"

#include <ostream>
#include <memory>

namespace mx
{
    namespace core
    {
        class Document;
        using DocumentPtr = std::shared_ptr<Document>;
    }
    
    namespace api
    {
        class DocumentManager
        {
        public:
            DocumentManager( const DocumentManager& other ) = delete;
            DocumentManager( DocumentManager&& other ) = delete;
            DocumentManager& operator=( const DocumentManager& other ) = delete;
            DocumentManager& operator=( DocumentManager&& other ) = delete;
            ~DocumentManager();
            
            // this class is a singleton, get it like this
            // auto& docMngr = DocumentManager::getInstance()
            static DocumentManager& getInstance();
            
            
            // creates a MusicXML document from a file
            // and returns the document's ID number, -1 if error
            int createFromFile( const std::string& filePath );
            
            
            // creates a MusicXML document from a character stream
            // and returns the document's ID number, -1 if error
            int createFromStream( std::istream& stream );
            
            
            // creates a MusicXML document from a Score structure
            // and returns the document's ID number, -1 if error
            int createFromScore( const ScoreData& score );
            
            
            // saves an existing MusicXML document to a
            // file, -1 if error
            void writeToFile( int documentId, const std::string& filePath ) const;
            
            
            // saves an existing MusicXML document to a
            // character stream, -1 if error
            void writeToStream( int documentId, std::ostream& stream ) const;

            
            // retreives the data from an existing document
            // and returns it in the Score structure.  if id
            // does not exist, returns a default Score object
            ScoreData getData( int documentId ) const;
            

            // destroys the shared_ptr which is holding the document
            // internally and stops tracking ownership of the document
            void destroyDocument( int documentId );
            
            
            // avoid using this function if possible, only use this function
            // if your requirements are not met by the Score structure
            // note: returns a nullptr if the documentId is bad.  if you
            // hold a DocumentPtr and call destroyDocument then the
            // DocumentManager will no longer know about the object, but the
            // object will still exist
            mx::core::DocumentPtr getDocument( int documentId ) const;
            
        private:
            DocumentManager();
            class Impl;
            std::unique_ptr<Impl> myImpl;
            
        };
    }
}
