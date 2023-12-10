#include "TerminalFunction.h"
using namespace std;



class syntax_Analyzer
{
public:
    void checkSyntax()
    {
        TerminalFunction func;
        if (func.start())
        {
            cout << "Successfully Parsed" << endl;
        }
        else
        {
            cout << "Error at Line " << func.lineNumber;
        }
    }
};