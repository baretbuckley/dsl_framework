#include <string>
#include <variant>
#include <type_traits>
#include <typeindex>

namespace ast {


    #define AST_STD_TYPES double, char*, bool

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
        Value(char *value) : instance_(value) {}
        Value(bool value) : instance_(value) {}

        template<typename T>
        requires IsAnyOf<T, AST_STD_TYPES>
        T read() {return std::get<T>(instance_);}
        
        typedef std::variant<AST_STD_TYPES> Variant;

        Variant instance_;
    };

}