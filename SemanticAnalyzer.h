#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class MemberEntry
{
public:
    string Name;
    string Type;
    string AccessModifier;
    string TypeModifier;
};

class MainEntry
{
public:
    string Name;
    string Type;
    string AccessModifier;
    string TypeModifier;
    string Parent;
    vector<MemberEntry> MemberTable;
};

class ScopeEntry
{
public:
    string Name;
    string Type;
    int Scope;
};

class SemanticAnalyzer
{
private:
    vector<MainEntry> mainTable;
    vector<ScopeEntry> currentScopeTable;
    int scopeCounter;

public:
    SemanticAnalyzer() : scopeCounter(0) {}
    vector<string> error;
    void createScopeTable()
    {
        scopeCounter++;
    }

    bool insertDataIntoMainTable(string name, string type, string accessModifier, string typeModifier, string parent)
    {
        if (lookupInMainTable(name))
        {
            string errorMessage = "Re-declare error: " + name + " already declared.";
            cerr << errorMessage << endl;
            error.push_back(errorMessage);
            return false;
        }

        mainTable.push_back({name, type, accessModifier, typeModifier, parent, {}});
        return true;
    }

    bool insertDataIntoMemberTable(string className, string name, string type, string accessModifier, string typeModifier)
    {
        MainEntry *classEntry = lookupInMainTable(className);
        if (!classEntry)
        {
            string errorMessage = "Undeclared Error: Class " + className + " not found.";
            cerr << errorMessage << endl;
            error.push_back(errorMessage);
            return false;
        }

        vector<MemberEntry> &memberTable = classEntry->MemberTable;
        if (lookupInMemberTable(className, name))
        {
            string errorMessage = "Re-declare error: " + name + " already declared in class " + className + ".";
            cerr << errorMessage << endl;
            error.push_back(errorMessage);
            return false;
        }

        memberTable.push_back({name, type, accessModifier, typeModifier});
        return true;
    }

    MainEntry *lookupInMainTable(string name)
    {
        auto it = find_if(mainTable.begin(), mainTable.end(), [&name](const MainEntry &entry)
                          { return entry.Name == name; });

        return (it != mainTable.end()) ? &(*it) : nullptr;
    }

    MemberEntry *lookupInMemberTable(string className, string name)
    {
        MainEntry *classEntry = lookupInMainTable(className);
        if (!classEntry)
        {
            string errorMessage = "Undeclared Error: Class " + className + " not found.";
            cerr << errorMessage << endl;
            error.push_back(errorMessage);
            return nullptr;
        }

        vector<MemberEntry> &memberTable = classEntry->MemberTable;
        auto it = find_if(memberTable.begin(), memberTable.end(), [&name](const MemberEntry &entry)
                          { return entry.Name == name; });

        return (it != memberTable.end()) ? &(*it) : nullptr;
    }

    bool lookupInScopeTable(string name, int scope)
    {
        return any_of(currentScopeTable.begin(), currentScopeTable.end(), [&name, scope](const ScopeEntry &entry)
                      { return entry.Name == name && entry.Scope == scope; });
    }

    bool insertDataIntoScopeTable(string name, string type)
    {
        if (lookupInScopeTable(name, scopeCounter))
        {
            string errorMessage = "Re-declare Error: " + name + " already declared in this scope.";
            cerr << errorMessage << endl;
            error.push_back(errorMessage);
            return false;
        }

        currentScopeTable.push_back({name, type, scopeCounter});
        return true;
    }

    void printResults()
    {
        cout << "Main Table:" << endl;
        for (const auto &mainEntry : mainTable)
        {
            cout << "Name: " << mainEntry.Name << ", Type: " << mainEntry.Type
                 << ", Access Modifier: " << mainEntry.AccessModifier
                 << ", Type Modifier: " << mainEntry.TypeModifier
                 << ", Parent: " << mainEntry.Parent << endl;

            cout << "Member Table:" << endl;
            for (const auto &memberEntry : mainEntry.MemberTable)
            {
                cout << "Name: " << memberEntry.Name << ", Type: " << memberEntry.Type
                     << ", Access Modifier: " << memberEntry.AccessModifier
                     << ", Type Modifier: " << memberEntry.TypeModifier << endl;
            }

            cout << "------------------------" << endl;
        }

        cout << "Scope Table:" << endl;
        for (const auto &scopeEntry : currentScopeTable)
        {
            cout << "Name: " << scopeEntry.Name << ", Type: " << scopeEntry.Type
                 << ", Scope: " << scopeEntry.Scope << endl;
        }

        cout << "------------------------" << endl;
    }

    bool checkErrors()
    {
        if (error.empty())
        {
            return true;
        }
        else
            return false;
    }
};