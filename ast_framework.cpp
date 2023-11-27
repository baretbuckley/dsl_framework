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

    std::cout << ast::IndexOfType<ast::Value, std::string, bool, int, float>::value << std::endl;

    // ast::Value valN("");
    ast::Value val0(0.0);
    ast::Value val1(0.4);
    ast::Value val2(1.4);
    ast::Value vald(0.4);
    ast::Value val3(std::string("this be a string"));
    ast::Value vals(std::string("this is also a string"));
    ast::Value val4(val1 + val2);
    ast::Value valt(true);
    ast::Value valf(false);
    std::cout 
            << (-val2) << ' '
            << val2 << ' '
            << (val2 || vald) << ' '
            << (val0 || vald) << ' '
            << (val0 || valf) << ' '
            << !val2 << ' '
            // << (val3.type()) << ' '
    //         // << (valt.type()) << " - "

    //         // << (val1.toString()) << ' '
    //         // << (val3.toString()) << ' '
    //         // << (valt.toString()) << " - "

    //         // << (val1.toBool()) << ' '
    //         // << (val3.toBool()) << ' '
    //         // << (valt.toBool()) << " - "

    //         // << (valt >= valf) << ' '
    //         // << (valt >= valt) << ' '
            << (1) << '\n';
    // return 0;


}
