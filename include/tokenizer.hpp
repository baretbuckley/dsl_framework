#pragma once

#include <token.hpp>

#include <bitset>
#include <set>
#include <memory>

namespace ast {

class Tokenizer {

    std::shared_ptr<TokenMap> tokenMap_;
    std::set<char> whiteSpaceMap_;
    std::bitset<256> symbolChars_;

    public:

    Tokenizer(std::shared_ptr<TokenMap> map)
        : tokenMap_(map),
        whiteSpaceMap_(), symbolChars_(0)
    {}
    ~Tokenizer() {}


    bool loadKeyword(std::string keyword);
    bool loadSymbol(std::string symbol);
    bool loadWhiteSpace(char c);

    void tokenize(std::vector<Token> &tokens, std::istream &input);

    std::shared_ptr<TokenMap> getTokenMap() {return tokenMap_;}
    private:
    void pushToken(const std::string string, std::vector<Token> &tokens);

};

}