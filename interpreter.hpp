#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "abstractsyntaxtree.hpp"
#include "symboltable.hpp"
#include "RecursiveDescentParser.hpp"
#include "stack.hpp"
#include "processingstack.hpp"
//#include <stack>
#include <vector>
#include <unordered_map> 
#include <sstream>
struct Variable{ 
    string value_name;
    int value;
    int scope;
    };
struct FunctionVariable{ 
    string value_name;
    int value;
    int scope;
    LCRS* head;
    };


class Interpreter {
public:
    

    Interpreter(AbstractSyntaxTree ast, RecursiveDescentParser cst, SymbolTable symbolTable);

    void printAstCstBySymbolTable();
    void printCstByAst();
    void executeMain(LCRS* abstractSyntaxTree, int scope);
    void doMath(ProcessingStack workingStack, int scope);
    
    /*
    Interpreter(LCRS* ast, SymbolTable symbolTable);
    void execute(); // Execute the AST starting from the main procedure
    */
private:
    LCRS* ast;
    LCRS* cst;

    vector<Variable> variables;
    vector<FunctionVariable> functionVariables;
    /**
     * maps each entry in symbol table to its corresponding node in ast
     */ 
    unordered_map<TableEntry, LCRS*, TableEntryHash> astBySymbolTable;  

    /**
     * maps each entry in symbol table to its corresponding node in cst
     */
    unordered_map<TableEntry, LCRS*, TableEntryHash> cstBySymbolTable;  

    /**
     * maps ast node to its corresponding cst node
     * @remark .first(left) is ast and .second(right) is cst
     */
    unordered_map<LCRS*, LCRS*> cstByAst;

    /**
     * @param symbolTable - an stl linked list representing our symbol table
     * @param _ast - the abstract syntax tree
     * @param _cst - the concrete syntax tree
     * @brief takes a symbol table entry as input and adds it to ast and cst maps
     */
    void populateMappings(list<TableEntry> symbolTable, LCRS *_ast, LCRS *_cst);

    void iterateMaps(unordered_map<TableEntry, LCRS*, TableEntryHash>, unordered_map<TableEntry, LCRS*, TableEntryHash>, unordered_map<LCRS*, LCRS*>);

    

    //Token executeNumericalExpression(Stack numberStack);


/*
    LCRS* abstractSyntaxTree;
    SymbolTable symbolTable;
    //AbstractSyntaxTree* ast;   // Pointer to the AST for the program
    //SymbolTable* symbolTable;  // Pointer to the symbol table
    int programCounter;        // Program counter to manage control flow
    std::stack<int> evalStack; // Stack to evaluate postfix expressions

    void executeNode(LCRS* node); // Recursive method to execute a node
    int evaluatePostfixExpression(const std::vector<Token>& tokens); // Evaluate postfix expressions
    void handleFunctionCall(LCRS* node); // Handle function calls
    void updateProgramCounter(LCRS* node); // Update program counter based on control structures
    
*/
};


#endif /*INTERPRETER_HPP*/

