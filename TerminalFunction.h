#include "Lexer.h"
using namespace std;

class TerminalFunction
{
private:
    int index = 0;
    int flag = 0;

public:
    int lineNumber = 0;
    bool var_st()
    {
        if (tokens[index].VP == "Var" || tokens[index].VP == "VAR" || tokens[index].VP == "var")
        {
            index++;
            return true;
        }
        return false;
    }

    bool DT()
    {
        if (tokens[index].CP == "DT")
        {
            index++;
            return true;
        }
        return false;
    }

    bool IDENTIFIER()
    {
        if (tokens[index].CP == "IDENTIFIER")
        {
            index++;
            return true;
        }
        return false;
    }

    bool OE()
    {
        if (DT())
        {
            return true;
        }
        if (tokens[index].CP == "INC_DEC" || tokens[index].CP == "Number" || tokens[index].CP == "Word" || tokens[index].VP == "this" || tokens[index].VP == "true" || tokens[index].VP == "false" || tokens[index].CP == "ARITHMATIC_OPERATORS" || tokens[index].CP == "LOGICAL_OPERATORS" || tokens[index].CP == "ASSIGN_OPERATORS" || tokens[index].CP == "IDENTIFIER" || tokens[index].VP == "(" || tokens[index].VP == ")" || tokens[index].VP == "[" || tokens[index].VP == "]")
        {
            index++;
            return true;
        }
        return false;
    }

    bool assign_op()
    {
        if (tokens[index].VP == "=")
        {
            index++;
            if (OE())
            {
                return true;
            }
        }
        return false;
    }

    bool init1()
    {
        while (tokens[index].VP != ";")
        {
            if (OE())
            {
                continue;
            }
            else
                break;
        }
        if (tokens[index].VP == ";")
        {
            return true;
        }
        return false;
    }

    bool dimen()
    {
        if (tokens[index].VP == "[")
        {
            index++;
            if (tokens[index].CP == "Number")
            {
                index++;
                if (tokens[index].VP == "]")
                {
                    index++;
                    return true;
                }
            }
            else if (tokens[index].VP == "]")
            {
                index++;
                return true;
            }
        }
        return false;
    }

    bool array_init2()
    {
        if (tokens[index].VP == ",")
        {
            index++;
            return true;
        }
        return false;
    }

    bool array_init()
    {
        while (tokens[index].VP != ";")
        {
            if (tokens[index].CP == "Number" || tokens[index].CP == "Word")
            {
                index++;
                if (array_init2())
                {
                    continue;
                }
                else if (tokens[index].VP == "}")
                {
                    index++;
                    if (tokens[index].VP == ";")
                    {
                        break;
                    }
                    else if (tokens[index].VP == "{")
                    {
                        index++;
                        continue;
                    }
                }
            }
        }
        if (tokens[index].VP == ";")
        {
            index++;
            return true;
        }
        return false;
    }

    bool array_decl()
    {
        if (tokens[index].VP == "[")
        {
            while (tokens[index].VP != "{" || tokens[index].VP != ";")
            {
                if (dimen())
                {
                    continue;
                }
                else
                    break;
            }
            if (tokens[index].VP == "{")
            {
                index++;
                if (array_init())
                {
                    return true;
                }
            }
            else if (tokens[index].VP == ";")
            {
                return true;
            }
        }
        return false;
    }

