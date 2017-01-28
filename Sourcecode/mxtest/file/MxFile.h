// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include <string>
#include <vector>

namespace mxtest
{
	class MxFile
	{
	public:
        std::string fileName;
        std::string subdirectory;
        std::string path;
        bool isLoadFailureExpected;
        int sizeBytes;

        inline std::string getFileNamePart() const
        {
            return fileName.substr( 0, static_cast<size_t>( static_cast<int>( fileName.size() ) - 4 ) );
        }

	private:
		
	};

	using MxFiles = std::vector<MxFile>;
}
