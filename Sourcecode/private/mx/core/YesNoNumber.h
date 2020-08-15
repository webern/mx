// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

namespace mx
{
    namespace core
    {
documentation()
        class classname
        {
        public:
            explicit classname();
            // explicit classname( const Decimal& value );
            // explicit classname( const SomeEnumType value );
            variants_ctor_decl
            explicit classname( const std::string& value );
            // bool getIsSomeEnumType() const;
            // bool getIsNumber() const;
            variants_get_is_decl
            // void setValue( const SomeEnumType value );
            // void setValue( const Decimal& value );
            variants_set_decl
            // SomeEnumType getValueSomeEnumType() const;
            // Decimal getValueNumber() const;
            variants_get_decl
            bool parse( const std::string& value );
            
        private:
            std::variant<variants_template_decl> myValue;
        };
        
        std::string toString( const classname& value );
		std::ostream& toStream( std::ostream& os, const classname& value );
		std::ostream& operator<<( std::ostream& os, const classname& value );
    }
}