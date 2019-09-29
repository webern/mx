// Copyright (c) Matthew James Briggs

#pragma once

#include <iostream>
#include <memory>

namespace ezxml
{
    class XAttribute;
    using XAttributePtr = std::shared_ptr<XAttribute>;

    class XAttribute;
    using XAttributePtr = std::shared_ptr<XAttribute>;

    class XAttributeIterator;
    using XAttributeIteratorPtr = std::shared_ptr<XAttributeIterator>;

    class XAttributeIterator;
    using XAttributeIteratorPtr = std::shared_ptr<XAttributeIterator>;

    class XElement;
    using XElementPtr = std::shared_ptr<XElement>;

    class XAttribute
    {
    public:
        virtual ~XAttribute() = default;

        virtual bool getIsNull() const = 0;
        virtual std::string getName() const = 0;
        virtual std::string getValue() const = 0;

        virtual void setName( const std::string& name ) = 0;
        virtual void setValue( const std::string& name ) = 0;
    };
}
