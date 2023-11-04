#include <value.hpp>

#include <variant>
#include <iostream>

namespace ast {

    template<typename ... Ts>
    struct Visitor : Ts ... {
        using Ts::operator() ...;
    };
    template<class... Ts> Visitor(Ts...) -> Visitor<Ts...>;

    template<typename V, typename... Ts>
    struct OverloadVisitor : V, Ts... {
        using Ts::operator() ...;
        using V::operator();
    };
    template<class V, class... Ts> OverloadVisitor(V, Ts...) -> OverloadVisitor<V, Ts...>;


    template <typename T>
    std::string STR(T&& t) {
        std::ostringstream oss;
        oss << t;
        return oss.str();
    }

    auto TO_DOUBLE_VISITOR = Visitor {
        [](double v) -> double {return v;},
        [](std::string v) -> double {return 0;},
        [](bool v) -> double {return (double)v;}
    };
    double Value::toDouble() const {
        return std::visit(TO_DOUBLE_VISITOR, instance_);
    }

    auto TO_STRING_VISITOR = Visitor {
        [](double v) -> std::string {return STR(v);},
        [](std::string v) -> std::string {return v;},
        [](bool v) -> std::string {return (v)? "True" : "False";},
        [](auto) -> std::string {
            return "";
        }
    };
    std::string Value::toString() const {
        return std::visit(TO_STRING_VISITOR, instance_);
    }

    auto TO_BOOL_VISITOR = Visitor {
        [](double v) -> bool {return (bool)v;},
        [](std::string v) -> bool {return false;},
        [](bool v) -> bool {return v;}
    };
    bool Value::toBool() const {
        return std::visit(TO_BOOL_VISITOR, instance_);
    }
    

    auto BINARY_VISITOR = Visitor {
        [](auto, auto) -> Value {return Value(0.0f);}
    };

    auto UNARY_VISITOR = Visitor {
        [](auto) -> Value {return Value(0.0f);}
    };

    // Arithmetic -----------------------------------------

    auto PLUS_VISITOR = OverloadVisitor{
        BINARY_VISITOR,
        [](double lv, double rv) -> Value {return Value(lv + rv);}
    };
    Value Value::operator+(const Value& v) const {
        return std::visit(PLUS_VISITOR, instance_, v.instance_);
    }

    auto MINUS_VISITOR = OverloadVisitor{
        BINARY_VISITOR,
        [](double lv, double rv) -> Value {return Value(lv - rv);}
    };
    Value Value::operator-(const Value& v) const {
        return std::visit(MINUS_VISITOR, instance_, v.instance_);
    }

    auto MULTIPLY_VISITOR = OverloadVisitor{
        BINARY_VISITOR,
        [](double lv, double rv) -> Value {return Value(lv * rv);}
    };
    Value Value::operator*(const Value& v) const {
        return std::visit(MULTIPLY_VISITOR, instance_, v.instance_);
    }

    auto DIVIDE_VISITOR = OverloadVisitor{
        BINARY_VISITOR,
        [](double lv, double rv) -> Value {return Value(lv / rv);}
    };
    Value Value::operator/(const Value& v) const {
        return std::visit(DIVIDE_VISITOR, instance_, v.instance_);
    }

    auto INCREMENT_VISITOR = OverloadVisitor{
        UNARY_VISITOR,
        [](double v) -> Value {return Value(++v);}
    };
    Value Value::operator++() const {
        return std::visit(INCREMENT_VISITOR, instance_);
    }

    auto DECREMENT_VISITOR = OverloadVisitor{
        UNARY_VISITOR,
        [](double v) -> Value {return Value(--v);}
    };
    Value Value::operator--() const {
        return std::visit(DECREMENT_VISITOR, instance_);
    }

    auto NEGATION_VISITOR = OverloadVisitor{
        UNARY_VISITOR,
        [](double v) -> Value {return Value(-v);}
    };
    Value Value::operator-() const {
        return std::visit(NEGATION_VISITOR, instance_);
    }

    // Relational -----------------------------------------
    
    auto RATIONAL_VISITOR = Visitor {
        [](auto, auto) -> Value {return Value(false);}
    };

    auto EQUAL_COMP_VISITOR = OverloadVisitor{
        RATIONAL_VISITOR,
        [](double lv, double rv) -> Value {return Value(lv == rv);},
        [](std::string lv, std::string rv) -> Value {return Value(!lv.compare(rv));},
        [](bool lv, bool rv) -> Value {return Value(lv == rv);}
    };
    Value Value::operator==(const Value& v) const {
        return std::visit(EQUAL_COMP_VISITOR, instance_, v.instance_);
    }

    auto NOT_EQUAL_COMP_VISITOR = OverloadVisitor{
        RATIONAL_VISITOR,
        [](double lv, double rv) -> Value {return Value(lv != rv);},
        [](std::string lv, std::string rv) -> Value {return Value((bool)lv.compare(rv));},
        [](bool lv, bool rv) -> Value {return Value(lv != rv);}
    };
    Value Value::operator!=(const Value& v) const {
        return std::visit(NOT_EQUAL_COMP_VISITOR, instance_, v.instance_);
    }

    auto GREATER_COMP_VISITOR = OverloadVisitor{
        RATIONAL_VISITOR,
        [](double lv, double rv) -> Value {return Value(lv > rv);},
        [](std::string lv, std::string rv) -> Value {return Value(lv.compare(rv)>0);},
    };
    Value Value::operator>(const Value& v) const {
        return std::visit(GREATER_COMP_VISITOR, instance_, v.instance_);
    }

    auto LESS_COMP_VISITOR = OverloadVisitor{
        RATIONAL_VISITOR,
        [](double lv, double rv) -> Value {return Value(lv < rv);},
        [](std::string lv, std::string rv) -> Value {return Value(lv.compare(rv)<0);},
    };
    Value Value::operator<(const Value& v) const {
        return std::visit(LESS_COMP_VISITOR, instance_, v.instance_);
    }

    auto GREATER_EQUAL_COMP_VISITOR = OverloadVisitor{
        RATIONAL_VISITOR,
        [](double lv, double rv) -> Value {return Value(lv >= rv);},
        [](std::string lv, std::string rv) -> Value {return Value(lv.compare(rv)>=0);},
    };
    Value Value::operator>=(const Value& v) const {
        return std::visit(GREATER_EQUAL_COMP_VISITOR, instance_, v.instance_);
    }

    auto LESS_COMP_EQUAL_VISITOR = OverloadVisitor{
        RATIONAL_VISITOR,
        [](double lv, double rv) -> Value {return Value(lv <= rv);},
        [](std::string lv, std::string rv) -> Value {return Value(lv.compare(rv)<=0);},
    };
    Value Value::operator<=(const Value& v) const {
        return std::visit(LESS_COMP_EQUAL_VISITOR, instance_, v.instance_);
    }

}