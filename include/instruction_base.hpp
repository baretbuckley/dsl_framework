#include "value.hpp"

#include <typeinfo>
#include <typeindex>
#include <vector>
#include <cstdlib>


namespace ast {

    class Expression {
        public:
        virtual Value evaluate()=0;
    };

    class CodeBlock : public Expression {
        std::vector<Expression*> instructions_;

        public:
        
        CodeBlock(std::initializer_list<Expression*> e)
            : instructions_(e) 
        {}
        void pushInstruction(Expression *e) {
            instructions_.push_back(e);
        }

        Value evaluate() override {
            for (auto node : instructions_)
                node->evaluate();
            return Value();
        }
    };

    // Data -------------------------------------------------------------------

    struct Constant : public Expression {
        private:
        Value val;
        public:
        template<typename T>
        Constant(T value) : val(value) {}
        Value evaluate() override {return val;}
    };

    struct Variable : public Expression {
        Value val;
        template<typename T>
        Variable(T value) : val(value) {}
        Value evaluate() override {return val;}
    };

    class SetVariable : public Expression {
        Variable *var_;
        Expression *source_;
        public:
        SetVariable(Variable *variable, Expression *source) :
            var_(variable), source_(source) {}
        Value evaluate() override {
            return var_->val = source_->evaluate();
        }
    };

    class TypedSetVariable : public Expression {
        Variable *var_;
        Expression *source_;
        public:
        TypedSetVariable(Variable *variable, Expression *source) :
            var_(variable), source_(source) {}
        Value evaluate() override {
            Value val = source_->evaluate();
            if (val.type() != var_->val.type())
                return Value(Value::Null("Error type mismatch in assignment."));
            return var_->val = val;
        }
    };

    // Control Flow

    class ControlFlow : public Expression {};

    template<class T>
    class Else : public T, public ControlFlow {};

    class IfConditional : public ControlFlow {
        Expression *condition_;
        Expression *result_;
        Expression *chainedControl_;
        public:
        IfConditional(Expression *cond, Expression *result, Expression *chain=nullptr)
            : condition_(cond)
            , result_(result)
            , chainedControl_(chain)
        {}
        Value evaluate() override {
            if (condition_->evaluate().toBool())
                return result_->evaluate();
            else if (chainedControl_)
                return chainedControl_->evaluate();
            return Value();
        }
    };

    class WhileLoop : public ControlFlow {
        Expression *condition_;
        Expression *result_;
        Expression *chainedControl_;
        public:
        WhileLoop(Expression *cond, Expression *result, Expression *chain=nullptr)
            : condition_(cond)
            , result_(result)
            , chainedControl_(chain)
        {}
        Value evaluate() override {
            while (condition_->evaluate().toBool())
                return result_->evaluate();
            if (chainedControl_)
                return chainedControl_->evaluate();
            return Value();
        }
    };

    // Rudimentary Expressions ----------------------------

    // Binary Instruction base

    class BinaryExpression_base : public Expression {
        protected:
        Expression *left_;
        Expression *right_;
        public:
        BinaryExpression_base(Expression *left, Expression *right)
            : left_(left), right_(right)
        {}
    };
    #define BINARY_EXPRESSION(op) : public BinaryExpression_base {\
        public:\
        using BinaryExpression_base::BinaryExpression_base;\
        Value evaluate() override {\
            return Value(left_->evaluate() op right_->evaluate());\
        }\
    }

    class MonaryExpression_base : public Expression {
        protected:
        Expression *val_;
        public:
        MonaryExpression_base(Expression *val)
            : val_(val)
        {}
    };
    #define MONARY_EXPRESSION(op) : public MonaryExpression_base {\
        public:\
        using MonaryExpression_base::MonaryExpression_base;\
        Value evaluate() override {\
            return Value(op val_->evaluate());\
        }\
    }

    // Arithmatic -------------------------------
    class Addition BINARY_EXPRESSION(+);
    class Subtraction BINARY_EXPRESSION(-);
    class Multiplication BINARY_EXPRESSION(*);
    class Division BINARY_EXPRESSION(/);
    class Negation MONARY_EXPRESSION(-);

    // Relational -------------------------------
    class CompEquals BINARY_EXPRESSION(==);
    class CompNotEquals BINARY_EXPRESSION(!=);
    class CompLess BINARY_EXPRESSION(<);
    class CompEqualLess BINARY_EXPRESSION(<=);
    class CompEqualGreater BINARY_EXPRESSION(>=);
    class CompGreater BINARY_EXPRESSION(>);

    // Logical ----------------------------------
    class LogicAnd BINARY_EXPRESSION(&&);
    class LogicOr BINARY_EXPRESSION(||);
    class LogicNot MONARY_EXPRESSION(!);


}