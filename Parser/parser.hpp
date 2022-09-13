#include "lexer.hpp"
#include <map>

class Error{
    public: 
    enum class ErrorType{
        Undeclared,
        Declared,
        InvalidType,
        InvalidAssignment,
        InvalidOperator
    };

    std::string type_to_string()const{
        switch(m_type){
            case(ErrorType::Undeclared):return "Identifier is not declared.";
            case(ErrorType::Declared):return "Identifier has already been declared.";
            case(ErrorType::InvalidType):return "Types do not match.";
            case(ErrorType::InvalidAssignment):return "Cannot assign value to type";
            case(ErrorType::InvalidOperator):return "Type does not support operator";
            default: return "Unknown error type.";
        }
    }

    private:
    ErrorType m_type{};
};

struct variable{
    std::string name;
    std::string type;
};

class Parser{
    public:
    Parser(){}
    Parser(std::vector<Token> tokens):m_tokens{tokens}{};

   
    int lookup(std::string);
    void addVariable(variable);
    void declaration(unsigned);
    unsigned assignment(unsigned);
    void io(unsigned);
    void logical(unsigned);
    unsigned arithmetic(unsigned,std::string,bool = false,struct variable = {});
    void parse();
    
    private:
    std::map<std::string,variable> symbolTable;
    std::vector<Token> m_tokens;
};
