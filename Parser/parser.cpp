#include "parser.hpp"
// Helper function to find an entry in the symbolTable

int Parser::lookup(std::string name){
    if(symbolTable.find(name) == symbolTable.end())
        return 0;
    else
    return 1;
}

// Helper Function to add a variable to the symbolTable

void Parser::addVariable(variable var){
    symbolTable[var.name] = var;
}

// Parses a declaration for Errors. If the variable exists,
// prints out an error message, if not adds it to the symbolTable

void Parser::declaration(unsigned index){
    if (lookup(m_tokens[index+1].get_lexemes()) == 1){
        std::cout << "Error: Variable " << m_tokens[index+1].get_lexemes() << " already declared" << std::endl;
    }
    else{
        struct variable var;
        var.name = m_tokens[index+1].get_lexemes();
        if(m_tokens[index].get_lexemes() == "string")
            var.type = "string";
        else if(m_tokens[index].get_lexemes() == "integer")
            var.type = "integer";
        else if(m_tokens[index].get_lexemes() == "bool")
            var.type = "bool";
        else if(m_tokens[index].get_lexemes() == "decimal")
            var.type = "decimal";
        else
            std::cout << "Something went wrong" << std::endl;
        addVariable(var);
        }
        
    }

//Parses an assignment statement checking for errors

unsigned Parser::assignment(unsigned index){
    std::string expectedType;
    std::string actualType;
    bool declaration = false;
    struct variable var;
    // If the current token is a datatype it checks if a variable has been declared with the same name
    if(m_tokens[index].get_Type() == Token::Type::Keyword && (m_tokens[index].get_lexemes() == "string" 
        || m_tokens[index].get_lexemes() == "integer" || m_tokens[index].get_lexemes() == "bool" || m_tokens[index].get_lexemes() == "decimal")){
        if(lookup(m_tokens[index+1].get_lexemes())==1){
            std::cout << "Error: Variable " << m_tokens[index+1].get_lexemes() << " has already been declared." << std::endl;
            expectedType = "undeclared";
        }else{
        var.name = m_tokens[index+1].get_lexemes();
        expectedType = m_tokens[index].get_lexemes();
        declaration = true;
        index++;
        }
        // If the first token is an Identifier then it checks if it exists already in the symbol table
    }else{
        if(lookup(m_tokens[index].get_lexemes())==0){
            std::cout << "Error: Variable " << m_tokens[index].get_lexemes() << " must be declared for use in assignment statement." << std::endl;
            declaration = false;
        }else{
        expectedType = symbolTable[m_tokens[index].get_lexemes()].type;
        }
    }
    index+=2;
    // Checks what the token after the = is and handles accordingly
    if(m_tokens.size() > index+1){
                if(m_tokens[index+1].get_lexemes() == "=="){
                    logical(index);
                    actualType = "bool";
                    index+=2;
                    // If the statement contains an operator then it passes functionality to the arithmetic function
                }else if(m_tokens[index+1].get_Type() == Token::Type::Operator){
                    index = arithmetic(index,expectedType,declaration,var);
                    return index;
                    }
                }
                if (m_tokens[index].get_Type() == Token::Type::Identifier){
                    if(lookup(m_tokens[index].get_lexemes()) == 0){
                        std::cout << "Error: Variable " << m_tokens[index].get_lexemes() << " must be declared for use in assignment statement." << std::endl;
                        actualType = "undeclared";
                    }else{
                        actualType = symbolTable[m_tokens[index].get_lexemes()].type;
                    }
                }else if(m_tokens[index].get_Type() == Token::Type::Bool || m_tokens[index].get_Type() == Token::Type::Nondecimal
         || m_tokens[index].get_Type() == Token::Type::String || m_tokens[index].get_Type() == Token::Type::Decimal) {
                 actualType = m_tokens[index].type_to_string();
            
        }else if(m_tokens[index].get_Type() == Token::Type::Keyword){
            std::cout << "Error: Cannot assign keyword to variable." << std::endl;
            actualType = "undeclared";
            index++;
        }

    // If the expected type is equal to the actual type then it adds it to the symbol table if not it outputs an error
    if(expectedType != actualType && (actualType != "undeclared")){
        std::cout << "Error: expected a " << expectedType << " but got " << actualType << " in assignment statement." << std::endl;
        declaration = false;
    }else{
        if(declaration && (expectedType == actualType)){
            var.type = expectedType;
            addVariable(var);
        }
    }
    return index;
}

