// MusicXML Class Library v0.3.0
// Copyright (c) 2015 - 2016 by Matthew James Briggs

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <set>

namespace mx
{
    namespace core
    {
        /* Ostensibly you could change the type of
         string being used for locality.  However,
         I am very American and have no idea what
         would happen, the universe may implode. */
        using StringType = std::string;
        
        class XsString
        {
        public:
            XsString();
            XsString( const StringType& value );
            virtual ~XsString();
            StringType getValue() const;
            virtual void setValue( const StringType& value );
        private:
            StringType myValue;
        };
        
        StringType toString( const XsString& xsstring );
        std::ostream& toStream( std::ostream& os, const XsString& xsstring, bool useXmlEscapeCharacters = true );
        std::ostream& operator<<( std::ostream& os, const XsString& xsstring );
        
        class XsToken : public XsString
        {
        public:
            XsToken();
            XsToken( const StringType& value );
            virtual void setValue( const StringType& value );
        };
        
        using XsNMToken = XsToken;
        
        using XsAnyUri = XsString;
        
        class XsID : public XsString
        {
        public:
            XsID();
            XsID( const StringType& value );
            virtual void setValue( const StringType& value );
        };
        
        using XsIDREF = XsID;
        
        /* Add restrictions to these later if needed */
        using XmlLang = XsString;
        using XsAnyUri = XsString;
        using XlinkHref = XsAnyUri;
        using XlinkRole = XsToken;
        using XlinkTitle = XsToken;
        
        using XsTokenSet = std::vector<XsToken>;
        using XsTokenSetIter = XsTokenSet::iterator;
        using XsTokenSetIterConst = XsTokenSet::const_iterator;
        
        class CommaSeparatedText
        {
        public:
            CommaSeparatedText();
            CommaSeparatedText( const StringType& value );
            CommaSeparatedText( const XsTokenSet& values );
            const XsTokenSet& getValues() const;
            XsTokenSetIter getValuesBegin();
            XsTokenSetIter getValuesEnd();
            XsTokenSetIterConst getValuesBeginConst() const;
            XsTokenSetIterConst getValuesEndConst() const;
            void setValues( const XsTokenSet& values );
            void parse( const StringType& commaSeparatedText );
            
        private:
            XsTokenSet myValues;
        
        };
        
        StringType toString( const CommaSeparatedText& value );
        std::ostream& toStream( std::ostream& os, const CommaSeparatedText& value );
        std::ostream& operator<<( std::ostream& os, const CommaSeparatedText& value );
        
        class CommaSeparatedListOfPositiveIntegers
        {
        public:
            CommaSeparatedListOfPositiveIntegers();
            CommaSeparatedListOfPositiveIntegers( const StringType& value );
            CommaSeparatedListOfPositiveIntegers( const std::set<int>& values );
            const std::set<int>& getValues() const;
            std::set<int>::iterator getValuesBegin();
            std::set<int>::iterator getValuesEnd();
            std::set<int>::const_iterator getValuesBeginConst() const;
            std::set<int>::const_iterator getValuesEndConst() const;
            void setValues( const std::set<int>& values );
            void parse( const StringType& commaSeparatedText );
            void setUseSpaceBetweenItems( bool value );
            bool getUseSpaceBetweenItems() const;
        private:
            std::set<int> myValues;
            bool myIsSpacingDesired;
        };
        
        StringType toString( const CommaSeparatedListOfPositiveIntegers& value );
        std::ostream& toStream( std::ostream& os, const CommaSeparatedListOfPositiveIntegers& value );
        std::ostream& operator<<( std::ostream& os, const CommaSeparatedListOfPositiveIntegers& value );
        
        using EndingNumber = CommaSeparatedListOfPositiveIntegers;
        using TimeOnly = CommaSeparatedListOfPositiveIntegers;
        
    }
}
