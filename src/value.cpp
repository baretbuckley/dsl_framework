#include <value.hpp>

#include <variant>
#include <iostream>
#include <concepts>

namespace ast {

    // Helper functions


    inline std::string STR(std::ostringstream& oss) {
        auto s = oss.str();
        oss.str(""); // clear the string buffer for next STR call
        oss.clear(); // reset stream error state for next STR call
        return s;
    }

    template <typename T, typename ... Args>
    std::string STR(std::ostringstream& oss, T&& t, Args&& ... args) {
        oss << t;
        return STR(oss, std::forward<Args>(args)...);
    }

    template <typename T, typename ... Args>
    std::string STR(T&& t, Args&& ... args) {
        // using thread_local here so that recursive template does not instantiate excessive ostringstreams
        thread_local std::ostringstream oss;
        oss << t;
        return STR(oss, std::forward<Args>(args)...);
    }

    template<typename T, typename V>
    Value UndefinedOperation(T, const char * op, V) {
        return Value(Value::Null(STR(
                "Undefined operation: ( ",
                Value::typeToString<T>(),
                ' ', op, ' ',
                Value::typeToString<V>(), " )"))); 
    }

    // Null class definition -----------------------

    Value::Null::Null(const char *cause)
        : cause_(cause) {}

    Value::Null::Null(const std::string &cause)
        : cause_(strcpy(new char[cause.size()+1], cause.c_str()))
    {}

    std::string Value::Null::toString() {
        std::string ret;
        ret.append("Null Value: \"");
        ret.append(cause_);
        ret.append("\"");
        return ret;
    }

    // Type accessors and helper functions

    Value::Type Value::type() {
        return static_cast<Type>(instance_.index());
    }
    
    std::string Value::typeToString(Value::Type t) {
        switch(t) {
            case Value::Type::Void: return "Void";
            case Value::Type::Null: return "Null";
            case Value::Type::Number: return "Number";
            case Value::Type::String: return "String";
            case Value::Type::Bool: return "Bool";
            default: return "Unsupported Type";
        }
    }

    std::string Value::typeAsString() {
        return typeToString(type());
    }


    // Conversion functions

    struct ToDouble_visitor {
        template<typename T>
        double operator()(T&) const {return 0.0;}
        double operator()(double &v) const {return v;}
    };
    double Value::toDouble() const {
        return std::visit(ToDouble_visitor(), instance_);
    }

    struct ToString_visitor {
        std::string operator()(std::monostate)  {return "Void";}
        std::string operator()(Value::Null v) {return v.toString();}
        std::string operator()(double v) {return STR(v);}
        std::string operator()(std::string v) {return v;}
        std::string operator()(bool v) {return (v)? "True" : "False";}
    };
    std::string Value::toString() const {
        return std::visit(ToString_visitor(), instance_);
    }

    struct ToBool_visitor {
        bool operator()(std::monostate) const {return false;}
        bool operator()(Value::Null) const {return false;}
        bool operator()(double v) const {return (bool)v;}
        bool operator()(std::string v) const {return v.empty();}
        bool operator()(bool v) const {return v;}
    };
    bool Value::toBool() const {
        return std::visit(ToBool_visitor(), instance_);
    }


    // Arithmetic -----------------------------------------

    struct Plus_visitor {
        Value operator()(const double lv, const double rv) const {
            return Value(lv + rv);
        }
        template<typename T, typename U>
        Value operator()(T &lv, U &rv) const {
            return UndefinedOperation(lv, "+", rv);
        }
    }; 
    Value Value::operator+(const Value& v) const {
        return std::visit(Plus_visitor(), instance_, v.instance_);
    }

    struct Minus_visitor {
        Value operator()(const double &lv, const double &rv) const {
            return Value(lv - rv);
        }
        template<typename T, typename U>
        Value operator()(T& lv, U& rv) const {
            return UndefinedOperation(lv, "-", rv);
        }
    };
    Value Value::operator-(const Value& v) const {
        return std::visit(Minus_visitor(), instance_, v.instance_);
    }

    struct Multiply_visitor {
        Value operator()(const double &lv, const double &rv) const {
            return Value(lv * rv);
        }
        template<typename T, typename U>
        Value operator()(T& lv, U& rv) const {
            return UndefinedOperation(lv, "*", rv);
        }
    };
    Value Value::operator*(const Value& v) const {
        return std::visit(Multiply_visitor(), instance_, v.instance_);
    }

    struct Divide_visitor {
        Value operator()(const double &lv, const double &rv) const {
            return Value(lv / rv);
        }
        template<typename T, typename U>
        Value operator()(T& lv, U& rv) const {
            return UndefinedOperation(lv, "/", rv);
        }
    };
    Value Value::operator/(const Value& v) const {
        return std::visit(Divide_visitor(), instance_, v.instance_);
    }

