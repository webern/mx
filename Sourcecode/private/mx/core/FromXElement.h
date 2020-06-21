// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ForwardDeclare.h"
#include "mx/core/ElementInterface.h"
#include "ezxml/XElement.h"
#include "ezxml/XAttribute.h"
#include "ezxml/XElementIterator.h"
#include "ezxml/XAttributeIterator.h"
#include <iosfwd>

#ifndef MX_FROM_XELEMENT_UNUSED
#define MX_FROM_XELEMENT_UNUSED( class_name ) \
\
        bool class_name::fromXElementImpl( std::ostream& message, ::ezxml::XElement& xelement ) \
        { \
            MX_UNUSED( xelement ); \
            message << #class_name ": fromXElement function is not expected to be called on this class" << std::endl; \
            return false; \
        } \
\

#endif

#ifndef MX_CHOICE_IF
#define MX_CHOICE_IF( enum_symbol, element_name, function_name_part  ) \
\
        if( xelement.getName() == element_name ) \
        { \
            myChoice = Choice::enum_symbol; \
            return get##function_name_part()->fromXElement( message, xelement ); \
        } \
\

#endif

#ifndef MX_BAD_ELEMENT_FAILURE
#define MX_BAD_ELEMENT_FAILURE( class_name  ) \
\
        message << #class_name << ": '" << xelement.getName() << "' is unrecognized" << std::endl; \
        return false; \

#endif

namespace mx
{
    namespace core
    {

        bool importElement(
            std::ostream& message,
            ::ezxml::XElement& xelement,
            bool& isSuccess,
            ElementInterface& element,
            bool& isMatch );
        

        template<typename T>
        bool importElementSet(
            std::ostream& message,
            ::ezxml::XElementIterator& iter,
            ::ezxml::XElementIterator& endIter,
            bool& isSuccess,
            const std::string& elementName,
            std::vector<std::shared_ptr<T>>& elementSet )
        {
            bool isFirstItemAdded = false;
            bool doDecrementIter = false;
            while( iter != endIter && iter->getName() == elementName )
            {
                auto item = std::make_shared<T>();
                isSuccess &= item->fromXElement( message, *iter );
                if( !isFirstItemAdded && elementSet.size() == 1 )
                {
                    *elementSet.begin() = item;
                    isFirstItemAdded = true;
                }
                else
                {
                    elementSet.push_back( item );
                    isFirstItemAdded = true;
                }
                doDecrementIter = true;
                ++iter;
            }
            if( doDecrementIter )
            {
                --iter;                
            }
            return isFirstItemAdded;
        }


        template<typename PARENT_TYPE, typename CHILD_TYPE>
        void checkSetGroupMemberAndIncrementIter(
            std::ostream& message,
            ::ezxml::XElementIterator& iter,
            ::ezxml::XElementIterator& endIter,
            bool& isSuccess,
            bool& hasIterBeenIncremented,
            std::shared_ptr<PARENT_TYPE>& elementPtr,
            std::shared_ptr<CHILD_TYPE>( PARENT_TYPE:: *getChildFunction ) () const,
            void ( PARENT_TYPE:: *setHasFunction ) ( bool ) )
        {
            if( iter == endIter )
            {
                return;
            }
            auto& parent = *elementPtr;
            auto childPtr = ( ( parent ).*( getChildFunction ) )();

            if( iter->getName() == childPtr->getElementName() )
            {
                isSuccess &= childPtr->fromXElement( message, *iter );
                ( ( parent ).*( setHasFunction ) )( true );
                ++iter;
                hasIterBeenIncremented = true;
            }
        }


        template<typename PARENT_TYPE, typename CHILD_TYPE>
        void checkSetGroupMemberAndIncrementIter(
            std::ostream& message,
            ::ezxml::XElementIterator& iter,
            ::ezxml::XElementIterator& endIter,
            bool& isSuccess,
            bool& hasIterBeenIncremented,
            std::shared_ptr<PARENT_TYPE>& elementPtr,
            std::shared_ptr<CHILD_TYPE>( PARENT_TYPE:: *getChildFunction ) () const,
            bool& isFound )
        {
            isFound = false;
            if( iter == endIter )
            {
                return;
            }
            auto& parent = *elementPtr;
            auto childPtr = ( ( parent ).*( getChildFunction ) )();

            if( iter->getName() == childPtr->getElementName() )
            {
                isFound = true;
                isSuccess &= childPtr->fromXElement( message, *iter );
                ++iter;
                hasIterBeenIncremented = true;
            }
        }


