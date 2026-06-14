// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

// Generic access to optional attributes that may or may not exist on a given
// core element class (mx-impl-port-plan.md §6).
//
// The old core exposed attributes as public data members
// (`attr.hasDefaultX` + `attr.defaultX`) and this header SFINAE-detected
// them. The new core exposes member functions returning std::optional
// (`el.defaultX()`, `el.setDefaultX(...)`), so detection is now a C++20
// requires-expression. The call-site shape is preserved exactly:
//
//   if (checkHasDefaultX<T>(&el)) { x = checkDefaultX<T>(&el); }
//   lookForAndSetHasDefaultX(true, &el);
//   lookForAndSetDefaultX(1.0, &el);
//
// Semantics under std::optional (presence and value are one):
// - checkHas<Field>: true iff the element has the field and it is engaged.
// - check<Field>: the value when engaged; the default otherwise.
// - lookForAndSetHas<Field>(true): engages the field with its natural zero
//   if it is not already engaged (returns true if the element supports the
//   field). (false): disengages.
// - lookForAndSet<Field>(v): overwrites the value ONLY when the field is
//   engaged -- exactly the old observable, where a value written while the
//   has-flag was false never serialized.

#include "mx/core/Decimal.h"

#include <optional>
#include <type_traits>
#include <utility>

// Presence check.
#define MX_OPTIONAL_HAS_FUNC(fieldName, fieldNameCapitalized)                                                          \
                                                                                                                       \
    template <class T> bool checkHas##fieldNameCapitalized(const T *const el)                                          \
    {                                                                                                                  \
        if constexpr (requires { el->fieldName().has_value(); })                                                       \
        {                                                                                                              \
            return el->fieldName().has_value();                                                                        \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            return false;                                                                                              \
        }                                                                                                              \
    }

// Value getter for a clamped-decimal wrapper field (e.g. Tenths).
#define MX_OPTIONAL_GET_DECIMAL_FUNC(fieldName, fieldNameCapitalized, defaultReturnValue)                              \
                                                                                                                       \
    template <class T> double check##fieldNameCapitalized(const T *const el)                                           \
    {                                                                                                                  \
        if constexpr (requires { el->fieldName()->value().value(); })                                                  \
        {                                                                                                              \
            if (el->fieldName().has_value())                                                                           \
            {                                                                                                          \
                return static_cast<double>(el->fieldName()->value().value());                                          \
            }                                                                                                          \
        }                                                                                                              \
        return defaultReturnValue;                                                                                     \
    }

// Value getter for a clamped-int wrapper field (e.g. NumberOfLines).
#define MX_OPTIONAL_GET_INT_FUNC(fieldName, fieldNameCapitalized, defaultReturnValue)                                  \
                                                                                                                       \
    template <class T> int check##fieldNameCapitalized(const T *const el)                                              \
    {                                                                                                                  \
        if constexpr (requires { el->fieldName()->value(); })                                                          \
        {                                                                                                              \
            if (el->fieldName().has_value())                                                                           \
            {                                                                                                          \
                return static_cast<int>(el->fieldName()->value());                                                     \
            }                                                                                                          \
        }                                                                                                              \
        return defaultReturnValue;                                                                                     \
    }

// Value getter returning the wrapped value itself (enum wrappers, Color,
// FontFamily, FontSize, ...).
#define MX_OPTIONAL_GET_VALUE_FUNC(fieldName, fieldNameCapitalized, valueType, defaultReturnValue)                     \
                                                                                                                       \
    template <class T> valueType check##fieldNameCapitalized(const T *const el)                                        \
    {                                                                                                                  \
        if constexpr (requires { valueType{*el->fieldName()}; })                                                       \
        {                                                                                                              \
            if (el->fieldName().has_value())                                                                           \
            {                                                                                                          \
                return *el->fieldName();                                                                               \
            }                                                                                                          \
        }                                                                                                              \
        return defaultReturnValue;                                                                                     \
    }

// Presence setter. (true) engages with the natural zero when disengaged so a
// following value write lands; (false) disengages. Returns whether the
// element supports the field (mirrors the old SFINAE return-or-false shape).
#define MX_OPTIONAL_SET_HAS_FUNC(fieldName, setterName, fieldNameCapitalized)                                          \
                                                                                                                       \
    template <class T> bool lookForAndSetHas##fieldNameCapitalized(bool has, T *const el)                              \
    {                                                                                                                  \
        if constexpr (requires { el->setterName(std::nullopt); })                                                      \
        {                                                                                                              \
            if (!has)                                                                                                  \
            {                                                                                                          \
                el->setterName(std::nullopt);                                                                          \
            }                                                                                                          \
            else if (!el->fieldName().has_value())                                                                     \
            {                                                                                                          \
                el->setterName(typename std::decay_t<decltype(el->fieldName())>::value_type{});                        \
            }                                                                                                          \
            return has;                                                                                                \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            return false;                                                                                              \
        }                                                                                                              \
    }

// Value setter for a clamped-decimal wrapper field.
#define MX_OPTIONAL_SET_DECIMAL_FUNC(fieldName, setterName, fieldNameCapitalized)                                      \
                                                                                                                       \
    template <class T, class VALUE_TYPE> void lookForAndSet##fieldNameCapitalized(VALUE_TYPE value, T *const el)       \
    {                                                                                                                  \
        if constexpr (requires {                                                                                       \
                          el->setterName(typename std::decay_t<decltype(el->fieldName())>::value_type{                 \
                              mx::core::Decimal{static_cast<double>(value)}});                                         \
                      })                                                                                               \
        {                                                                                                              \
            if (el->fieldName().has_value())                                                                           \
            {                                                                                                          \
                el->setterName(typename std::decay_t<decltype(el->fieldName())>::value_type{                           \
                    mx::core::Decimal{static_cast<double>(value)}});                                                   \
            }                                                                                                          \
        }                                                                                                              \
    }

// Value setter for a clamped-int wrapper field.
#define MX_OPTIONAL_SET_INT_FUNC(fieldName, setterName, fieldNameCapitalized)                                          \
                                                                                                                       \
    template <class T> void lookForAndSet##fieldNameCapitalized(int value, T *const el)                                \
    {                                                                                                                  \
        if constexpr (requires {                                                                                       \
                          el->setterName(typename std::decay_t<decltype(el->fieldName())>::value_type{value});         \
                      })                                                                                               \
        {                                                                                                              \
            if (el->fieldName().has_value())                                                                           \
            {                                                                                                          \
                el->setterName(typename std::decay_t<decltype(el->fieldName())>::value_type{value});                   \
            }                                                                                                          \
        }                                                                                                              \
    }

// Value setter taking the wrapped value itself.
#define MX_OPTIONAL_SET_VALUE_FUNC(fieldName, setterName, fieldNameCapitalized)                                        \
                                                                                                                       \
    template <class T, class VALUE_TYPE> void lookForAndSet##fieldNameCapitalized(VALUE_TYPE value, T *const el)       \
    {                                                                                                                  \
        if constexpr (requires { el->setterName(std::move(value)); })                                                  \
        {                                                                                                              \
            if (el->fieldName().has_value())                                                                           \
            {                                                                                                          \
                el->setterName(std::move(value));                                                                      \
            }                                                                                                          \
        }                                                                                                              \
    }
