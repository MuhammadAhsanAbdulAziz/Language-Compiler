#include "Lexer.h"
#include "SemanticAnalyzer.h"
using namespace std;

class TerminalFunction
{
private:
    int index = 0;
    SemanticAnalyzer SA;
    string className = " ", name = " ", type = " ", accessModifer = " ", typeModifer = " ", parent = " ", paramsList = "", constructorName = "", returnType = " ",funcName = " ";
    int inherit = 0;

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
            type = tokens[index].VP;
            index++;
            return true;
        }
        return false;
    }

    bool IDENTIFIER()
    {
        if (tokens[index].CP == "IDENTIFIER")
        {
            name = tokens[index].VP;
            index++;
            return true;
        }
        return false;
    }

    bool OE()
    {
        if (tokens[index].CP == "INC_DEC" || tokens[index].CP == "Number" || tokens[index].CP == "Word" || tokens[index].VP == "this" || tokens[index].VP == "true" || tokens[index].VP == "false" || tokens[index].CP == "ARITHMATIC_OPERATORS" || tokens[index].CP == "LOGICAL_OPERATORS" || tokens[index].CP == "ASSIGN_OPERATORS" || tokens[index].CP == "IDENTIFIER" || tokens[index].VP == "this" || tokens[index].VP == "." || tokens[index].VP == "(" || tokens[index].VP == ")" || tokens[index].VP == "[" || tokens[index].VP == "]")
        {
            index++;
            return true;
        }
        return false;
    }

    bool assign_op()
    {
        if (tokens[index].CP == "ASSIGN_OPERATORS")
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
            index++;
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
                SA.createScopeTable();
                index++;
                if (array_init())
                {
                    return true;
                }
            }
            else if (tokens[index].VP == ";")
            {
                index++;
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
                        // SA.insertDataIntoScopeTable(name, type);
                        index++;
                        return true;
                    }
                    else if (tokens[index].VP == "=")
                    {
                        index++;
                        if (init1())
                        {
                            // SA.insertDataIntoScopeTable(name, type);
                            return true;
                        }
                    }
                    else if (tokens[index].VP == "[")
                    {
                        if (array_decl())
                        {
                            // SA.insertDataIntoScopeTable(name,type+ " Array");
                            return true;
                        }
                    }
                    else if (tokens[index].VP == "<")
                    {
                        index++;
                        if (tokens[index].VP == "Stack")
                        {
                            index++;
                            if (tokens[index].VP == ">")
                            {
                                index++;
                                if (tokens[index].VP == ";")
                                {
                                    // SA.insertDataIntoScopeTable(name, type);
                                    index++;
                                    return true;
                                }
                            }
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
            name = tokens[index].VP;
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
        if (tokens[index].CP == "IDENTIFIER" || tokens[index].CP == "Word" || tokens[index].CP == "Number" || tokens[index].VP == "true" || tokens[index].VP == "false")
        {
            index++;
            return true;
        }
        return false;
    }

    bool cond()
    {
        if (tokens[index].CP == "IDENTIFIER" || tokens[index].CP == "Word" || tokens[index].CP == "Number" || tokens[index].VP == "true" || tokens[index].VP == "false")
        {
            index++;
            while (tokens[index].VP != ")")
            {
                if (i_c())
                {
                    continue;
                }
                if (tokens[index].CP == "RELATIONAL_OPERATORS" || tokens[index].CP == "LOGICAL_OPERATORS" || tokens[index].CP == "ARITHMATIC_OPERATORS")
                {
                    index++;
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
                        SA.createScopeTable();
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

    bool obj_init()
    {
        if (tokens[index].CP == "IDENTIFIER" || tokens[index].CP == "Number" || tokens[index].CP == "Word" || tokens[index].CP == "Boolean" || tokens[index].VP == "[" || tokens[index].VP == "]" || tokens[index].VP == "(" || tokens[index].VP == ")" || tokens[index].VP == ",")
        {
            index++;
            return true;
        }
        return false;
    }

    bool obj_decl()
    {
        if (tokens[index].CP == "IDENTIFIER")
        {
            name = tokens[index].VP;
            index++;
            if (tokens[index].VP == "(")
            {
                index++;
                while (tokens[index].VP != ")")
                {
                    if (obj_init())
                    {
                        continue;
                    }
                    else
                        break;
                }
                if (tokens[index].VP == ")")
                {
                    index++;
                    // SA.insertDataIntoScopeTable(name, type);
                    // if (tokens[index].VP == ";")
                    // {
                    //     index++;
                    //     return true;
                    // }
                    return true;
                }
            }
        }

        return false;
    }

    bool assign_st()
    {
        if (tokens[index].VP == "this")
        {
            index++;
            if (tokens[index].CP == "IDENTIFIER")
            {
                if (OE())
                {
                    return true;
                }
                else if (tokens[index].CP == "ASSIGN_OPERATORS")
                {
                    if (OE())
                    {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    bool body()
    {
        if (tokens[index].CP == "IDENTIFIER" && tokens[index + 1].CP == "IDENTIFIER")
        {
            type = tokens[index].VP;
            index++;
            obj_decl();
            SA.insertDataIntoScopeTable(name, type);
            return true;
        }
        if (decl())
        {
            SA.insertDataIntoScopeTable(name, type);
            return true;
        }
        if (looping_till() || if_st() || init() || assign_st())
        {
            return true;
        }
        else if (tokens[index].VP == ";" || tokens[index].VP == "this" || tokens[index].VP == "." || tokens[index].CP == "Word" || tokens[index].CP == "Number" || tokens[index].CP == "Boolean" || tokens[index].CP == "IDENTIFIER" || tokens[index].VP == "true" || tokens[index].VP == "false" || tokens[index].VP == "}")
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
                SA.createScopeTable();
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
                        SA.createScopeTable();
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
                            if (if_not_st() || do_this())
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
                        SA.createScopeTable();
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
                            if (if_not_st() || if_st() || do_this())
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

    bool class_type()
    {
        if (tokens[index].VP == "abstract" || tokens[index].VP == "sealed")
        {
            typeModifer = tokens[index].VP;
            index++;
            return true;
        }
        return false;
    }

    bool inheritence()
    {
        if (tokens[index].VP == "(")
        {
            index++;
            if (tokens[index].CP == "IDENTIFIER")
            {
                if (SA.lookupInMainTable(tokens[index].VP))
                {
                    parent = tokens[index].VP;
                }
                else {
                    
                    cerr << "No Parent Class with name \"" << tokens[index].VP <<  "\" found"  << endl;
                }
                index++;
                if (tokens[index].VP == ")")
                {
                    index++;
                    return true;
                }
            }
        }
        return false;
    }

    bool constructor()
    {
        if (tokens[index].CP == "IDENTIFIER")
        {
            name = tokens[index].VP;
            index++;
            if (tokens[index].VP == "(")
            {
                index++;
                if (sp_decl())
                {
                    if (tokens[index].VP == ";")
                    {
                        index++;
                        return true;
                    }
                    else if (tokens[index].VP == "{")
                    {
                        SA.createScopeTable();
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
                else if (tokens[index].VP == ")")
                {
                    index++;
                    if (tokens[index].VP == "{")
                    {
                        SA.createScopeTable();
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

    bool destructor()
    {
        if (tokens[index].VP == "~")
        {
            index++;
            if (tokens[index].CP == "IDENTIFIER")
            {
                name = "~"+tokens[index].VP;
                index++;
                if (tokens[index].VP == "(")
                {
                    index++;
                    if (tokens[index].VP == ")")
                    {
                        SA.insertDataIntoMemberTable(className,name,"destructor",accessModifer," ");
                        index++;
                        if (tokens[index].VP == "{")
                        {
                            SA.createScopeTable();
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
        }
        return false;
    }

    bool statement()
    {
        if (tokens[index].CP == "IDENTIFIER" && tokens[index + 1].VP == "(")
        {
            constructorName = tokens[index].VP;
            if (constructor())
            {
                SA.insertDataIntoMemberTable(className, constructorName, paramsList, accessModifer, "");
                return true;
            }
        }
        else if (decl())
        {
            SA.insertDataIntoMemberTable(className, name, type, accessModifer, " ");
            return true;
        }
        else if (func_st())
        {
            return true;
        }
        else if (init() || destructor())
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

    bool cbody()
    {
        if (tokens[index].CP == "ACCESS_MODIFIERS")
        {
            accessModifer = tokens[index].VP;
            index++;
            if (statement())
            {
                return true;
            }
        }
        return false;
    }

    bool class_def()
    {
        if (class_type())
        {
            if (tokens[index].CP == "ACCESS_MODIFIERS")
            {
                accessModifer = tokens[index].VP;
                index++;
                if (tokens[index].VP == "class")
                {
                    type = tokens[index].VP;
                    index++;
                    if (tokens[index].CP == "IDENTIFIER")
                    {
                        className = tokens[index].VP;
                        index++;
                        if (inheritence())
                        {
                            if (tokens[index].VP == "{")
                            {
                                SA.createScopeTable();
                                SA.insertDataIntoMainTable(className, type, accessModifer, typeModifer, parent);
                                index++;
                                while (tokens[index].VP != "}")
                                {
                                    if (cbody())
                                    {
                                        continue;
                                    }
                                    else
                                        break;
                                }
                                if (tokens[index].VP == "}")
                                {
                                    index++;
                                    return true;
                                }
                            }
                        }
                        else if (tokens[index].VP == "{")
                        {
                            SA.createScopeTable();
                            SA.insertDataIntoMainTable(className, type, accessModifer, typeModifer, " ");
                            index++;
                            while (tokens[index].VP != "}")
                            {
                                if (cbody())
                                {
                                    continue;
                                }
                                else
                                    break;
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
        }
        else if (tokens[index].CP == "ACCESS_MODIFIERS")
        {
            accessModifer = tokens[index].VP;
            index++;
            if (tokens[index].VP == "class")
            {
                type = tokens[index].VP;
                index++;
                if (tokens[index].CP == "IDENTIFIER")
                {
                    className = tokens[index].VP;
                    index++;
                    if (inheritence())
                    {
                        SA.insertDataIntoMainTable(className, type, accessModifer, " ", parent);
                        if (tokens[index].VP == "{")
                        {
                            SA.createScopeTable();
                            index++;
                            while (tokens[index].VP != "}")
                            {
                                if (cbody())
                                {
                                    continue;
                                }
                                else
                                    break;
                            }
                            if (tokens[index].VP == "}")
                            {
                                index++;
                                return true;
                            }
                        }
                    }
                    else if (tokens[index].VP == "{")
                    {
                        SA.createScopeTable();
                        SA.insertDataIntoMainTable(className, type, accessModifer, " ", " ");
                        index++;
                        while (tokens[index].VP != "}")
                        {
                            if (cbody())
                            {
                                continue;
                            }
                            else
                                break;
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
                            if (DT())
                            {
                                paramsList += " " + name + " " + type;
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
                    if (DT())
                    {
                        paramsList +=" " +  name + " " + type;
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
            returnType = tokens[index].VP;
            index++;
            if (tokens[index].CP == "IDENTIFIER")
            {
                funcName = tokens[index].VP;
                index++;
                if (tokens[index].VP == "=>")
                {
                    index++;
                    if (tokens[index].VP == "(")
                    {
                        index++;
                        if (sp_decl())
                        {
                            paramsList+= " -> " + returnType;
                            SA.insertDataIntoMemberTable(className,funcName,paramsList,accessModifer,"");
                            if (tokens[index].VP == "{")
                            {
                                SA.createScopeTable();
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
            }
        }
        return false;
    }

    bool other()
    {
        while (tokens[index].VP != "#")
        {
            if (class_def())
            {
                continue;
            }
            else
                break;
        }
        if (tokens[index].VP == "#")
        {
            return true;
        }

        return false;
    }

    bool main_fn()
    {
        if (tokens[index].VP == "main" || tokens[index].VP == "Main")
        {
            name = tokens[index].VP;
            index++;
            if (tokens[index].VP == "=>")
            {
                index++;
                if (tokens[index].VP == "(")
                {
                    index++;
                    if (tokens[index].VP == ")")
                    {
                        SA.insertDataIntoMemberTable(className, name, "", "", "");
                        index++;
                        if (tokens[index].VP == "{")
                        {
                            SA.createScopeTable();
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
        }
        return false;
    }

    bool start()
    {
        while (tokens[index + 2].VP != "Main")
        {
            if (class_def())
            {
                continue;
            }
        }
        if (tokens[index].VP == "public")
        {
            accessModifer = tokens[index].VP;
            index++;
            if (tokens[index].VP == "class")
            {
                type = tokens[index].VP;
                index++;
                if (tokens[index].VP == "Main")
                {
                    className = tokens[index].VP;
                    SA.insertDataIntoMainTable(className, type, accessModifer, "", "");
                    index++;
                    if (tokens[index].VP == "{")
                    {
                        SA.createScopeTable();
                        index++;

                        while (!main_fn())
                        {
                            if (func_st())
                            {
                                SA.insertDataIntoMemberTable(className,name,paramsList,"","");
                                continue;
                            }
                            else
                                break;
                        }
                        if (tokens[index].VP == "}")
                        {
                            index++;
                            if (other())
                            {
                                SA.printResults();
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
