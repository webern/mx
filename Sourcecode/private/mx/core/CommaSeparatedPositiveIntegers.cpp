// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/CommaSeparatedPositiveIntegers.h"
#include "mx/core/StringUtils.h"

namespace mx
{
    namespace core
    {
        inline std::set<int> copyPositives( const std::set<int>& input )
        {
            std::set<int> output;
            std::copy_if( input.cbegin(), input.cend(), std::inserter( output, output.begin() ),
                          [](const int i){ return i > 0; } );
            return output;
        }

        CommaSeparatedListOfPositiveIntegers::CommaSeparatedListOfPositiveIntegers()
        :myValues()
        ,myIsSpacingDesired( false )
        {

        }
        
        
        CommaSeparatedListOfPositiveIntegers::CommaSeparatedListOfPositiveIntegers( const StringType& value )
        :myValues()
        ,myIsSpacingDesired(false)
        {
            parse( value );
        }
        
        
        CommaSeparatedListOfPositiveIntegers::CommaSeparatedListOfPositiveIntegers( const std::set<int>& values )
        :myValues()
        {
            setValues( values );
        }
        
        
        const std::set<int>& CommaSeparatedListOfPositiveIntegers::getValues() const
        {
            return myValues;
        }


        std::set<int>::iterator CommaSeparatedListOfPositiveIntegers::getValuesBegin()
        {
            return myValues.begin();
        }


        std::set<int>::iterator CommaSeparatedListOfPositiveIntegers::getValuesEnd()
        {
            return myValues.end();
        }


        std::set<int>::const_iterator CommaSeparatedListOfPositiveIntegers::getValuesBeginConst() const
        {
            return myValues.cbegin();
        }


        std::set<int>::const_iterator CommaSeparatedListOfPositiveIntegers::getValuesEndConst() const
        {
            return myValues.cend();
        }
        
        
        void CommaSeparatedListOfPositiveIntegers::setValues( const std::set<int>& values )
        {
            myValues = copyPositives( values );
        }
        
        
        void CommaSeparatedListOfPositiveIntegers::parse( const StringType& commaSeparatedText )
        {
            StringType cleaned = onlyAllow( commaSeparatedText, "", "1234567890,-" );
            myValues.clear();
            std::istringstream iss( cleaned );
            std::string token;
            while( std::getline( iss, token, ',' ) )
            {
                if ( token.length() > 0 )
                {
                    std::stringstream ss( token );
                    int value = 1;
                    ss >> value;
                    if ( value > 0 )
                    {
                        myValues.insert( value );
                    }
                }
            }
        }
        
        
        void CommaSeparatedListOfPositiveIntegers::setUseSpaceBetweenItems( bool value )
        {
            myIsSpacingDesired = value;
        }
        
        
        bool CommaSeparatedListOfPositiveIntegers::getUseSpaceBetweenItems() const
        {
            return myIsSpacingDesired;
        }


        std::ostream& toStream( std::ostream& os, const CommaSeparatedListOfPositiveIntegers& value )
        {
            bool isfirst = true;
            for ( auto x : value.getValues() )
            {
                if( isfirst )
                {
                    os << x;
                    isfirst = false;
                }
                else
                {
                    os << ",";
                    if( value.getUseSpaceBetweenItems() )
                    {
                        os << " ";
                    }
                    os << x;
                }
            }
            return os;
        }


        std::ostream& operator<<( std::ostream& os, const CommaSeparatedListOfPositiveIntegers& value )
        {
            return toStream( os, value );
        }

        
        StringType toString( const CommaSeparatedListOfPositiveIntegers& value )
        {
            std::stringstream ss;
            toStream( ss, value );
            return ss.str();
        }
    }
}
