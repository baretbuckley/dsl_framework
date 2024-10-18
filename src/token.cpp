#include <token.hpp>

namespace ast {

// bool TokenMap::IDMap::loadString(std::string keyword, uint32_t id) {
//     if (keyword.length() == 0) return false;// fail if empty string
//     auto &idRef = idMap_[keyword];
//     if (idRef) return false;
//     idRef = id;
//     return true;

//     // if (keyword.length() == 0) return false;// fail if empty string
//     // auto &idRef = idMap_[keyword];
//     // if (idRef) return 0;
//     // idRef = idMap_.size() + 1; // IDs are offset from index by +1
//     // idMap_.push_back(keyword);
//     // return idRef;
// }

// uint32_t TokenMap::IDMap::id(const std::string &string) {
//     auto it = idMap_.find(string);
//     if (it == idMap_.end()) return 0;
//     return it->second;
// }
// bool TokenMap::IDMap::contains(uint32_t id) {
//     // return id <= idMap_.size();
//     return false;
// }

// std::string TokenMap::getKeyword(Token token) {
//     if (token.type() != Token::Keyword) return token.string();
//     return stringMap_[token.id()-1];
// }

// uint32_t  TokenMap::loadToken(std::string keyword) {
//     if (!tokenMap_.loadString(keyword, stringMap_.size()+1)) return 0;
//     stringMap_.push_back(keyword);
//     return stringMap_.size();
// }
// uint32_t TokenMap::tokenID(const std::string &string) {
//     return tokenMap_.id(string);
// }


// uint32_t TokenMap::loadKey(std::string keyword) {
//     if (!keyMap_.loadString(keyword, stringMap_.size()+1)) return 0;
//     stringMap_.push_back(keyword);
//     return stringMap_.size();
// }
// uint32_t TokenMap::keyID(const std::string &string) {
//     return keyMap_.id(string);
// }


// TokenID::TokenID(Token &token)
//     : id(token.id())
// { 
//     if (token.type() == Token::Identifier)
//         id = 0;
// }

}