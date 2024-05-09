#pragma once

#include <variant>
#include <stdint.h>
#include <string>
#include <vector>
#include <map>

namespace ast {

class Token {
    public:
    enum Type {
        Keyword,
        Identifier
    };
    private:

    Type type_;
    std::variant<uint32_t, std::string> ref_;

    public:

    Token(const uint32_t id)
        : type_(Keyword), ref_(id) {}
    Token(const std::string string)
        : type_(Identifier), ref_(string) {}
    ~Token() {}

    Type type() const {return type_;}
    const uint32_t &id() const {return std::get<uint32_t>(ref_);}
    const std::string &string() const {return std::get<std::string>(ref_);}
};

struct TokenID {
    uint32_t  id;
    public:
    TokenID(Token &token); 
    TokenID(uint32_t _id) 
        : id(_id) {}
};

class TokenMap {
    class IDMap {
        std::map<std::string, uint32_t> idMap_;
        public:

        IDMap()
            : idMap_() {}
        ~IDMap() {}

        bool loadString(std::string keyword, uint32_t id);
        uint32_t id(const std::string &string);
        bool contains(uint32_t id);
    };
    // IDMap tokenMap_;
    // IDMap keyMap_;

    std::vector<std::string> stringMap_;
    IDMap tokenMap_;
    IDMap keyMap_;

    public:

    TokenMap()
        : stringMap_(), tokenMap_(), keyMap_() {}
    ~TokenMap() {}

    std::string getKeyword(Token token);
    
    uint32_t loadToken(std::string keyword);
    uint32_t tokenID(const std::string &string);

    uint32_t loadKey(std::string keyword);
    uint32_t keyID(const std::string &string);
};

} // End of namespace ast