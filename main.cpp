#include "SyntaxAnalyzer.h"
using namespace std;



int main()
{
    Lexer lexer("input.txt");
    lexer.createToken();
    lexer.getToken();
    syntax_Analyzer syntax;
    syntax.checkSyntax();
}