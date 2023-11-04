#include <string>
#include <variant>
#include <type_traits>
#include <typeindex>
#include <sstream>

namespace ast {


    #define AST_STD_TYPES double, std::string, bool

    // Concept to check if type is in given list of types
    template<typename T, typename ... U>
    concept IsAnyOf = (std::same_as<T, U> || ...); 



    class Value {

        public:

        enum class Type {
            Number,
            String,
            Bool
        };

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
        Value operator++() const;
        Value operator--() const;
        Value operator-() const;
        // Relational
        Value operator==(const Value& v) const;
        Value operator!=(const Value& v) const;
        Value operator<(const Value& v) const;
        Value operator<=(const Value& v) const;
        Value operator>=(const Value& v) const;
        Value operator>(const Value& v) const;
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
        typedef std::variant<AST_STD_TYPES> Variant;
        Variant instance_;
    };

}