// Handles arithmetic statements
unsigned Parser::arithmetic(unsigned index,std::string expectedType,bool declaration,struct variable var){
    std::string actualType;
    std::string currentType;
    // while the size of the size of the tokens vector is able to support arithmetic it will run
    do{
        // Handles Tokens based on type
        switch(m_tokens[index].get_Type()){
            case(Token::Type::Operator):
                    // Excludes certain datatypes from using different operators
                    if(expectedType == "bool"){
                        if (m_tokens[index].get_lexemes() != "+" && m_tokens[index].get_lexemes() != "*"){
                            std::cout << "Error: operator " << m_tokens[index].get_lexemes() << " not allowed with type " << expectedType << std::endl;
                            declaration = false;
                        }
                    }
                    else if(expectedType == "string"){
                        if(m_tokens[index].get_lexemes() != "+"){
                            std::cout << "Error: operator " << m_tokens[index].get_lexemes() << " not allowed with type " << expectedType << std::endl;
                            declaration = false;
                        }
                    }
                break;
            case(Token::Type::Keyword):
                std::cout<< "Error: Cannot use keyword in arithmetic expression" << std::endl;
                break;
            // If it's an Identifier it makes sure it has been declared before continuing 
            case(Token::Type::Identifier):
                if(lookup(m_tokens[index].get_lexemes()) == 0){
                    std::cout << "Error: Variable " << m_tokens[index].get_lexemes() << " must be declared for use in arithmetic statement." << std::endl;
                    actualType = "undeclared";
                    declaration = false;
                }else{
            // Cases for the different types of values
            case(Token::Type::Nondecimal):
            case(Token::Type::String):
            case(Token::Type::Decimal):
            case(Token::Type::Bool):
                    if(actualType == ""){
                        // If its an identifier it looks up its typ from the symbol table otherwise it checks the type of the value
                        if(m_tokens[index].get_Type() == Token::Type::Identifier)
                            actualType = symbolTable[m_tokens[index].get_lexemes()].type;
                        else
                            actualType = m_tokens[index].type_to_string();
                        currentType = actualType;
                    }
                    else{
                        if(m_tokens[index].get_Type() == Token::Type::Identifier){
                            currentType = symbolTable[m_tokens[index].get_lexemes()].type;
                        }
                        else
                            currentType = m_tokens[index].type_to_string();
                }
                
                }
                if(currentType == "decimal" && actualType == "integer")
                        actualType = "decimal";
                if(actualType == "decimal" && currentType == "integer") 
                        currentType = "decimal";
                
            if(expectedType != currentType && actualType != "undeclared"){
                std::cout << "Error: expected a " << expectedType << " but " << m_tokens[index].get_lexemes() << " is of type " << currentType << " in arithmetic statement." << std::endl;
                declaration = false;
            }
            // If the current token a value or Identifier and the next token is not an operator then it checks if a new vriable is being declared,
            // and adds it to the symbol table, if not it just returns
            if((m_tokens[index].get_Type() == Token::Type::Identifier || m_tokens[index].get_Type() == Token::Type::Nondecimal || m_tokens[index].get_Type() == Token::Type::Decimal ||
            m_tokens[index].get_Type() == Token::Type::String || m_tokens[index].get_Type() == Token::Type::Bool) && m_tokens[index+1].get_Type() != Token::Type::Operator){
                if(declaration && actualType == expectedType){
                    var.type = expectedType;
                    addVariable(var);
                }
                return index;
                }
                default:
                    break;
        }
              index++; 
            }while(m_tokens.size() >= index+1);
    return index;
}

