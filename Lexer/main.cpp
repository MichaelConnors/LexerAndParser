#include "lexer.hpp"

using namespace std;

// handles usage error, creates the lexer, parses the file outputs the result
int main(int argc,char* argv[]){
    if(argc != 2){
        cout << "Usage: ./lexer [filename]" << endl;
        return 0;
    }
    Lexer lexer = Lexer(argv[1]);
    lexer.parse();
    lexer.print_lexemes();
}