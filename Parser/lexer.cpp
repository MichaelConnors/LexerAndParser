// Michael Connors
// 8001024856
// Assignment 3
#include "lexer.hpp"

    // Returns true if the specified ascii value is found for the different types
bool is_space(int value){return value == 32 || value == 10;}

bool is_digit(int value){return (value >= 48 && value <= 57) || value == 46;}

bool is_letter(int value){return (value >= 65 && value <= 90) || (value >= 97 && value <= 122) || value == 95;}

bool is_operator(int value){
    switch(value){
        case(42):
        case(43): 
        case(45):
        case(92):
        case(61): 
        return true;
    }
    return false;
}
// checks if the passed in string is a keyword
bool is_keyword(std::string word){
    std::string keywords[9] = {"integer", "decimal", "string","print","define","read", "true", "false","bool"};
    
    for(int i = 0; i < sizeof(keywords)/sizeof(keywords[0]);i++){
        if(keywords[i] == word)
            return true;
        
    }
    return false;
}

    // Prints the lexemes out
void Lexer::print_lexemes(){
    std::cout << "Token Type\t" << "Token Value" << std::endl;
    for(int i = 0; i < lexemes.size();i++){
    std::cout << std::left << std::setw(16) << lexemes[i].type_to_string() << lexemes[i].get_lexemes() << std::endl;
    }
}

std::vector<Token> Lexer::getLexemes(){return lexemes;}

   // parses an id and checks if it is a keyword
void Lexer::is_id_or_keyword(){
    std::string lexeme;
    char c;
    do {
        c=file.get();
        lexeme.push_back(c);
    }while(is_letter(file.peek())  || is_digit(file.peek()));
    if(is_keyword(lexeme))
        if(lexeme == "true" || lexeme == "false")
            lexemes.push_back(Token(Token::Type::Bool,lexeme));
        else
            lexemes.push_back(Token(Token::Type::Keyword,lexeme));
    else
        lexemes.push_back(Token(Token::Type::Identifier,lexeme));
}

// parses a string and checks if supports interpolation
void Lexer::is_str(){
    std::string lexeme;
    char c;
    bool interpolation = false;
    char quote;
    quote = file.get();
    if(quote == 39)
        interpolation = true;
    do{
        if (file.peek() == 10){
            lexemes.push_back(Token(Token::Type::Unknown,lexeme));
            lexeme="";
            break;
        }
        if(interpolation && file.peek() == 92){
            file.get();
            if(!lexeme.empty()){
                lexemes.push_back(Token(Token::Type::String,lexeme));
                lexeme = "";
            }
            do{
                c = file.get();
                lexeme.push_back(c);
                
            }while(is_letter(file.peek()) || is_digit(file.peek()));
            lexemes.push_back(Token(Token::Type::Identifier,lexeme));
            lexeme = "";
            }
            else{
                c = file.get();
                lexeme.push_back(c);
            }        
        }while(file.peek() != quote);
        file.get();
        if(!lexeme.empty()){
            lexemes.push_back(Token(Token::Type::String,lexeme));
        }
        
    }

    // parses a number and adds the proper lexeme to the vector of lexemes
void Lexer::is_num(){
    std::string lexeme;
    char c;
    bool is_decimal = false;
    do{
    c = file.get();
    lexeme.push_back(c);
    if(c == 46)
    is_decimal = true;
    }while(is_digit(file.peek()));
    if(is_decimal){
    lexemes.push_back(Token(Token::Type::Decimal,lexeme));
    }else{
        lexemes.push_back(Token(Token::Type::Nondecimal,lexeme));
    }
}

    // parses an operator and if the same operator appears after it it adds them both
void Lexer::is_op(){
    std::string lexeme;
    char c;
    c = file.get();
    lexeme.push_back(c);
    if(c == file.peek()){
        c = file.get();
        lexeme.push_back(c);
    }
    lexemes.push_back(Token(Token::Type::Operator,lexeme));

}


    // adds an eos character to the lexemes vector
void Lexer::is_eos(){
    std::string lexeme;
    lexeme.push_back(char(file.get()));
    lexemes.push_back(Token(Token::Type::EOS,lexeme));
}

    // opens a file and parses it adds lexemes as it parses
void Lexer::parse(){
    std::string comment;
    file.open(filename);
    if(file.is_open()){

    char character;

    while(!file.eof()){
    
    while(is_space(file.peek())){
        file.get();
    }
    character = file.peek();
    if (character == '#')
        getline(file,comment);
    else if(is_letter(character))
        is_id_or_keyword();
    else if(character == 34 || character == 39)
        is_str();
    else if (is_digit(character))
        is_num();
    else if (is_operator(character))
        is_op();
    else if(character == 59)
        is_eos();
    else if(character != 13 && character != -1){
        std::string lexeme;
        lexeme.push_back(char(file.get()));
        lexemes.push_back(Token(Token::Type::Unknown,lexeme));
    }
    else
    file.get();
    }
    file.close();
    }
    else{
        std::cout << "Error: Could not open file." << std::endl;
        exit(0);
    }
}

