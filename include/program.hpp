#include <map>
#include <any>
#include <memory>

namespace dsll {

class Program {
    std::map<std::string, Function> 

    loadFunction(std::string name, Function);

    std::any executeFunction(std::string functionName);

    const std::shared_ptr<Function> getFunction(std::string functionName);

};

}