#pragma once

#include <stdint.h>
#include <string>
#include <vector>
#include <map>


namespace ast {


struct Token {

    size_t begining;
    size_t end;
    uint32_t id;

    Token(size_t b, size_t e, uint32_t ID)
        : begining(b), end(e), id(ID) {}
    Token()
        : begining(0), end(0), id(0) {}
};

struct TokenID {
    // uint32_t  id;
    // public:
    // TokenID(Token &token); 
    // TokenID(uint32_t _id) 
    //     : id(_id) {}
};

class TokenMap {
    // class IDMap {
    //     std::map<std::string, uint32_t> idMap_;
    //     public:

    //     IDMap()
    //         : idMap_() {}
    //     ~IDMap() {}

    //     bool loadString(std::string keyword, uint32_t id);
    //     uint32_t id(const std::string &string);
    //     bool contains(uint32_t id);
    // };
    // // IDMap tokenMap_;
    // // IDMap keyMap_;

    // std::vector<std::string> stringMap_;
    // IDMap tokenMap_;
    // IDMap keyMap_;

    // public:

    // TokenMap()
    //     : stringMap_(), tokenMap_(), keyMap_() {}
    // ~TokenMap() {}

    // std::string getKeyword(Token token);
    
    // uint32_t loadToken(std::string keyword);
    // uint32_t tokenID(const std::string &string);

    // uint32_t loadKey(std::string keyword);
    // uint32_t keyID(const std::string &string);
};

} // End of namespace ast