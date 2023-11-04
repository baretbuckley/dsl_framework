#include <iostream>
#include "value.hpp"
#include <typeindex>

#include <string>

#define PROJECT_NAME "ast_framework"

int main(int argc, char **argv) {
    if(argc != 1) {
        std::cout << argv[0] <<  "takes no arguments.\n";
        return 1;
    }
    std::cout << "This is project " << PROJECT_NAME << ".\n";

    ast::Value val1(0.4);
    ast::Value val2(1.4);
    ast::Value vald(0.4);
    ast::Value val3(std::string("this be a string"));
    ast::Value vals(std::string("this is also a string"));
    ast::Value val4(val1 + val2);
    ast::Value valt(true);
    ast::Value valf(false);
    std::cout << (val1.toDouble()) << ' '
            << (val3.toDouble()) << ' '
            << (valt.toDouble()) << " - "

            << (val1.toString()) << ' '
            << (val3.toString()) << ' '
            << (valt.toString()) << " - "

            << (val1.toBool()) << ' '
            << (val3.toBool()) << ' '
            << (valt.toBool()) << " - "

            << (valt >= valf) << ' '
            << (valt >= valt) << ' '
            << (0) << '\n';
    return 0;


}