    struct Negation_visitor {
        Value operator()(double v) const {
            return Value(-v);
        }
        template<typename T>
        Value operator()(T) const {
            return STR("Undefined Operation: ( -",
                        Value::typeToString<T>(), " )");
        }
    };
    Value Value::operator-() const {
        return std::visit(Negation_visitor(), instance_);
    }

    // Relational -----------------------------------------


    struct EqualComp_visitor {
        template<std::equality_comparable T>
        Value operator()(const T lv, const T rv) const {
            return Value(lv == rv);
        }
        template<typename T, typename U>
        Value operator()(T lv, U rv) const {
            return UndefinedOperation(lv, "==", rv);
        }
    };
    Value Value::operator==(const Value& v) const {
        return std::visit(EqualComp_visitor(), instance_, v.instance_);
    }

    struct NotEqualComp_visitor {
        template<std::equality_comparable T>
        Value operator()(const T lv, const T rv) const {
            return Value(lv != rv);
        }
        template<typename T, typename U>
        Value operator()(T lv, U rv) const {
            return UndefinedOperation(lv, "!=", rv);
        }
    };
    Value Value::operator!=(const Value& v) const {
        return std::visit(NotEqualComp_visitor(), instance_, v.instance_);
    }

    struct GreaterComp_visitor {
        template<std::totally_ordered T>
        Value operator()(const T lv, const T rv) const {
            return Value(lv > rv);
        }
        template<typename T, typename U>
        Value operator()(T lv, U rv) const {
            return UndefinedOperation(lv, ">", rv);
        }
    };
    Value Value::operator>(const Value& v) const {
        return std::visit(GreaterComp_visitor(), instance_, v.instance_);
    }

    struct LessComp_visitor {
        template<std::totally_ordered T>
        Value operator()(const T lv, const T rv) const {
            return Value(lv < rv);
        }
        template<typename T, typename U>
        Value operator()(T lv, U rv) const {
            return UndefinedOperation(lv, "<", rv);
        }
    };
    Value Value::operator<(const Value& v) const {
        return std::visit(LessComp_visitor(), instance_, v.instance_);
    }

    struct GreaterEqualComp_visitor {
        template<std::totally_ordered T>
        Value operator()(const T lv, const T rv) const {
            return Value(lv >= rv);
        }
        template<typename T, typename U>
        Value operator()(T lv, U rv) const {
            return UndefinedOperation(lv, ">=", rv);
        }
    };
    Value Value::operator>=(const Value& v) const {
        return std::visit(GreaterEqualComp_visitor(), instance_, v.instance_);
    }

    struct LessEqualComp_visitor {
        template<std::totally_ordered T>
        Value operator()(const T lv, const T rv) const {
            return Value(lv <= rv);
        }
        template<typename T, typename U>
        Value operator()(T lv, U rv) const {
            return UndefinedOperation(lv, "<=", rv);
        }
    };
    Value Value::operator<=(const Value& v) const {
        return std::visit(LessEqualComp_visitor(), instance_, v.instance_);
    }

    // Logical --------------------------------------------


    struct LogicalAnd_visitor {
        template<typename T, typename U>
        requires std::convertible_to<T, bool> &&
                 std::convertible_to<U, bool>
        Value operator()(const T lv, const U rv) const {
            return Value(lv && rv);
        }
        template<typename T, typename U>
        Value operator()(T lv, U rv) const {
            return UndefinedOperation(lv, "&&", rv);
        }
    };
    Value Value::operator&&(const Value& v) const {
        return std::visit(LogicalAnd_visitor(), instance_, v.instance_);
    }

    struct LogicalOr_visitor {
        template<typename T, typename U>
        requires std::convertible_to<T, bool> &&
                 std::convertible_to<U, bool>
        Value operator()(const T lv, const U rv) const {
            return Value(lv || rv);
        }
        template<typename T, typename U>
        Value operator()(T lv, U rv) const {
            return UndefinedOperation(lv, "||", rv);
        }
    };
    Value Value::operator||(const Value& v) const {
        return std::visit(LogicalOr_visitor(), instance_, v.instance_);
    }

    struct LogicalNot_visitor {
        template<typename T>
        requires std::convertible_to<T, bool>
        Value operator()(const T v) const {
            return Value(!v);
        }
        template<typename T>
        Value operator()(T) const {
            return STR("Undefined Operation: ( !",
                        Value::typeToString<T>(), " )");
        }
    };
    Value Value::operator!() const {
        return std::visit(LogicalNot_visitor(), instance_);
    }


    
    std::ostream &operator<<(std::ostream &os, Value::Type t) {
       return os << Value::typeToString(t);
    }
}