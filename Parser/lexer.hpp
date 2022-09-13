#ifndef LEXER_H_INCLUDED
#define LEXER_H_INCLUDED

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <iomanip>

// Token class with an enum for the lexeme type
class Token {
    public:
    enum class Type {
        Keyword,
        Identifier,
        Operator,
        Nondecimal,
        String,
        Comment,
        Decimal,
        EOS,
        Bool,
        Unknown
    };

    Token(){}
    Token(Type type,std::string lexemes):m_type{type},m_lexeme{lexemes}{}

    //Helper function to return the lexeme type as a string

    std::string type_to_string()const{
        switch(m_type){
            case(Type::Keyword): return "Keyword";
            case(Type::Identifier): return "Identifier";
            case(Type::Operator): return "Operator";
            case(Type::Nondecimal): return "integer";
            case(Type::String): return "string";
            case(Type::Comment): return "Comment";
            case(Type::Decimal): return "decimal";
            case(Type::EOS):  return "EOS";
            case(Type::Bool): return "bool";
            case(Type::Unknown): return "Unknown";
       
        }
        return NULL;
    }

    // Helper function to return the lexemes
    std::string get_lexemes()const{
        return m_lexeme;
    }

    Type get_Type()const{
        return m_type;
    }

    private:
        Type m_type{};
        std::string m_lexeme{};
};

class Lexer{
    public:
    Lexer(){}
    Lexer(std::string fname):filename{fname}{};
    
    void parse(); // Function to parse a given filename
    void print_lexemes(); // prints the token type and lexemes
    std::vector<Token> getLexemes();
    private:
    // functions that handle the differrent type of the lexeme
    void is_id_or_keyword();
    void is_num();
    void is_str();
    void is_op();
    void is_eos();
    std::vector<Token> lexemes;
    std::string filename;
    std::ifstream file;
};

#endif