// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once 

#include "mx/api/ScoreData.h"
#include "mxtest/api/ScoreDataCreator.h"
#include "mxtest/file/MxFile.h"
#include "mxtest/file/MxFileTest.h"

namespace mxtest
{
    class ApiTester : public MxFileTest
    {
    public:
        ApiTester(
        	ScoreDataCreatorPtr&& inScoreDataCreator,
        	MxFile inFile,
          	std::string cppFileName,
          	int cppFileLineNumber); 

    virtual void runTestCode() override;
        
    private:
        ScoreDataCreatorPtr myScoreDataCreator;
    };
}

