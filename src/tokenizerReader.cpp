
#include <tokenizerReader.hpp>

#include <sstream>

namespace ast {

// trim from start (in place)
inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

inline std::string &trim(std::string &&s) {
    ltrim(s);
    rtrim(s);
    return s;
}

inline std::string &trim(std::string &s) {
    ltrim(s);
    rtrim(s);
    return s;
}

inline int ltrimCount(std::string &s) {
    int i = 0;
    while (s[i] == ' ') i++;
    s.erase(s.begin(), s.begin()+i);
    return i;
}

void TokenizerReader::readToken(std::istream &fileStream, std::string &line) {
    Token t;
    t.name = trim(line.substr(5, line.find(':')-5));
    std::cout << "Read name: " << t.name << std::endl;
    int indent = 0;
    if (fileStream) {
        do {std::getline(fileStream, line);}
        while (fileStream && line == "");
        indent = ltrimCount(line);
        std::string cause = trim(line.substr(0, line.find('>')));
        if (cause.size() >= 5 && !cause.compare(0, 5, "Enter"))
            entranceRules_[t.name] = trim(cause.substr(5, cause.size()));
        else if (cause == "other") t.otherTransition = trim(line.substr(line.find('>')+1, line.size()));
        else t.transitions.push_back({cause,
                trim(line.substr(line.find('>')+1, line.size()))});
        std::cout << "First transition: " << trim(line.substr(0, line.find('>'))) << " - " << trim(line.substr(line.find('>')+1, line.size())) << '\n';
    }
    int lastIndent = indent;
    while (fileStream && lastIndent == indent) {
        std::getline(fileStream, line);
        lastIndent = ltrimCount(line);
        if (line == "") {
            lastIndent = indent; 
            continue;
        }
        if (lastIndent != indent) break;
        std::string cause = trim(line.substr(0, line.find('>')));
        if (cause == "other") t.otherTransition = trim(line.substr(line.find('>')+1, line.size()));
        else t.transitions.push_back({cause,
                trim(line.substr(line.find('>')+1, line.size()))});
        std::cout << "other transition: " << trim(line.substr(0, line.find('>'))) << " - " << trim(line.substr(line.find('>')+1, line.size())) << '\n';
    }
    tokens_.push_back(t);
}

void TokenizerReader::readStart(std::istream &fileStream, std::string &line) {
    uint32_t indent = 0;
    uint32_t lastIndent = 0;
    if (fileStream) {
        do {std::getline(fileStream, line);}
        while (fileStream && line == "");
        std::cout << "Working start: " << line << '\n';
        indent = ltrimCount(line);
        std::cout << indent << '\n';
        if (indent == 0) return;
        start_.push_back(trim(line));
        std::cout << "End of line\n";
    }
    lastIndent = indent;
    while (fileStream && lastIndent == indent) {
        std::getline(fileStream, line);
        std::cout << "Working start: " << line << '\n';
        lastIndent = ltrimCount(line);
        if (line == "") {
            lastIndent = indent; 
            continue;
        }
        if (lastIndent == 0) return;
        start_.push_back(trim(line));
        if (lastIndent != indent) {
            std::cerr << "WARNING: unexpected indentation level in Start element\n";
        }
    }
}

void TokenizerReader::read(std::string tokenizerPath) {
    std::ifstream file(tokenizerPath);
    if (!file.is_open()) {
        std::cerr << "Error opening the file!";
        return;
    }
    read(file);
}


void TokenizerReader::read(std::istream &fileStream){
    std::string line;
    std::getline(fileStream, line);
    while (fileStream) {
            
        if (line == "") {
            std::cout << "Empty line\n";
            std::getline(fileStream, line);
            
        } else if (line.size() >= 5 && !line.compare(0, 5, "Token")) {
            std::cout << "reading token: " << line << std::endl;
            readToken(fileStream, line);
        } else if (line.size() >= 5 && !line.compare(0, 5, "Start")) {
            std::cout << "Reading start\n";
            readStart(fileStream, line);
        } else {
            std::cerr << "Warning: Unrecognized Command\n";
            std::cerr << '"' << line << "\"\n";
            std::getline(fileStream, line);
        }
    }
    for (auto t : tokens_)
        initToken(t);
    for (auto t : tokens_)
        finalizeToken(t);
    finalizeStart();

    
}

void TokenizerReader::initToken(Token &t) {
    std::cout << "Start of init token " << t.name << '\n';
    if (entrances_[t.name] != nullptr) {
        std::cerr << "Error: multiple Elements with name \"" << t.name << "\"\n";
        return;
    }
    FAState *node = nullptr;
    if (t.otherTransition == "") {
        node = new FAState(0);
    } else {
        if (t.otherTransition.size() >= 9 &&
                !t.otherTransition.compare(0, 9, "Terminate")) {
            std::string target = trim(t.otherTransition.substr(9, t.otherTransition.size()));
            std::cout << "name: " << target << '\n';
            if (target == "") target = t.name;

            uint32_t targetID = tokenIDs_[target];
            if (targetID == 0) tokenIDs_[target] = targetID = idCount_++;
            tokenNames_[targetID] = t.name;
            node = new FAState(targetID);
            std::cout << "Adding other termination as " << target << " (" << targetID << ") to state " << t.name << '\n';
        }
        else if (!t.otherTransition.compare("Reject")) {
            node = new FAState(0);
            std::cout << "Adding other termination as rejection to state " << t.name << '\n';
        } else {
            std::cerr << "Error: Unknown Transition: other > " << t.otherTransition << '\n';
        }
        
    }
    entrances_[t.name] = node;
}

void TokenizerReader::finalizeToken(Token &t) {
    FAState *entrance = entrances_[t.name];
    if (entrance == nullptr) {
        std::cerr << "ERROR: Token initialized without entrance state.\n";
        return;
    }
    for (auto r : t.transitions) {
        if (entrances_[r.second] == nullptr) {
            std::cout << "ERROR: in Element '" << t.name << "' Attempt to transition to unknown Element '" << r.second << "'.\n";
            continue;
        }
        entrance->addTransition(r.first[0], entrances_[r.second]);
    }
}
void TokenizerReader::finalizeStart() {
    FAState *start = entrances_["Start"] = new FAState(0);
    FAState *dest = nullptr;

    for (auto s : start_) {
        dest = entrances_[s];
        if (dest == nullptr) {
            std::cerr << "ERROR: starting element '" << s << "' has no corisponding entrance.\n";
            continue;
        }
        std::cout << "Adding start transition: " << entranceRules_[s][0] << " > " << s << '\n';
        start->addTransition(entranceRules_[s][0], dest);
    }
}

Tokenizer *TokenizerReader::createTokenizer() {
    return new Tokenizer(entrances_["Start"], tokenNames_, tokenIDs_);
}

}