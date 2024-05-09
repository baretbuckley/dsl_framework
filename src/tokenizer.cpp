#include "tokenizer.hpp"

#include <string>

#include <iostream>
#include <memory>

namespace ast {

    // Token::Token(const Token &other)
    //     : type_(other.type()), ref_(0)
    // {
    //     if (type_ == Keyword) {
    //         string = *new std::string();
    //         string = other.string();
    //     }
    //     else ref_.id = other.id();
    // }

    // Token::Token(const Token &&other)
    //     : type_(other.type()), ref_(0)
    // {
    //     if (type_ == Keyword) ref_.string.assign(other.string());
    //     else ref_.id = other.id();
    // }

    // bool TokenMap::loadKeyword(std::string keyword) {
    //     if (keyword.length() == 0) return keyword.length();// fail if empty string
    //     auto &idRef = tokenMap_[keyword];
    //     if (idRef) return false;
    //     idRef = idMap_.size() + 1;
    //     idMap_.push_back(keyword);
    //     return true;
    // }
    // std::string TokenMap::keyword(Token token) {
    //     if (token.type() != Token::Keyword) return token.string();
    //     return idMap_[token.id()];
    // }
    // uint32_t TokenMap::id(const std::string &string) {
    //     auto it = tokenMap_.find(string);
    //     if (it == tokenMap_.end()) return 0;
    //     return it->second;
    // }


    bool Tokenizer::loadKeyword(std::string keyword) {
        for (char c : keyword) if (symbolChars_[(uint8_t)c]) return false;
        return tokenMap_->loadToken(keyword);
    }
    bool Tokenizer::loadSymbol(std::string symbol) {
        if (!tokenMap_->loadToken(symbol)) return false;
        for (char c : symbol) symbolChars_[(uint8_t)c] = true;
        return true;
    }
    bool Tokenizer::loadWhiteSpace(char c) {
        whiteSpaceMap_.insert(c);
        return true;
    }

    void Tokenizer::tokenize(std::vector<Token> &tokens, std::istream &input) {
        
        std::string currentToken;
        char next;

        enum {
            Token,
            WhiteSpace,
            Symbol,
        } state = WhiteSpace;

        while (input.get(next)) {
            std::cout << '\'' << next << "'";
            switch (state) {
                case Token:
                    if (symbolChars_[(uint8_t)next]) {
                        pushToken(currentToken, tokens);
                        currentToken.clear();
                        state = Symbol;
                    } else if (whiteSpaceMap_.contains(next)) {
                        pushToken(currentToken, tokens);
                        state = WhiteSpace;
                    }
                    currentToken.push_back(next);
                    break;
                    
                case WhiteSpace:
                    if (!whiteSpaceMap_.contains(next)) {
                        currentToken = next;
                        if (symbolChars_[(uint8_t)next])
                            state = Symbol;
                        else state = Token;
                    }
                    break;

                case Symbol:
                    if (whiteSpaceMap_.contains(next)) {
                        pushToken(currentToken, tokens);
                        state = WhiteSpace;
                    } else if (!symbolChars_[(uint8_t)next]) {
                        pushToken(currentToken, tokens);
                        currentToken.clear();
                        state = Token;
                    } 
                    currentToken.push_back(next);
                    break;
            }
        }
        if (state != WhiteSpace) {
            pushToken(currentToken, tokens);
            currentToken.push_back(next);
        }
    }

    void Tokenizer::pushToken(const std::string string, std::vector<Token> &tokens) {
        int id;
        id = tokenMap_->tokenID(string);
        if (id) {
            tokens.emplace_back(id);
        } else {
            tokens.emplace_back(string);
        }
    }
    

};