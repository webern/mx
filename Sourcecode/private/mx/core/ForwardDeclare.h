// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include <string>
#include <vector>
#include <memory>
#include "mx/utility/Throw.h"
#include "mx/utility/Lock.h"
#include "mx/utility/JitAllocate.h"

#ifndef MX_FORWARD_DECLARE_ELEMENT
#define MX_FORWARD_DECLARE_ELEMENT( class_name )                      \
class class_name ;                                                    \
using class_name ## Ptr = std::shared_ptr< class_name >;              \
using class_name ## UPtr = std::unique_ptr< class_name >;             \
using class_name ## Set = std::vector< class_name ## Ptr >;           \
using class_name ## SetIter = class_name ## Set::iterator;            \
using class_name ## SetIterConst = class_name ## Set::const_iterator; \

#endif


#ifndef MX_FORWARD_DECLARE_ATTRIBUTES
#define MX_FORWARD_DECLARE_ATTRIBUTES( struct_name )                  \
struct struct_name ;                                                  \
using struct_name ## Ptr = std::shared_ptr< struct_name >;            \

#endif

// uncomment the next line to throw on parse issues
// #define MX_DO_THROW_ON_PARSE_ISSUE

    #ifdef MX_DO_THROW_ON_PARSE_ISSUE

        #define MX_DEBUG_THROW_ON_PARSE_ISSUE if( !isSuccess ) { MX_THROW( "isSuccess is false" ); }

    #else

        #define MX_DEBUG_THROW_ON_PARSE_ISSUE

    #endif


#ifndef MX_RETURN_IS_SUCCESS
    #define MX_RETURN_IS_SUCCESS MX_DEBUG_THROW_ON_PARSE_ISSUE; return isSuccess;
#endif
