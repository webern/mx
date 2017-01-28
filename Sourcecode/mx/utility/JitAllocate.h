// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

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
