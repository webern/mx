// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// Hand-written runtime for the generated mx::core model; never regenerated.

#pragma once

#include "mx/core/ParseContext.h"

#include "pugixml.hpp"

#include <span>

namespace mx::core
{

/// How the schema types an element's identity attribute.
enum class IdAttributeKind
{
    definition, // xs:ID: no other element in the document may use the value
    reference   // xs:IDREF: the value must be some element's xs:ID
};

struct IdAttribute
{
    const char *element;
    const char *attribute;
    IdAttributeKind kind;
    /// For a reference, the element the value must name; empty otherwise.
    /// MusicXML states this in prose only: XSD can require that an ID exist
    /// but not that it belong to a particular element.
    const char *target;
};

/// Every identity attribute in MusicXML 4.0, one entry per element name.
///
/// To whoever extends this, human or coding agent: a MusicXML version that
/// adds an xs:ID or xs:IDREF attribute needs an entry here, and a new
/// reference's target has to be read out of the schema's documentation.
/// IdSchemaDriftTest re-derives these entries from the schema and fails while
/// the two disagree, but the schema does not state a target, so check that
/// one by hand.
std::span<const IdAttribute> idAttributes();

/// Reports duplicate xs:ID values and xs:IDREF values that do not resolve.
/// Nothing is changed: the caller is owed fidelity to the file they handed
/// us, and renaming on import would break the parse/serialize round trip.
///
/// Values are compared after Token repair, because that is what the parsed
/// document holds. Two ids that differ in the file can therefore collide
/// here, which is the collision reportIdRepair warns may happen.
void checkIds(const pugi::xml_document &doc, const DiagnosticHandler &handler);

/// The same checks for a document about to be written, renaming duplicates
/// so we never emit a document that violates the schema. The first element
/// to claim an id keeps it and later ones are renamed; references are left
/// pointing at the first, because nothing says which was meant.
void repairIds(pugi::xml_document &doc, const DiagnosticHandler &handler);

} // namespace mx::core
