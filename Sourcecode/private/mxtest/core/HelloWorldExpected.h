// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once
#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_CORE_TESTS

#include <string>
#include <sstream>

inline std::string helloWorldExpected()
{
std::stringstream ss;   

ss << R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>)" << std::endl;
ss << R"(<!DOCTYPE score-partwise PUBLIC "-//Recordare//DTD MusicXML 3.0 Partwise//EN" "http://www.musicxml.org/dtds/partwise.dtd">)" << std::endl;
ss << R"(<score-partwise version="3.0">)" << std::endl;
ss << R"(   <part-list>)" << std::endl;
ss << R"(      <score-part id="P1">)" << std::endl;
ss << R"(         <part-name>Music</part-name>)" << std::endl;
ss << R"(      </score-part>)" << std::endl;
ss << R"(   </part-list>)" << std::endl;
ss << R"(   <part id="P1">)" << std::endl;
ss << R"(      <measure number="1">)" << std::endl;
ss << R"(         <attributes>)" << std::endl;
ss << R"(            <divisions>1</divisions>)" << std::endl;
ss << R"(            <key>)" << std::endl;
ss << R"(               <fifths>0</fifths>)" << std::endl;
ss << R"(            </key>)" << std::endl;
ss << R"(            <time>)" << std::endl;
ss << R"(               <beats>4</beats>)" << std::endl;
ss << R"(               <beat-type>4</beat-type>)" << std::endl;
ss << R"(            </time>)" << std::endl;
ss << R"(            <clef>)" << std::endl;
ss << R"(               <sign>G</sign>)" << std::endl;
ss << R"(               <line>2</line>)" << std::endl;
ss << R"(            </clef>)" << std::endl;
ss << R"(         </attributes>)" << std::endl;
ss << R"(         <note>)" << std::endl;
ss << R"(            <pitch>)" << std::endl;
ss << R"(               <step>C</step>)" << std::endl;
ss << R"(               <octave>4</octave>)" << std::endl;
ss << R"(            </pitch>)" << std::endl;
ss << R"(            <duration>4</duration>)" << std::endl;
ss << R"(         </note>)" << std::endl;
ss << R"(      </measure>)" << std::endl;
ss << R"(   </part>)" << std::endl;
ss << R"(</score-partwise>)";

return ss.str();
}

#endif