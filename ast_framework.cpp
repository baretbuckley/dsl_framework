#include <iostream>
#include "value.hpp"
#include <typeindex>
#include <instruction_base.hpp>
#include <tokenizer.hpp>
#include <sr_parser.hpp>
#include <token.hpp>

#include <sstream>

#include <string>

#include <typeindex>

#define PROJECT_NAME "ast_framework"

void loadRule(ast::srp::Parser &parser, std::vector<uint32_t> rule, uint32_t result) {
    std::vector<ast::TokenID> tokenRule;
    for (auto id : rule) 
        tokenRule.push_back(ast::TokenID(id));
    parser.loadRule(tokenRule, ast::TokenID(result));
}

// void printTree(ast::srp::VariableTree::Iterator it) {
    
// }

int main(int argc, char **argv) {


    // ast::Variable valA(0.5);
    // ast::Variable valB(1.9);
    // /**
    //  * Aprox:
    //  * if a <= b:
    //  *   b = b-a
    //  * else:
    //  *   b = b + (a*a)
    //  *   a = a+1
    // */
    // ast::Expression *exp = new ast::IfConditional(
    //     new ast::CompEqualLess(&valA, &valB),
    //     new ast::SetVariable(&valB,
    //         new ast::Subtraction(&valB, &valA)),
    //     new ast::CodeBlock({
    //         new ast::SetVariable(&valA, new ast::Addition(&valA, new ast::Constant(1.0))),
    //         new ast::SetVariable(&valB,
    //             new ast::Addition(&valB,
    //                 new ast::Multiplication(&valA, new ast::Constant(2.0))))
    //     })
    // );
    // for (int i = 0; i < 10; i++)
    //     std::cout << exp->evaluate() << ' ' << valA.val << ' ' << valB.val << '\n';


    // const char data[] = 
    //     "testing this if (next) {\n"
    //     "   and this call(end)\n"
    //     "}";
    // std::cout << "loading data: \n" << data << '\n';
    
    
    // ast::Tokenizer tokenizer;
    // std::string test("if");
    // tokenizer.loadKeyword(test);
    // tokenizer.loadKeyword("this");
    // tokenizer.loadKeyword("next");
    // tokenizer.loadKeyword("call");
    // tokenizer.loadKeyword("end");
    // tokenizer.loadSymbol("(");
    // tokenizer.loadSymbol(")");
    // tokenizer.loadSymbol("{");
    // tokenizer.loadSymbol("}");
    // tokenizer.loadWhiteSpace(' ');
    // tokenizer.loadWhiteSpace('\n');


    // std::istringstream stream(data);
    // std::vector<ast::Token> tokens;
    // tokenizer.tokenize(tokens, stream);
    // std::cout << tokens.size() << '\n';
    // for (auto t : tokens) {
    //     if (t.type() == ast::Token::Keyword)
    //         std::cout << t.id() << ' ';
    //     else std::cout << '"' << t.string() << "\" ";
    // }

    //        r
    //    0       4
    //  2   3   1   9
    // 8     5       6
    // auto tree = new ast::srp::VariableTree();
    // loadRule(tree, {0, 2}, 8);
    // loadRule(tree, {0, 3}, 5);
    // loadRule(tree, {4}, 1);
    // loadRule(tree, {4, 9}, 6);
    // std::cout << "loaded rules\n";
    // ast::srp::VariableTree::Iterator it = tree->iterator();
    // it.print();

    

    std::stringstream input("5 + 2");
    auto idMap = std::make_shared<ast::TokenMap>();
    ast::Tokenizer tokenizer(idMap);
    tokenizer.loadWhiteSpace(' ');
    tokenizer.loadWhiteSpace('\n');
    tokenizer.loadKeyword("0");
    tokenizer.loadKeyword("1");
    tokenizer.loadKeyword("2");
    tokenizer.loadKeyword("3");
    tokenizer.loadKeyword("4");
    tokenizer.loadKeyword("5");
    tokenizer.loadKeyword("6");
    tokenizer.loadKeyword("7");
    tokenizer.loadKeyword("8");
    tokenizer.loadKeyword("9");
    tokenizer.loadSymbol("+");
    tokenizer.loadSymbol("*");

    std::vector<ast::Token> tokens;
    tokenizer.tokenize(tokens, input);

    for (auto t : tokens) {
        if (t.type() == ast::Token::Keyword)
            std::cout << t.id() << ' ';
        else std::cout << '"' << t.string() << "\" ";
    }
    std::cout << std::endl;

    for (auto t : tokens) {
        if (t.type() == ast::Token::Keyword)
            std::cout << idMap->getKeyword(t.id()) << ' ';
        else std::cout << '"' << t.string() << "\" ";
    }
    std::cout << std::endl;

    // ast::srp::Parser parser;





    // E -> T + E | T 
    // T -> V * T | V
    // V -> #

    ast::srp::Parser parser;
    idMap->loadKey("E");
    idMap->loadKey("T");
    idMap->loadKey("V");
    std::cout << idMap->keyID("T") << ' ' << idMap->tokenID("+") << ' ' << idMap->keyID("E") << ' ' << idMap->keyID("E") << std::endl;
    loadRule(parser, {idMap->tokenID("0")}, idMap->keyID("V"));
    loadRule(parser, {idMap->tokenID("1")}, idMap->keyID("V"));
    loadRule(parser, {idMap->tokenID("2")}, idMap->keyID("V"));
    loadRule(parser, {idMap->tokenID("3")}, idMap->keyID("V"));
    loadRule(parser, {idMap->tokenID("4")}, idMap->keyID("V"));
    loadRule(parser, {idMap->tokenID("5")}, idMap->keyID("V"));
    loadRule(parser, {idMap->tokenID("6")}, idMap->keyID("V"));
    loadRule(parser, {idMap->tokenID("7")}, idMap->keyID("V"));
    loadRule(parser, {idMap->tokenID("8")}, idMap->keyID("V"));
    loadRule(parser, {idMap->tokenID("9")}, idMap->keyID("V"));

    loadRule(parser, {idMap->keyID("V")}, idMap->keyID("T"));
    loadRule(parser, {idMap->keyID("V"), idMap->tokenID("*"), idMap->keyID("T")}, idMap->keyID("T"));

    loadRule(parser, {idMap->keyID("T")}, idMap->keyID("E"));
    loadRule(parser, {idMap->keyID("E"), idMap->tokenID("+"), idMap->keyID("T")}, idMap->keyID("E"));

    parser.tmpPrintTree(*idMap);
    std::cout << "parsing\n";
    parser.parse(tokens);
    std::cout << "Finished parse\n";

    
    


}