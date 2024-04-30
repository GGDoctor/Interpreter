#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "abstractsyntaxtree.hpp"
#include "symboltable.hpp"
#include <stack>
#include <vector>

class Interpreter {
public:
    Interpreter(AbstractSyntaxTree* ast, SymbolTable* symbolTable);

    void execute(); // Execute the AST starting from the main procedure

private:
    AbstractSyntaxTree* ast;   // Pointer to the AST for the program
    SymbolTable* symbolTable;  // Pointer to the symbol table
    int programCounter;        // Program counter to manage control flow
    std::stack<int> evalStack; // Stack to evaluate postfix expressions

    void executeNode(ASTNode* node); // Recursive method to execute a node
    int evaluatePostfixExpression(const std::vector<Token>& tokens); // Evaluate postfix expressions
    void handleFunctionCall(ASTNode* node); // Handle function calls
    void updateProgramCounter(ASTNode* node); // Update program counter based on control structures
};

#endif // INTERPRETER_HPP
