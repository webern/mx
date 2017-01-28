// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include <iostream>
#include <memory>

namespace mx
{
    namespace core
    {
        class Date
        {
        public:
            Date( int year_in, int month_in, int day_in );
            Date(); // defaults to 1900-01-01
            Date( const std::string& yyyy_mm_dd ); // e.g. "2014-07-01"
            virtual ~Date();
            Date( const Date& other );
            Date( Date&& other );
            Date& operator=( const Date& other );
            Date& operator=( Date&& other );
            
            /** Returns the year as an int. **/
            int getYear() const;
            
            /** Returns the month as an int. **/
            int getMonth() const;
            
            /** Returns the day as an int. **/
            int getDay() const;
            
            /** Returns true if the contained year is a leap year **/
            bool isLeapYear() const;
            
            /** Returns true if the passed year is a leap year **/
            bool isLeapYear( int year_in ) const;
            
            /** Returns the number of days in the current month **/
            int daysInMonth() const;
            
            /** Returns the number of days in the given month/year **/
            int daysIntMonth( int month_in, int year_in ) const;
            
            /* gets the day of the week where 1 = Sunday, 2 = Monday,
             3 = Tuesday, 4 = Wednesday, 5 = Thursday, 6 = Friday,
             7 = Saturday */
            int getWeekday() const;
            
            bool setValue( int year_in, int month_in, int day_in );
            void setYear( int year_in );
            void setMonth( int month_in );
            void setDay( int day_in );
            bool parse( const std::string& value_in );
            
            
            bool operator==( const Date& other_in ) const;
            bool operator!=( const Date& other_in ) const;
            bool operator>( const Date& other_in ) const;
            bool operator<( const Date& other_in ) const;
            bool operator>=( const Date& other_in ) const;
            bool operator<=( const Date& other_in ) const;
            
            
            Date& operator++();
            Date operator++( int );
            Date& operator--();
            Date operator--( int );

            Date& addDays( int days_in );
            Date& addMonths( int months_in );
            Date& addYears( int years_in );
            
        private:
            class impl;
            std::unique_ptr<impl> myImpl;
            
        };
        
        std::string toString( const Date& value );
        std::ostream& toStream( std::ostream& os, const Date& value );
        std::ostream& operator<<( std::ostream& os, const Date& value );
    }
}
