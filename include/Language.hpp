#include <map>
#include <string>

namespace dsll {

class Language {
    std::Vector<LanguageExtension> extensions_;
    Parser parser;

    Language();
    ~Language();

    void addExtension(LanguageExtension extension);

    void loadLanguage(std::string languagePath);

    Program parse(std::string programPath);


    

};

}