        template<typename PARENT_TYPE, typename CHILD_TYPE>
        void checkSetGroupMemberSetAndIncrementIter(
            std::ostream& message,
            ::ezxml::XElementIterator& iter,
            ::ezxml::XElementIterator& endIter,
            bool& isSuccess,
            bool& hasIterBeenIncremented,
            std::string elementName,
            std::shared_ptr<PARENT_TYPE>& parentPtr,
            const std::vector<std::shared_ptr<CHILD_TYPE>>& ( PARENT_TYPE:: *getConstSet ) () const,
            void ( PARENT_TYPE:: *addItem ) ( const std::shared_ptr<CHILD_TYPE>& ),
            void ( PARENT_TYPE:: *removeItem ) ( const typename std::vector<std::shared_ptr<CHILD_TYPE>>::const_iterator& ) )
        {
            if( iter == endIter )
            {
                return;
            }

            bool isFirstItemAdded = false;

            auto& parent = *parentPtr;
            const auto& set = ( ( parent ).*( getConstSet ) )();

            while( iter != endIter && iter->getName() == elementName )
            {
                auto item = std::make_shared<CHILD_TYPE>();
                isSuccess &= item->fromXElement( message, *iter );
                ++iter;
                hasIterBeenIncremented = true;

                if( !isFirstItemAdded && set.size() == 1 )
                {
                    ( ( parent ).*( addItem ) )( item );
                    ( ( parent ).*( removeItem ) )( set.cbegin() );
                    isFirstItemAdded = true;
                }
                else
                {
                    ( ( parent ).*( addItem ) )( item );
                    isFirstItemAdded = true;
                }
            }
        }


        bool isEndWithDecrementIfNeeded(
            ::ezxml::XElementIterator& iter,
            ::ezxml::XElementIterator& endIter,
            bool isDecrementNeeded );


        MX_FORWARD_DECLARE_ELEMENT( EditorialGroup );
        
        void importGroup(
            std::ostream& message,
            ::ezxml::XElementIterator& iter,
            ::ezxml::XElementIterator& endIter,
            bool& isSuccess,
            EditorialGroupPtr& group );
        
        MX_FORWARD_DECLARE_ELEMENT( EditorialVoiceGroup );
        
        void importGroup(
            std::ostream& message,
            ::ezxml::XElementIterator& iter,
            ::ezxml::XElementIterator& endIter,
            bool& isSuccess,
            EditorialVoiceGroupPtr& group );

        MX_FORWARD_DECLARE_ELEMENT( LayoutGroup );
        
        void importGroup(
            std::ostream& message,
            ::ezxml::XElementIterator& iter,
            ::ezxml::XElementIterator& endIter,
            bool& isSuccess,
            LayoutGroupPtr& group );

        MX_FORWARD_DECLARE_ELEMENT( DisplayStepOctaveGroup );
        
        void importGroup(
            std::ostream& message,
            ::ezxml::XElementIterator& iter,
            ::ezxml::XElementIterator& endIter,
            bool& isSuccess,
            DisplayStepOctaveGroupPtr& group,
            bool& hasGroup );
        
        MX_FORWARD_DECLARE_ELEMENT( SyllabicTextGroup );

        void importGroup(
            std::ostream& message,
            ::ezxml::XElementIterator& iter,
            ::ezxml::XElementIterator& endIter,
            bool& isSuccess,
            SyllabicTextGroupPtr& group );
        
        MX_FORWARD_DECLARE_ELEMENT( EditorialVoiceDirectionGroup );

        void importGroup(
            std::ostream& message,
            ::ezxml::XElementIterator& iter,
            ::ezxml::XElementIterator& endIter,
            bool& isSuccess,
            EditorialVoiceDirectionGroupPtr& group );
        
        MX_FORWARD_DECLARE_ELEMENT( BeatUnitGroup );

        void importGroup(
            std::ostream& message,
            ::ezxml::XElementIterator& iter,
            ::ezxml::XElementIterator& endIter,
            bool& isSuccess,
            BeatUnitGroupPtr& group );

        MX_FORWARD_DECLARE_ELEMENT( MetronomeRelationGroup );

        void importGroup(
            std::ostream& message,
            ::ezxml::XElementIterator& iter,
            ::ezxml::XElementIterator& endIter,
            bool& isSuccess,
            MetronomeRelationGroupPtr& group,
            bool& hasGroup );

        MX_FORWARD_DECLARE_ELEMENT( TimeSignatureGroup );

        void importGroup(
            std::ostream& message,
            ::ezxml::XElementIterator& iter,
            ::ezxml::XElementIterator& endIter,
            bool& isSuccess,
            TimeSignatureGroupPtr& group );

        MX_FORWARD_DECLARE_ELEMENT( MidiInstrument );

        void importGroup(
            std::ostream& message,
            ::ezxml::XElementIterator& iter,
            ::ezxml::XElementIterator& endIter,
            bool& isSuccess,
            MidiInstrumentPtr& group );

        MX_FORWARD_DECLARE_ELEMENT( CreditWordsGroup );

        void importGroup(
            std::ostream& message,
            ::ezxml::XElementIterator& iter,
            ::ezxml::XElementIterator& endIter,
            bool& isSuccess,
            CreditWordsGroupPtr& group );

/*
        void importGroup(
            std::ostream& message,
            ::ezxml::XElementIterator& iter,
            ::ezxml::XElementIterator& endIter,
            bool& isSuccess,
            BeatUnitPerPtr& group );
*/
        
