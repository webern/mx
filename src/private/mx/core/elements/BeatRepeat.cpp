// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/BeatRepeat.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/SlashDot.h"
#include "mx/core/elements/SlashType.h"
#include <iostream>

namespace mx
{
namespace core
{
BeatRepeat::BeatRepeat()
    : myAttributes(std::make_shared<BeatRepeatAttributes>()), mySlashType(makeSlashType()), myHasSlashType(false),
      mySlashDotSet()
{
}

bool BeatRepeat::hasAttributes() const
{
    return myAttributes->hasValues();
}

std::ostream &BeatRepeat::streamAttributes(std::ostream &os) const
{
    return myAttributes->toStream(os);
}

std::ostream &BeatRepeat::streamName(std::ostream &os) const
{
    os << "beat-repeat";
    return os;
}

bool BeatRepeat::hasContents() const
{
    return myHasSlashType || mySlashDotSet.size() > 0;
}

std::ostream &BeatRepeat::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const
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

BeatRepeatAttributesPtr BeatRepeat::getAttributes() const
{
    return myAttributes;
}

void BeatRepeat::setAttributes(const BeatRepeatAttributesPtr &value)
{
    if (value)
    {
        myAttributes = value;
    }
}

SlashTypePtr BeatRepeat::getSlashType() const
{
    return mySlashType;
}

void BeatRepeat::setSlashType(const SlashTypePtr &value)
{
    if (value)
    {
        mySlashType = value;
    }
}

bool BeatRepeat::getHasSlashType() const
{
    return myHasSlashType;
}

void BeatRepeat::setHasSlashType(const bool value)
{
    myHasSlashType = value;
}

const SlashDotSet &BeatRepeat::getSlashDotSet() const
{
    return mySlashDotSet;
}

void BeatRepeat::removeSlashDot(const SlashDotSetIterConst &value)
{
    if (value != mySlashDotSet.cend())
    {
        mySlashDotSet.erase(value);
    }
}

void BeatRepeat::addSlashDot(const SlashDotPtr &value)
{
    if (value)
    {
        mySlashDotSet.push_back(value);
    }
}

void BeatRepeat::clearSlashDotSet()
{
    mySlashDotSet.clear();
}

SlashDotPtr BeatRepeat::getSlashDot(const SlashDotSetIterConst &setIterator) const
{
    if (setIterator != mySlashDotSet.cend())
    {
        return *setIterator;
    }
    return SlashDotPtr();
}

bool BeatRepeat::fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement)
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
