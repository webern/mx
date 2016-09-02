// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#define MX_JIT_ALLOCATE_ATTRIBUTES( attributesType ) \
if( !myAttributes ) \
{ \
    myAttributes = std::make_shared<attributesType>(); \
}

#define MX_JIT_ALLOCATE( namePart ) \
if( !my##namePart ) \
{ \
    my##namePart = make##namePart(); \
}
