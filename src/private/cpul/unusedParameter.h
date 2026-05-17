#pragma once

#define PRIVATE_UNUSED_PARAMETER_STRINGIFY(argument) #argument

#ifndef _MSC_VER
    #define UNUSED_PARAMETER(arg) _Pragma(PRIVATE_UNUSED_PARAMETER_STRINGIFY(unused(arg)))
#else
    #define UNUSED_PARAMETER(arg) (##arg);
#endif