        template<typename CHOICE_CLASS_TYPE, typename CHILD_TYPE>
        bool checkSetChoiceMember(
            std::ostream& message,
            ::ezxml::XElement& xelement,
            bool& isSuccess,
            std::shared_ptr<CHOICE_CLASS_TYPE>& elementPtr,
            std::string choiceElementName,
            std::shared_ptr<CHILD_TYPE> ( CHOICE_CLASS_TYPE:: *getFunction ) () const,
            int choiceValue )
        {
            if( xelement.getName() != choiceElementName )
            {
                return false;
            }
            using enumType = typename CHOICE_CLASS_TYPE::Choice;
            auto enumValue = static_cast<enumType>( choiceValue );
            auto& parent = *elementPtr;
            auto childPtr = ( ( parent ).*( getFunction ) )();
            isSuccess &= childPtr->fromXElement( message, xelement );
            parent.setChoice( enumValue );
            return true;
        }


        template <typename T>
        bool parseAttribute(
            std::ostream& message,
            ::ezxml::XAttributeIterator& iter,
            const std::string& className,
            bool& isSucces,
            T& assignmentTarget,
            bool& hasAttribute,
            const char* const attributeName,
            T (*parseFunction)( const std::string& ) )
        {
            if( iter->getName() != attributeName )
            {
                return false;
            }
            hasAttribute = true;
            assignmentTarget = parseFunction( iter->getValue() );
            if( iter->getValue() != toString( assignmentTarget ) )
            {
                isSucces = false;
                message << className << ": attribute '" << attributeName << "' parsing failed - the value '";
                message << iter->getValue() << "' was unrecognized - defaulting to value '";
                message << toString( assignmentTarget ) << "'";
            }
            return true;
        }


        class XsString;

        bool parseAttribute(
            std::ostream& message,
            ::ezxml::XAttributeIterator& iter,
            const std::string& className,
            bool& isSucces,
            XsString& assignmentTarget,
            bool& hasAttribute,
            const char* const attributeName );


        class Decimal;

        bool parseAttribute(
            std::ostream& message,
            ::ezxml::XAttributeIterator& iter,
            const std::string& className,
            bool& isSucces,
            Decimal& assignmentTarget,
            bool& hasAttribute,
            const char* const attributeName );


        class Integer;

        bool parseAttribute(
            std::ostream& message,
            ::ezxml::XAttributeIterator& iter,
            const std::string& className,
            bool& isSucces,
            Integer& assignmentTarget,
            bool& hasAttribute,
            const char* const attributeName );


        class Color;

        bool parseAttribute(
            std::ostream& message,
            ::ezxml::XAttributeIterator& iter,
            const std::string& className,
            bool& isSucces,
            Color& assignmentTarget,
            bool& hasAttribute,
            const char* const attributeName );


        class Date;

        bool parseAttribute(
            std::ostream& message,
            ::ezxml::XAttributeIterator& iter,
            const std::string& className,
            bool& isSucces,
            Date& assignmentTarget,
            bool& hasAttribute,
            const char* const attributeName );


        class FontSize;

        bool parseAttribute(
            std::ostream& message,
            ::ezxml::XAttributeIterator& iter,
            const std::string& className,
            bool& isSucces,
            FontSize& assignmentTarget,
            bool& hasAttribute,
            const char* const attributeName );


        class NumberOrNormal;

        bool parseAttribute(
            std::ostream& message,
            ::ezxml::XAttributeIterator& iter,
            const std::string& className,
            bool& isSucces,
            NumberOrNormal& assignmentTarget,
            bool& hasAttribute,
            const char* const attributeName );


        class PositiveIntegerOrEmpty;

        bool parseAttribute(
            std::ostream& message,
            ::ezxml::XAttributeIterator& iter,
            const std::string& className,
            bool& isSucces,
            PositiveIntegerOrEmpty& assignmentTarget,
            bool& hasAttribute,
            const char* const attributeName );


        class YesNoNumber;

        bool parseAttribute(
            std::ostream& message,
            ::ezxml::XAttributeIterator& iter,
            const std::string& className,
            bool& isSucces,
            YesNoNumber& assignmentTarget,
            bool& hasAttribute,
            const char* const attributeName );


        class CommaSeparatedText;

        bool parseAttribute(
            std::ostream& message,
            ::ezxml::XAttributeIterator& iter,
            const std::string& className,
            bool& isSucces,
            CommaSeparatedText& assignmentTarget,
            bool& hasAttribute,
            const char* const attributeName );


        class DistanceType;

        bool parseAttribute(
            std::ostream& message,
            ::ezxml::XAttributeIterator& iter,
            const std::string& className,
            bool& isSucces,
            DistanceType& assignmentTarget,
            bool& hasAttribute,
            const char* const attributeName );


        class LineWidthType;

        bool parseAttribute(
            std::ostream& message,
            ::ezxml::XAttributeIterator& iter,
            const std::string& className,
            bool& isSucces,
            LineWidthType& assignmentTarget,
            bool& hasAttribute,
            const char* const attributeName );
        

        class CommaSeparatedListOfPositiveIntegers;

        bool parseAttribute(
            std::ostream& message,
            ::ezxml::XAttributeIterator& iter,
            const std::string& className,
            bool& isSucces,
            CommaSeparatedListOfPositiveIntegers& assignmentTarget,
            bool& hasAttribute,
            const char* const attributeName );
    }
}
