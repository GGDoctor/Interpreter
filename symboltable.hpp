#ifndef SYMBOLTABLE_HPP
#define SYMBOLTABLE_HPP

#include <iostream>
#include <vector>
#include <list>
#include "Tokenization.hpp" // Include the header where Token is defined
#include "RecursiveDescentParser.hpp"

using namespace std;

struct TableEntry {
    TableEntry() : identifierName(""), identifierType(""), datatype(""), 
                    datatypeIsArray(false), datatypeArraySize(0), scope(0) { }
    string identifierName;
    string identifierType;
    string datatype;
    bool datatypeIsArray;
    int datatypeArraySize;
    int scope;
};

bool operator==(const TableEntry& lhs, const TableEntry& rhs);

// Define a hash function for TableEntry
struct TableEntryHash {
    std::size_t operator()(const TableEntry& entry) const {
        // Combine hash values of individual members
        std::size_t hashValue = 0;
        hashValue ^= std::hash<std::string>{}(entry.identifierName);
        hashValue ^= std::hash<std::string>{}(entry.identifierType);
        hashValue ^= std::hash<std::string>{}(entry.datatype);
        hashValue ^= std::hash<bool>{}(entry.datatypeIsArray);
        hashValue ^= std::hash<int>{}(entry.datatypeArraySize);
        hashValue ^= std::hash<int>{}(entry.scope);
        return hashValue;
    }
};

struct ParamListEntry {
    ParamListEntry() : identifierName(""), datatype(""), 
                       datatypeIsArray(false), datatypeArraySize(0), 
                       scope(0) { }
    string paramListName;
    string identifierName;
    string datatype;
    bool datatypeIsArray;
    int datatypeArraySize;
    int scope;
};

enum StateDFA {
    OTHER_STATE,
    FUNCTION,
    PROCEDURE,
    VARIABLE,
};

// Symbol table class
class SymbolTable {
public:
    /**
     * @brief Constructs SymbolTable object
     * @param cst - The string version of the concrete syntax tree that was
     *          generated in RecursiveDescentParser
     */
    SymbolTable(RecursiveDescentParser concreteSyntaxTree);

    /**
     * @brief Output operator overload
     * @param os - The output stream operator
     * @param obj - The SymbolTable object to output
     * @returns The modified output stream
     * @remark Outputs symbol table according to project spec
     * 
     *      ex: cout << SymbolTableObj;
     */
    friend ostream& operator << (ostream& os, const SymbolTable& obj);
private:
    void parseParams(const vector<string>& params, int scope, const string& 
                                                                paramListName);
    list<TableEntry> table;
    list<ParamListEntry> paramTable;
    
    friend class AbstractSyntaxTree;
    friend class Interpreter;
};

#endif /* SYMBOLTABLE_HPP */
