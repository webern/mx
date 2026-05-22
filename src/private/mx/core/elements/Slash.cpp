// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Slash.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/SlashDot.h"
#include "mx/core/elements/SlashType.h"
#include <iostream>

namespace mx
{
namespace core
{
Slash::Slash()
    : myAttributes(std::make_shared<SlashAttributes>()), mySlashType(makeSlashType()), myHasSlashType(false),
      mySlashDotSet()
{
}

bool Slash::hasAttributes() const
{
    return myAttributes->hasValues();
}

std::ostream &Slash::streamAttributes(std::ostream &os) const
{
    return myAttributes->toStream(os);
}

std::ostream &Slash::streamName(std::ostream &os) const
{
    os << "slash";
    return os;
}

bool Slash::hasContents() const
{
    return myHasSlashType || mySlashDotSet.size() > 0;
}

std::ostream &Slash::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const
{
    if (myHasSlashType)
    {
        os << std::endl;
        mySlashType->toStream(os, indentLevel + 1);
    }
    for (auto x : mySlashDotSet)
    {
        os << std::endl;
        x->toStream(os, indentLevel + 1);
    }
    if (myHasSlashType || mySlashDotSet.size() > 0)
    {
        isOneLineOnly = false;
        os << std::endl;
    }
    else
    {
        isOneLineOnly = true;
    }
    return os;
}

SlashAttributesPtr Slash::getAttributes() const
{
    return myAttributes;
}

void Slash::setAttributes(const SlashAttributesPtr &value)
{
    if (value)
    {
        myAttributes = value;
    }
}

SlashTypePtr Slash::getSlashType() const
{
    return mySlashType;
}

void Slash::setSlashType(const SlashTypePtr &value)
{
    if (value)
    {
        mySlashType = value;
    }
}

bool Slash::getHasSlashType() const
{
    return myHasSlashType;
}

void Slash::setHasSlashType(const bool value)
{
    myHasSlashType = value;
}

const SlashDotSet &Slash::getSlashDotSet() const
{
    return mySlashDotSet;
}

void Slash::removeSlashDot(const SlashDotSetIterConst &value)
{
    if (value != mySlashDotSet.cend())
    {
        mySlashDotSet.erase(value);
    }
}

void Slash::addSlashDot(const SlashDotPtr &value)
{
    if (value)
    {
        mySlashDotSet.push_back(value);
    }
}

void Slash::clearSlashDotSet()
{
    mySlashDotSet.clear();
}

SlashDotPtr Slash::getSlashDot(const SlashDotSetIterConst &setIterator) const
{
    if (setIterator != mySlashDotSet.cend())
    {
        return *setIterator;
    }
    return SlashDotPtr();
}

bool Slash::fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement)
{
    bool isSuccess = true;
    isSuccess &= myAttributes->fromXElement(message, xelement);

    auto endIter = xelement.end();
    for (auto it = xelement.begin(); it != endIter; ++it)
    {
        if (importElement(message, *it, isSuccess, *mySlashType, myHasSlashType))
        {
            continue;
        }
        importElementSet(message, it, endIter, isSuccess, "slash-dot", mySlashDotSet);
    }

    MX_RETURN_IS_SUCCESS;
}

} // namespace core
} // namespace mx
