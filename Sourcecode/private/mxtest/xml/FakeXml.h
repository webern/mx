// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_XML_TESTS


namespace mxtest
{
    const char* const fakeXml = R"((((<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<!DOCTYPE total-fake-stuff>
<root-node>
    <abc>abcdefg</abc>
    <empty-element attr-a="A"/>
    <nested-stuff color="blue">
        <something-with-attributes attr-a="A" attr-b="B" attr-c="C">
            <cat>Bones</cat>
            <cat>Bishop</cat>
        </something-with-attributes>
    </nested-stuff>
</root-node>))))";

}

#endif
