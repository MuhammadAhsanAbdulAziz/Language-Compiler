#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <sstream>
using namespace std;

vector<string> KEYWORDS = {"Public", "main", "class", "Main", "void", "var", "VAR", "Var", "true", "false", "if", "if_not", "do_this", "looping_till", "class", "sealed", "abstract", "this"};
vector<string> DT = {"Number", "Boolean", "Word", "Stack"};
vector<string> TERMINATORS = {";", "{", "}", "(", ")", ":", "=>", "[", "]", "~", "."};
vector<string> ACCESS_MODIFIERS = {"public", "private", "protected"};
vector<string> ASSIGN_OPERATORS = {"=", "+=", "-=", "*=", "/="};
vector<string> INC_DEC = {"++", "--"};
vector<string> ARITHMATIC_OPERATORS = {"+", "-", "*", "/", "%"};
vector<string> RELATIONAL_OPERATORS = {"==", "!=", "<=", ">=", "<", ">"};
vector<string> LOGICAL_OPERATORS = {"&&", "||", "!"};

class Token
{
public:
    string CP;
    string VP;
    int line;

public:
    Token(string CP, string VP, int line)
    {
        this->CP = CP;
        this->VP = VP;
        this->line = line;
    }
};

vector<Token> tokens;

class Lexer
{
private:
    fstream inputFile;

    regex Number = regex("^[+-]?[0-9]+(.[0-9]+)?$");
    regex Word = regex("([a-zA-Z]+)");
    regex Identifier = regex("^[a-zA-Z_][a-zA-Z0-9_]*$");

public:
    Lexer(string filePath) : inputFile(filePath)
    {
        if (!inputFile)
        {
            cout << "Error in opening file " << endl;
        }
    }

    void createToken()
    {
        string strLine;
        string str;
        string tempConcatKeyWord;
        int lineCount = 1;
        int eqInt = 0;
        int commentOn = 0;
        while (getline(inputFile, strLine, '\n'))
        {
            stringstream check(strLine);
            while (getline(check, str, ' '))
            {
                if (str == "``")
                {
                    if (commentOn == 0)
                        commentOn = 1;
                    else
                        commentOn = 0;
                    Token t("COMMENT", str, lineCount);
                    tokens.push_back(t);
                    continue;
                }
                else if (str == "`")
                    break;
                if (commentOn == 1)
                    continue;
                if (str == "")
                    continue;
                if (str == " ")
                    continue;
                if (count(DT.begin(), DT.end(), str))
                {
                    Token t("DT", str, lineCount);
                    tokens.push_back(t);
                }
                else if (count(KEYWORDS.begin(), KEYWORDS.end(), str))
                {
                    if (str == "true" || str == "false")
                    {
                        eqInt = 0;
                    }
                    Token t("KEYWORDS", str, lineCount);
                    tokens.push_back(t);
                }
                else if (count(TERMINATORS.begin(), TERMINATORS.end(), str))
                {
                    Token t("TERMINATORS", str, lineCount);
                    tokens.push_back(t);
                }
                else if (count(INC_DEC.begin(), INC_DEC.end(), str))
                {
                    Token t("INC_DEC", str, lineCount);
                    tokens.push_back(t);
                }
                else if (count(ACCESS_MODIFIERS.begin(), ACCESS_MODIFIERS.end(), str))
                {
                    Token t("ACCESS_MODIFIERS", str, lineCount);
                    tokens.push_back(t);
                }
                else if (count(ASSIGN_OPERATORS.begin(), ASSIGN_OPERATORS.end(), str))
                {
                    if (str == "=")
                        eqInt = 1;
                    Token t("ASSIGN_OPERATORS", str, lineCount);
                    tokens.push_back(t);
                }
                else if (count(ARITHMATIC_OPERATORS.begin(), ARITHMATIC_OPERATORS.end(), str))
                {
                    Token t("ARITHMATIC_OPERATORS", str, lineCount);
                    tokens.push_back(t);
                }
                else if (count(RELATIONAL_OPERATORS.begin(), RELATIONAL_OPERATORS.end(), str))
                {
                    Token t("RELATIONAL_OPERATORS", str, lineCount);
                    tokens.push_back(t);
                }
                else if (count(LOGICAL_OPERATORS.begin(), LOGICAL_OPERATORS.end(), str))
                {
                    Token t("LOGICAL_OPERATORS", str, lineCount);
                    tokens.push_back(t);
                }
                else
                {
                    if (str[0] == '"' || str[str.length() - 1] == '"')
                    {
                        if (str[0] == '"' && str[str.length() - 1] == '"')
                        {
                            string str_replace = "";
                            for (char a : str)
                            {
                                if (a == '"')
                                    continue;
                                else
                                    str_replace = str_replace + a;
                            }
                            if (isWord(str_replace))
                            {
                                Token t("Word", str_replace, lineCount);
                                tokens.push_back(t);
                            }
                            else
                            {
                                Token t("Invalid Lexene", str, lineCount);
                                tokens.push_back(t);
                            }
                        }
                        else
                        {
                            Token t("Invalid Lexene", str, lineCount);
                            tokens.push_back(t);
                        }
                        eqInt = 0;
                    }
                    else if (isNumber(str))
                    {
                        Token t("Number", str, lineCount);
                        tokens.push_back(t);
                    }
                    else if(isIdentifier(str))
                    {
                        Token t("IDENTIFIER", str, lineCount);
                        tokens.push_back(t);
                    }
                    else if (isWord(str))
                    {
                        Token t("IDENTIFIER", str, lineCount);
                        tokens.push_back(t);
                    }
                    else
                    {
                        Token t("Invalid Lexene", str, lineCount);
                        tokens.push_back(t);
                    }
                }
            }
            lineCount++;
        }
        Token t("END MARKER", "#", lineCount);
        tokens.push_back(t);
        inputFile.close();
    }
    bool isNumber(string str)
    {
        if (regex_match(str, Number))
        {
            return true;
        }
        else
            return false;
    }
    bool isWord(string str)
    {
        if (regex_match(str, Word))
        {
            return true;
        }
        else
            return false;
    }
    bool isIdentifier(string str)
    {
        if (regex_match(str, Identifier))
        {
            return true;
        }
        else
            return false;
    }
    void getToken()
    {
        ofstream outputFile("Output.txt");
        for (int i = 0; i < tokens.size(); i++)
        {
            outputFile << "(" + tokens[i].CP + ", " + "\"" + tokens[i].VP + "\"" + " ," + to_string(tokens[i].line) + ")" << endl;
        }
    }
};
