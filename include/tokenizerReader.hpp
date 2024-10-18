
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm> 
#include <cctype>
#include <locale>
#include <cinttypes>

#include "finite_automata.hpp"
#include "tokenizer.hpp"

namespace ast {

    class TokenizerReader {
        struct Token {
            std::string name;
            std::vector<std::pair<std::string, std::string>> transitions;
            std::string otherTransition;
        };

        typedef fa::Node<uint32_t> FAState;

        std::vector<Token> tokens_;
        std::map<std::string, FAState*>  entrances_;
        std::map<std::string, std::string> entranceRules_;
        std::map<std::string, uint32_t> tokenIDs_;
        std::map<uint32_t, std::string> tokenNames_;
        uint32_t idCount_;
        std::vector<std::string> start_;
        

        public:

        TokenizerReader() 
            : idCount_(1) {}
        ~TokenizerReader() {}

        private:
        

        void readToken(std::istream &fileStream, std::string &line);

        void readStart(std::istream &fileStream, std::string &line);

        public:
        void read(std::string tokenizerPath);

        void read(std::istream &fileStream);


        void initToken(Token &t);

        void finalizeToken(Token &t);

        void finalizeStart();

        Tokenizer *createTokenizer();

    };

}