// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// Adapted from info found here:
// https://en.wikibooks.org/wiki/C_Programming/Preprocessor#X-Macros

#ifndef MX_ENUM_CLASS_NAME
    #error MX_ENUM_CLASS_NAME must be defined
#endif

#ifndef MX_ENUM_MEMBERS
    #error MX_ENUM_MEMBERS must be defined
#endif

#define RETURN_MX_ENUM_CLASS_NAME(MX_ENUM_CLASS_NAME) MX_ENUM_CLASS_NAME

#define MX_CAT(a, ...) MX_PRIMITIVE_CAT(a, __VA_ARGS__)
#define MX_PRIMITIVE_CAT(a, ...) a ## __VA_ARGS__

#define MX_ENUM_VALUE(value, string) value,

enum class MX_ENUM_CLASS_NAME { MX_ENUM_MEMBERS(MX_ENUM_VALUE) };

#undef MX_ENUM_VALUE
#ifdef MX_STRING_CONSTANT_NAME
#undef MX_STRING_CONSTANT_NAME
#endif

#define MX_STRING_CONSTANT_NAME(XXX, value) MX_CAT(MX_CAT(XXX, _), MX_CAT(value, _String))

#define MX_ENUM_VALUE(value, string) static constexpr const char* const MX_STRING_CONSTANT_NAME(  RETURN_MX_ENUM_CLASS_NAME( MX_ENUM_CLASS_NAME ) , value ) = string;

MX_ENUM_MEMBERS(MX_ENUM_VALUE);

#undef MX_ENUM_VALUE

#define MX_ENUM_VALUE(value, string) case MX_ENUM_CLASS_NAME::value: return MX_STRING_CONSTANT_NAME(  RETURN_MX_ENUM_CLASS_NAME( MX_ENUM_CLASS_NAME ) , value );

#define MX_ENUM_TO_STRING(A) MX_PRIMITIVE_CAT(A, ToString)

inline const char* const MX_ENUM_TO_STRING(MX_ENUM_CLASS_NAME) ( MX_ENUM_CLASS_NAME inValue ) {
    switch (inValue) {
        MX_ENUM_MEMBERS(MX_ENUM_VALUE)
    }
    throw std::runtime_error("bad enum value");
}

#undef MX_ENUM_VALUE

#define MX_ENUM_VALUE(value, string) if (inValue == MX_STRING_CONSTANT_NAME(  RETURN_MX_ENUM_CLASS_NAME( MX_ENUM_CLASS_NAME ) , value )) { \
    return MX_ENUM_CLASS_NAME::value; \
} \

#define MX_TO_ENUM(A) MX_PRIMITIVE_CAT(A, FromString)

inline MX_ENUM_CLASS_NAME MX_TO_ENUM(MX_ENUM_CLASS_NAME) ( const std::string& inValue ) {
    MX_ENUM_MEMBERS(MX_ENUM_VALUE)
    throw std::runtime_error("bad string value");
}

#undef MX_TO_ENUM_FUNCTION_DECLARATION
#undef MX_ENUM_VALUE
#undef MX_ENUM_VALUES
#undef MX_ENUM_CLASS_NAME
#undef MX_ENUM_MEMBERS
