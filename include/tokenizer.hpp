#pragma once

#include <token.hpp>
#include <finite_automata.hpp>

#include <bitset>
#include <set>
#include <memory>
#include <map>

namespace ast {

class Tokenizer {
    fa::Node<uint32_t> *start_;
    std::map<uint32_t, std::string> tokenNames_;
    std::map<std::string, uint32_t> tokenIDs_;
    #define BufferSize 32
    std::vector<Token> buffer_;
    InputBuffer<> input_;

    public:
    Tokenizer(fa::Node<uint32_t> *start,
                std::map<uint32_t, std::string> &tokenN,
                std::map<std::string, uint32_t> &tokenI)
        : start_(start), tokenNames_(tokenN), tokenIDs_(tokenI),
        buffer_(BufferSize)
    {}

    void loadStream(std::istream *s);

    Token next();

    std::string &tokenName(uint32_t id) {return tokenNames_[id];}

    bool eof() {return input_.eof();}


};

// class Tokenizer {

//     std::shared_ptr<TokenMap> tokenMap_;
//     std::set<char> whiteSpaceMap_;
//     std::bitset<256> symbolChars_;

//     public:

//     Tokenizer(std::shared_ptr<TokenMap> map)
//         : tokenMap_(map),
//         whiteSpaceMap_(), symbolChars_(0)
//     {}
//     ~Tokenizer() {}


//     bool loadKeyword(std::string keyword);
//     bool loadSymbol(std::string symbol);
//     bool loadWhiteSpace(char c);

//     void tokenize(std::vector<Token> &tokens, std::istream &input);

//     std::shared_ptr<TokenMap> getTokenMap() {return tokenMap_;}
//     private:
//     void pushToken(const std::string string, std::vector<Token> &tokens);

// };

}