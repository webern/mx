// Zeus, Copyright 2017 by Matthew James Briggs

// Adapted from info found here:
// https://en.wikibooks.org/wiki/C_Programming/Preprocessor#X-Macros

#ifndef ZEUS_ENUM_CLASS_NAME
    #error ZEUS_ENUM_CLASS_NAME must be defined
#endif

#ifndef ZEUS_ENUM_MEMBERS
    #error ZEUS_ENUM_MEMBERS must be defined
#endif

#define ZEUS_CAT(a, ...) ZEUS_PRIMITIVE_CAT(a, __VA_ARGS__)
#define ZEUS_PRIMITIVE_CAT(a, ...) a ## __VA_ARGS__

#define ZEUS_ENUM_VALUE(value) value,

enum class ZEUS_ENUM_CLASS_NAME { ZEUS_ENUM_MEMBERS(ZEUS_ENUM_VALUE) };

#undef ZEUS_ENUM_VALUE

#undef ZEUS_ENUM_VALUE
#ifdef ZEUS_STRING_CONSTANT_NAME
#undef ZEUS_STRING_CONSTANT_NAME
#endif

#define ZEUS_STRING_CONSTANT_NAME(XXX, value) ZEUS_CAT(ZEUS_CAT(XXX, _), ZEUS_CAT(value, _String))

#define ZEUS_ENUM_VALUE(value) static constexpr const char* const ZEUS_STRING_CONSTANT_NAME(ZEUS_ENUM_CLASS_NAME, value) = #value;

ZEUS_ENUM_MEMBERS(ZEUS_ENUM_VALUE);

#undef ZEUS_ENUM_VALUE

#define ZEUS_ENUM_VALUE(value) case ZEUS_ENUM_CLASS_NAME::value: return ZEUS_STRING_CONSTANT_NAME(ZEUS_ENUM_CLASS_NAME, value);

#define ZEUS_ENUM_TO_STRING(A) ZEUS_PRIMITIVE_CAT(A, ToString)

inline const char* const ZEUS_ENUM_TO_STRING(ZEUS_ENUM_CLASS_NAME) ( ZEUS_ENUM_CLASS_NAME inValue ) {
    switch (inValue) {
        ZEUS_ENUM_MEMBERS(ZEUS_ENUM_VALUE)
    }
    throw std::runtime_error("bad enum value");
}

#undef ZEUS_ENUM_VALUE

#define ZEUS_ENUM_VALUE(value) if (inValue == ZEUS_STRING_CONSTANT_NAME(ZEUS_ENUM_CLASS_NAME, value)) { \
    return ZEUS_ENUM_CLASS_NAME::value; \
} \

#define ZEUS_TO_ENUM(A) ZEUS_PRIMITIVE_CAT(A, FromString)

inline ZEUS_ENUM_CLASS_NAME ZEUS_TO_ENUM(ZEUS_ENUM_CLASS_NAME) ( const std::string& inValue ) {
    ZEUS_ENUM_MEMBERS(ZEUS_ENUM_VALUE)
    throw std::runtime_error("bad string value");
}

#undef ZEUS_TO_ENUM_FUNCTION_DECLARATION
#undef ZEUS_ENUM_VALUE
#undef ZEUS_ENUM_VALUES
#undef ZEUS_ENUM_CLASS_NAME
#undef ZEUS_ENUM_MEMBERS
