// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// Hand-written runtime for the generated mx::core model; never regenerated.

#pragma once

#include <string>
#include <string_view>

namespace mx::core
{

// Locale-free lexical parse/format helpers (std::from_chars / std::to_chars
// based) the generated value types call. Parsing is lexically strict: the
// whole input must be consumed (leading/trailing XML whitespace tolerated,
// nothing else). The lenient flavors return 0 on garbage — the import
// leniency policy (plan §2.4); range clamping happens in the caller.

/// The input minus leading/trailing XML whitespace.
std::string_view trimWhitespace(std::string_view text);

bool tryParseInt(std::string_view text, int &out);

/// Lenient: 0 when the text is not an integer.
int parseInt(std::string_view text);

bool tryParseDouble(std::string_view text, double &out);

/// Lenient: 0.0 when the text is not a number.
double parseDouble(std::string_view text);

std::string formatInt(int value);

/// True when a MusicXML version string ("3.1") declares a version NEWER
/// than the supported one. Unparseable versions are not newer (MusicXML
/// 1.0 had no version attribute; absence means oldest).
bool musicXmlVersionExceeds(std::string_view declared, std::string_view supported);

/// Shortest fixed-notation decimal that round-trips the double (xs:decimal
/// forbids exponent notation).
std::string formatDouble(double value);

} // namespace mx::core