// Function to handle logical statements makes sure any variable being used is previously declared
void Parser::logical(unsigned index){
    switch(m_tokens[index].get_Type()){
        case(Token::Type::Identifier):
            if(lookup(m_tokens[index].get_lexemes())==0)
                std::cout << "Error: Variable " << m_tokens[index+3].get_lexemes() << " must be declared for use in a logical statement." << std::endl; 
        case(Token::Type::Bool):
        case(Token::Type::Decimal):
        case(Token::Type::Nondecimal):
        case(Token::Type::String):
            break;
        default:
            std::cout << "Cannot use " << m_tokens[index].type_to_string() << " in logical statements" << std::endl; 
    }
    switch(m_tokens[index+2].get_Type()){
        case(Token::Type::Identifier):
        if(lookup(m_tokens[index].get_lexemes()) == 0)
            std::cout << "Error: Variable " << m_tokens[index+3].get_lexemes() << " must be declared for use in a logical statement." << std::endl; 
        case(Token::Type::Bool):
        case(Token::Type::Decimal):
        case(Token::Type::Nondecimal):
        case(Token::Type::String):
            break;
        default:
            std::cout << "Cannot use " << m_tokens[index+2].type_to_string() << " in logical statements" << std::endl; 
    }
}

// Handles both the read and print statments also ensures that a variable has been declared before being used,
// and ensures only certain types can be used
void Parser::io(unsigned index){
    if(m_tokens[index].get_lexemes() == "read"){
        index++;
        if(m_tokens[index].get_Type() == Token::Type::Identifier){
            if(lookup(m_tokens[index].get_lexemes()) == 0)
                std::cout << "Error: Variable " << m_tokens[index].get_lexemes() << " must be declared for use in read statements." << std::endl; 
            if(m_tokens[index].get_Type() == Token::Type::Bool)
                std::cout << "Cannot read boolean values" << std::endl;
        } else{
            std::cout << "Error: Cannot use " << m_tokens[index].type_to_string() << " in read statement." << std::endl;
        }
    }else if(m_tokens[index].get_lexemes() == "print"){
        index++;
        switch(m_tokens[index].get_Type()){
            case(Token::Type::Identifier):
            if(lookup(m_tokens[index].get_lexemes()) == 0)
                std::cout << "Error: Variable " << m_tokens[index].get_lexemes() << " must be declared for use in print statements." << std::endl; 
            case(Token::Type::Decimal):
            case(Token::Type::Nondecimal):
            case(Token::Type::String):
            case(Token::Type::Bool):
            break;
            default:
            std::cout << "Error: Cannot use " << m_tokens[index].type_to_string() << " in print statement." << std::endl;

        }
    }
}
// Parses the token of vectors and calls helper functions based on the tokens
void Parser::parse(){
    std::vector<Token>::size_type size = m_tokens.size();
    for(unsigned i=0;i<size;i++){
        switch(m_tokens[i].get_Type()){
            case(Token::Type::Keyword):
                if(m_tokens[i].get_lexemes() == "integer" || m_tokens[i].get_lexemes() == "string" || m_tokens[i].get_lexemes() == "decimal" || m_tokens[i].get_lexemes() == "bool"){
                    if(size-1 > i+2){
                    if(m_tokens[i+2].get_lexemes() == "="){
                        i = assignment(i);
                        break;
                    } else{
                        declaration(i);
                        i++;
                        break;
                        }
                }else{
                    declaration(i);
                    i++;
                    break;
                }
                }
                else if (m_tokens[i].get_lexemes() == "read" || m_tokens[i].get_lexemes() == "print"){
                  io(i);
                  i++;
                  break;
                }
            case(Token::Type::Identifier):
                i = assignment(i);
                break;
            default:
                break;
            }
        }
    }

