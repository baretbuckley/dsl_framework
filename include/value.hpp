#include <string>
#include <variant>
#include <type_traits>
#include <typeindex>
#include <sstream>

namespace ast {


    

    // Concept to check if type is in given list of types
    template<typename T, typename ... U>
    concept IsAnyOf = (std::same_as<T, U> || ...); 


    // Template structure for getting index of type in type pack
    template<typename T, typename... U>
    struct IndexOfType {
        private:
        template<int index, typename Ti, typename Tc, typename... V>
        struct Worker {
            constexpr static int
            value = (sizeof...(V) == 0)? -1 :
                    (std::is_same<Ti, Tc>::value)?
                        index :
                        Worker<index+1, Ti, V...>::value;
        };
        public:
        constexpr static int value = Worker<0, T, U...>::value;
    };


    class Value {

        public:

        class Null {
            const char *cause_;
            public:
            Null(const char *cause) : cause_(cause) {}
            const char *cause() {return cause_;}
            std::string toString() {
                std::string ret;
                ret.append("Null Value: \"");
                ret.append(cause_);
                ret.append("\"");
                return ret;
            }
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
        static std::string typeToString() {
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
        
        Variant instance_;
    };

    std::ostream &operator<<(std::ostream &os, Value::Type t);

}