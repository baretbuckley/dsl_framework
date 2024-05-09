#pragma once

#include <string>
#include <variant>
#include <type_traits>
#include <typeindex>
#include <sstream>
#include <cstring>

namespace ast {


    

    // Concept to check if type is in given list of types
    template<typename T, typename ... U>
    concept IsAnyOf = (std::same_as<T, U> || ...); 


    // Template structure for getting index of type in type pack
    template<typename T, typename... U>
    struct IndexOfType {
        private:
        template<int index, typename U_>
        constexpr static int worker() {
            return -1;
        }
        template<int index, typename U_, typename V0, typename... V>
        constexpr static int worker() {
            return (std::is_same<U_, V0>::value)?
                        index :
                        worker<index+1, U_, V...>();
        }
        
        
        public:
        constexpr static int value = worker<0, T, U...>();
    };


    class Value {

        public:

        class Null {
            const char *cause_;
            public:
            Null(const char *cause);
            Null(const std::string &cause);
            const char *cause() {return cause_;}
            std::string toString();
        };

        #define AST_STD_TYPES std::monostate, Null, double, std::string, bool
        typedef std::variant<AST_STD_TYPES> Variant;
        enum class Type {
            Void=0,
            Null=1,
            Number=2,
            String=3,
            Bool=4,
        };
        Type type();
        static std::string typeToString(Type t);
        template<typename T>
        static constexpr std::string typeToString() {
            return typeToString(static_cast<Type>(IndexOfType<T, AST_STD_TYPES>::value));
        }
        std::string typeAsString();

        Value() : instance_() {}
        Value(Null value) : instance_(value) {}
        Value(double value) : instance_(value) {}
        Value(std::string value) : instance_(value) {}
        Value(bool value) : instance_(value) {}

        

        template<typename T>
        requires IsAnyOf<T, AST_STD_TYPES>
        T get() {return std::get<T>(instance_);}

        // conversions
        double toDouble() const;
        std::string toString() const;
        bool toBool() const;
        
        
        // Simple operators
        // Arithmetic
        Value operator+(const Value& v) const;
        Value operator-(const Value& v) const;
        Value operator*(const Value& v) const;
        Value operator/(const Value& v) const;
        Value operator%(const Value& v) const = delete; // Integers not supported
        Value operator-() const;
        // Relational
        Value operator==(const Value& v) const;
        Value operator!=(const Value& v) const;
        Value operator<(const Value& v) const;
        Value operator<=(const Value& v) const;
        Value operator>=(const Value& v) const;
        Value operator>(const Value& v) const;
        // Logical
        Value operator&&(const Value &v) const;
        Value operator||(const Value &v) const;
        Value operator!() const;
        // Bitwise (Not supported as currently integers aren't)
        Value operator^(const Value& v) const=delete;
        Value operator&(const Value& v) const=delete;
        Value operator|(const Value& v) const=delete;
        Value operator~() const=delete;
        // Array accessor
        Value operator[](size_t idx) const = delete; // Arrays not supported
        Value operator[](const Value& v) const = delete; // Arrays not supported

        
        friend std::ostream& operator<<(std::ostream& stream, const Value& value) {
            return stream << value.toString();
        }


        private:
        
        Variant instance_;
    };

    std::ostream &operator<<(std::ostream &os, Value::Type t);

}