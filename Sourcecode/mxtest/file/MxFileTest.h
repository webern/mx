// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "cpul/cpulTest.h"
#include "mxtest/file/MxFile.h"

namespace mxtest
{
	class MxFileTest : public ::Test
	{
	public:
		MxFileTest(
            mxtest::MxFile inTestFile,
            std::string inTestName,
            std::string inTestCppFileName,
            int inTestCppFileLineNumber );

		virtual void runTest( TestResult& rEsUlT_ ) final;

		virtual const std::string& testFileName() const final;
		virtual const std::string testFileNamePart() const final;
		virtual const std::string testFileExtension() const final;
		virtual const std::string& testFilePath() const final;
		virtual const std::string& testSubdirectory() const final;
        virtual const bool isLoadFailureExpected() const final { return myTestFile.isLoadFailureExpected; }

		virtual bool getIsSuccess() const final;
		virtual void setIsSuccess( bool inIsSuccess ) final;
		virtual const std::string& getFailureMessage() const final;
		virtual void setFailureMessage( std::string inMessage ) final;

        virtual inline const std::string& getCppFileName() const final { return myCppFileName; }
        virtual inline const int getCppFileLineNumber() const final { return myCppFileLineNumber; }

		virtual void runTestCode() = 0;

        // MxFileTest( MxFileTest&& other );
        // MxFileTest( const MxFileTest& other );
        // MxFileTest& operator=( MxFileTest&& other );
        // MxFileTest& operator=( const MxFileTest& other );

    private:
        mxtest::MxFile myTestFile;
        std::string myTestName;
        std::string myCppFileName;
        int myCppFileLineNumber;
        bool myIsSuccess;
        std::string myFailureMessage;
	};
}
