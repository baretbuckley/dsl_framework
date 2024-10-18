
#include "instruction_base.hpp"

namespace dsll {

class Function {
    const ast::Expression *functionStart_;

    Function(const ast::Expression *fn) : functionStart_(fn) {}

    ast::Value execute() {return functionStart_->evaluate();}

    ast::Value operator()() {return execute();}
};

}