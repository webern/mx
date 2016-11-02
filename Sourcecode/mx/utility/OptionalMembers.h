// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

// The various structs which derive from AttributeInterface all have different
// sets of public member variables.  Many of them share common sets of sets of
// these, defaultX, defaultY, etc., but not always exactly the same.
//
// The following functions allow us to inspect the Attributes struct to see if
// it has the desired field, and if it does, to extract its value.
//
// For the pairs of functions below, the compiler will prefer to resolve to the
// first function in the case that 'attributeFieldName' exists on T.  If it
// not exists, then the compiler will choose the second version of the function
// which does not access the 'attributeFieldName'
//
// If you have a struct SomeAttributes { int someInterestingField; }
// Then you instantiate these functions with a macro call that looks like this:
//
// MX_ATTR_FUNC_OPTIONAL( someInterestingField, SomeInterestingField, int, 0 );
//
// Then to call the functions it looks like this:
//
// SomeAttributes someAttributes;
// someAttributes.someInterestingField = 6;
// int x = checkSomeInterestingField( &someAttributes ); /* returns '6' */
//
// struct DoesntHaveTheField { int unrelatedStuff; }
// DoesntHaveTheField doesnt;
// int x = checkSomeInterestingField( &doesnt ); /* returns '0' */
//
// The second macro MX_ATTR_FUNC_OPTIONAL_WITH_GETTER is used the same way
// but for fieds that require a call to a .getValue() accessor
//
// The magic came from here h t t p : / / stackoverflow.com/a/31860104/2779792

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    #define MX_ATTR_FUNC_OPTIONAL( attributeFieldName, attributeFieldNameCapitalized, attributeType, defaultReturnValue )               \
                                                                                                                                        \
    template<class T>                                                                                                                   \
    auto check##attributeFieldNameCapitalized( const T* const attributesRawPtr )                                                        \
    -> decltype(  attributesRawPtr->attributeFieldName  )                                                                               \
    {                                                                                                                                   \
        return attributesRawPtr->attributeFieldName;                                                                                    \
    }                                                                                                                                   \
                                                                                                                                        \
    template<class T>                                                                                                                   \
    inline auto check##attributeFieldNameCapitalized(...) -> attributeType                                                              \
    {                                                                                                                                   \
        return defaultReturnValue;                                                                                                      \
    }                                                                                                                                   \

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    #define MX_ATTR_FUNC_OPTIONAL_WITH_GETTER( attributeFieldName, attributeFieldNameCapitalized, attributeType, defaultReturnValue )   \
                                                                                                                                        \
    template<class T>                                                                                                                   \
    auto check##attributeFieldNameCapitalized( const T* const obj )                                                                     \
    -> decltype(  obj->attributeFieldName.getValue()  )                                                                                 \
    {                                                                                                                                   \
        return obj->attributeFieldName.getValue();                                                                                      \
    }                                                                                                                                   \
                                                                                                                                        \
    template<class T>                                                                                                                   \
    inline auto check##attributeFieldNameCapitalized(...) -> attributeType                                                              \
    {                                                                                                                                   \
        return defaultReturnValue;                                                                                                      \
    }                                                                                                                                   \

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    #define MX_ATTR_SETFUNC_OPTIONAL( attributeFieldName, attributeFieldNameCapitalized, attributeType, failureReturnValue )            \
                                                                                                                                        \
    template<class ATTRIBUTES_STRUCT, class VALUE_TYPE>                                                                                 \
    auto MXIMPLFUNC_lookForAndSet##attributeFieldNameCapitalized(                                                                       \
        VALUE_TYPE valueToSet,                                                                                                          \
        ATTRIBUTES_STRUCT* const outAttributesRawPtr,                                                                                   \
        int dummy )                                                                                                                     \
    -> decltype(  outAttributesRawPtr->attributeFieldName  )                                                                            \
    {                                                                                                                                   \
        MX_UNUSED( dummy );                                                                                                             \
        outAttributesRawPtr->attributeFieldName = valueToSet;                                                                           \
        return outAttributesRawPtr->attributeFieldName;                                                                                 \
    }                                                                                                                                   \
                                                                                                                                        \
    template<class ATTRIBUTES_STRUCT, class VALUE_TYPE>                                                                                 \
    auto MXIMPLFUNC_lookForAndSet##attributeFieldNameCapitalized(                                                                       \
        VALUE_TYPE valueToSet,                                                                                                          \
        ATTRIBUTES_STRUCT* const outAttributesRawPtr,                                                                                   \
        long dummy )                                                                                                                    \
    -> decltype( failureReturnValue )                                                                                                   \
    {                                                                                                                                   \
        MX_UNUSED( outAttributesRawPtr );                                                                                               \
        MX_UNUSED( dummy );                                                                                                             \
        MX_UNUSED( valueToSet );                                                                                                        \
        return failureReturnValue;                                                                                                      \
    }                                                                                                                                   \
                                                                                                                                        \
    template<class ATTRIBUTES_STRUCT, class VALUE_TYPE>                                                                                 \
    auto lookForAndSet##attributeFieldNameCapitalized(                                                                                  \
        VALUE_TYPE valueToSet,                                                                                                          \
        ATTRIBUTES_STRUCT* const outAttributesRawPtr )                                                                                  \
    -> decltype( MXIMPLFUNC_lookForAndSet##attributeFieldNameCapitalized( valueToSet, outAttributesRawPtr, 0 ) )                        \
    {                                                                                                                                   \
         return MXIMPLFUNC_lookForAndSet##attributeFieldNameCapitalized( valueToSet, outAttributesRawPtr, 0 );                          \
    }                                                                                                                                   \

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    #define MX_ATTR_SETFUNC_OPTIONAL_WITH_SETTER( attributeFieldName, attributeFieldNameCapitalized, attributeType, failureReturnValue )\
                                                                                                                                        \
    template<class ATTRIBUTES_STRUCT, class VALUE_TYPE>                                                                                 \
    auto MXIMPLFUNC_lookForAndSet##attributeFieldNameCapitalized(                                                                       \
        VALUE_TYPE valueToSet,                                                                                                          \
        ATTRIBUTES_STRUCT* const outAttributesRawPtr,                                                                                   \
        int dummy )                                                                                                                     \
    -> decltype( outAttributesRawPtr->attributeFieldName.setValue( valueToSet ) )                                                       \
    {                                                                                                                                   \
        MX_UNUSED( dummy );                                                                                                             \
        outAttributesRawPtr->attributeFieldName.setValue( valueToSet );                                                                 \
    }                                                                                                                                   \
                                                                                                                                        \
    template<class ATTRIBUTES_STRUCT, class VALUE_TYPE>                                                                                 \
    auto MXIMPLFUNC_lookForAndSet##attributeFieldNameCapitalized(                                                                       \
        VALUE_TYPE valueToSet,                                                                                                          \
        ATTRIBUTES_STRUCT* const outAttributesRawPtr,                                                                                   \
        long dummy )                                                                                                                    \
    -> decltype( void() )                                                                                                               \
    {                                                                                                                                   \
        MX_UNUSED( outAttributesRawPtr );                                                                                               \
        MX_UNUSED( dummy );                                                                                                             \
        MX_UNUSED( valueToSet );                                                                                                        \
    }                                                                                                                                   \
                                                                                                                                        \
    template<class ATTRIBUTES_STRUCT, class VALUE_TYPE>                                                                                 \
    auto lookForAndSet##attributeFieldNameCapitalized(                                                                                  \
        VALUE_TYPE valueToSet,                                                                                                          \
        ATTRIBUTES_STRUCT* const outAttributesRawPtr )                                                                                  \
    -> decltype( MXIMPLFUNC_lookForAndSet##attributeFieldNameCapitalized( valueToSet, outAttributesRawPtr, 0 ), void() )                \
    {                                                                                                                                   \
         MXIMPLFUNC_lookForAndSet##attributeFieldNameCapitalized( valueToSet, outAttributesRawPtr, 0 );                                 \
    }                                                                                                                                   \
                                                                                                                                        \

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