    bool list()
    {
        if (tokens[index].VP == ":" || tokens[index].VP == ",")
        {
            if (tokens[index].VP == ":")
            {
                index++;
                if (DT())
                {
                    if (tokens[index].VP == ";")
                    {
                        return true;
                    }
                    else if (tokens[index].VP == "=")
                    {
                        index++;
                        if (init1())
                        {
                            return true;
                        }
                    }
                    else if (tokens[index].VP == "[")
                    {
                        if (array_decl())
                        {
                            return true;
                        }
                    }
                }
            }
            else if (tokens[index].VP == ",")
            {
                index++;
                if (IDENTIFIER())
                {
                    if (list())
                    {
                        return true;
                    }
                    else if (assign_op())
                    {
                        if (list())
                        {
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }

    bool decl()
    {
        if (var_st())
        {
            if (IDENTIFIER())
            {
                if (list())
                {
                    return true;
                }
            }
        }
        return false;
    }

    bool init()
    {
        if (tokens[index].CP == "IDENTIFIER")
        {
            index++;
            if (tokens[index].CP == "ASSIGN_OPERATORS")
            {
                index++;
                if (init1())
                {
                    return true;
                }
            }
            else if (tokens[index].CP == "INC_DEC")
            {
                index++;
                return true;
            }
        }

        return false;
    }

    bool i_c()
    {
        if (tokens[index].CP == "IDENTIFIER" || tokens[index].CP == "Word" || tokens[index].CP == "Number" || tokens[index].VP == "true" || tokens[index].VP == "false" || tokens[index].CP == "RELATIONAL_OPERATORS" || tokens[index].VP == ")" || tokens[index].CP == "LOGICAL_OPERATORS" || tokens[index].CP == "ASSIGN_OPERATORS" || tokens[index].CP == "ARITHMATIC_OPERATORS")
        {
            index++;
            return true;
        }
        return false;
    }

    bool cond()
    {
        if (i_c())
        {
            while (tokens[index].VP != ")")
            {
                if (i_c())
                {
                    continue;
                }
                else
                {
                    break;
                }
            }
            if (tokens[index].VP == ")")
            {
                index++;
                return true;
            }
        }
        return false;
    }

    bool looping_till()
    {
        if (tokens[index].VP == "looping_till")
        {
            index++;
            if (tokens[index].VP == "(")
            {
                index++;
                if (cond())
                {
                    if (tokens[index].VP == "{")
                    {
                        index++;
                        while (tokens[index].VP != "}")
                        {
                            if (body())
                            {
                                continue;
                            }
                            else
                            {
                                break;
                            }
                        }
                        if (tokens[index].VP == "}")
                        {
                            index++;
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }

    bool body()
    {
        if (looping_till() || if_st() || decl() || init())
        {
            return true;
        }
        else if (tokens[index].VP == ";" || tokens[index].CP == "Word" || tokens[index].CP == "Number" || tokens[index].CP == "Boolean" || tokens[index].CP == "IDENTIFIER" || tokens[index].VP == "true" || tokens[index].VP == "false" || tokens[index].CP == "DT" || tokens[index].VP == "}")
        {
            index++;
            return true;
        }
        return false;
    }

    bool do_this()
    {
        if (tokens[index].VP == "do_this")
        {
            index++;
            if (tokens[index].VP == "{")
            {
                index++;
                while (tokens[index].VP != "}")
                {
                    if (body())
                    {
                        continue;
                    }
                    else
                    {
                        break;
                    }
                }
                if (tokens[index].VP == "}")
                {
                    index++;
                    return true;
                }
            }
        }
        return false;
    }

    bool if_not_st()
    {
        if (tokens[index].VP == "if_not")
        {
            index++;
            if (tokens[index].VP == "(")
            {
                index++;
                if (cond())
                {
                    if (tokens[index].VP == "{")
                    {
                        index++;
                        while (tokens[index].VP != "}")
                        {
                            if (body())
                            {
                                continue;
                            }
                            else
                            {
                                break;
                            }
                        }
                        if (tokens[index].VP == "}")
                        {
                            index++;
                            if (if_not_st() || do_this() || body())
                            {
                                return true;
                            }
                        }
                    }
                }
            }
        }
        return false;
    }

    bool if_st()
    {
        if (tokens[index].VP == "if")
        {
            index++;
            if (tokens[index].VP == "(")
            {
                index++;
                if (cond())
                {
                    if (tokens[index].VP == "{")
                    {
                        index++;
                        while (tokens[index].VP != "}")
                        {
                            {
                                if (body())
                                {
                                    continue;
                                }
                                else
                                {
                                    break;
                                }
                            }
                        }
                        if (tokens[index].VP == "}")
                        {
                            index++;
                            if (if_not_st() || if_st() || do_this() || body())
                            {
                                return true;
                            }
                        }
                    }
                }
            }
        }
        return false;
    }

    bool class_def()
    {
        return false;
    }

    bool newline()
    {
        while (tokens[index].VP != ")")
        {
            if (tokens[index].VP == ",")
            {
                index++;
                if (var_st())
                {
                    if (IDENTIFIER())
                    {
                        if (tokens[index].VP == ":")
                        {
                            index++;
                            if (tokens[index].CP == "DT")
                            {
                                index++;
                                continue;
                            }
                        }
                    }
                }
            }
        }
        if (tokens[index].VP == ")")
        {
            index++;
            return true;
        }
        return false;
    }

    bool sp_decl()
    {
        if (var_st())
        {
            if (IDENTIFIER())
            {
                if (tokens[index].VP == ":")
                {
                    index++;
                    if (tokens[index].CP == "DT")
                    {
                        index++;
                        if (newline())
                        {
                            return true;
                        }
                    }
                }
            }
        }
        else if (tokens[index].VP == ")")
        {
            index++;
            return true;
        }
        return false;
    }

    bool func_st()
    {
        if (tokens[index].CP == "DT" || tokens[index].VP == "void")
        {
            index++;
            if (tokens[index].CP == "IDENTIFIER")
            {
                index++;
                if (tokens[index].VP == "=>")
                {
                    index++;
                    if (tokens[index].VP == "(")
                    {
                        index++;
                        if (sp_decl())
                        {
                            if (tokens[index].VP == "{")
                            {
                                index++;
                                while (tokens[index].VP != "}")
                                {
                                    if (body())
                                    {
                                        continue;
                                    }
                                    else
                                    {
                                        break;
                                    }
                                }
                                if (tokens[index].VP == "}")
                                {
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }
        return false;
    }

    bool other()
    {
        if (tokens[index].VP == "}")
        {
            index++;
            if (class_def())
            {
                other();
            }
            else if (tokens[index].VP == "}")
            {
                index++;
                if (tokens[index].VP == "#")
                {
                    return true;
                }
            }
            else if (func_st())
            {
                if(other()){
                    return true;
                }
            }
        }
        return false;
    }

    bool main_fn()
    {
        // if (tokens[index].VP == "void")
        // {
        //     flag = 1;
        //     index++;
        if (tokens[index].VP == "main" || tokens[index].VP == "Main")
        {
            index++;
            if (tokens[index].VP == "=>")
            {
                index++;
                if (tokens[index].VP == "(")
                {
                    index++;
                    if (tokens[index].VP == ")")
                    {
                        index++;
                        if (tokens[index].VP == "{")
                        {
                            index++;
                            while (tokens[index].VP != "}")
                            {
                                if (body())
                                {
                                    continue;
                                }
                                else
                                {
                                    break;
                                }
                            }
                            if (tokens[index].VP == "}")
                            {
                                // if (other())
                                // {
                                //     return true;
                                // }
                                return true;
                            }
                        }
                    }
                }
            }
        }
        // }
        return false;
    }

    bool start()
    {
        if (tokens[index].VP == "public")
        {
            index++;
            if (tokens[index].VP == "class")
            {
                index++;
                if (tokens[index].VP == "Program")
                {
                    index++;
                    if (tokens[index].VP == "{")
                    {
                        index++;

                        while (!main_fn())
                        {
                            if (func_st())
                            {
                                index++;
                                continue;
                            }
                            else
                                break;
                        }
                        if (tokens[index].VP == "}")
                        {
                            if (other())
                            {
                                return true;
                            }
                        }
                    }
                }
            }
        }
        lineNumber = tokens[index].line;
        return false;
    }
};
