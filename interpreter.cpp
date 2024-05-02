#include "interpreter.hpp"
using namespace std;


Interpreter::Interpreter(AbstractSyntaxTree ast, SymbolTable symbolTable, RecursiveDescentParser concreteSyntaxTree): 
ast(ast), symbolTable(symbolTable), concreteSyntaxTree(concreteSyntaxTree), programCounter(0) {

    execute();
}

void Interpreter::execute() {
        // Start execution from the main procedure
        list<TableEntry> newTable = symbolTable.table;
        for( auto i: newTable){
            if(i.identifierName == "main"){
                cout << "HERE " << i.scope << endl;
                int mainLine = FindMain(ast.abstractSyntaxTree, concreteSyntaxTree.getConcreteSyntaxTree());
                cout << mainLine << endl;
                break;
            }
        }
        //executeNode(ast.abstractSyntaxTree);
}

int Interpreter::FindMain(LCRS* mainAST, LCRS* mainCST){
    LCRS* tempCST = mainCST;
    LCRS* tempAST = mainAST;
    // while(tempCST->leftChild != nullptr){
    //     while(tempCST->rightSibling != nullptr){
    //         if(tempCST->token.character == "main"){
    //             cout << "I found main on line " << tempCST->token.lineNumber << endl;
    //             return tempCST->token.lineNumber;
    //         }
    //         tempCST = tempCST->rightSibling;
    //     }
    
    // }
    //cout << tempCST->token.character << endl;
    while(tempCST != nullptr){
        cout << tempCST->leftChild->token.character << endl;
        if(tempCST->token.character == "procedure" && tempCST->rightSibling->token.character == "main"){
            return tempCST->token.lineNumber;
        }
        while(tempCST){
            tempCST = tempCST->rightSibling;
        }
        tempCST = tempCST->leftChild;
    }
    
    // if(tempCST == nullptr){
    //     return 0;
    // }
    // cout << tempCST->token.character << " ";

    // if(tempCST->token.character == "main"){
    //     cout << "I found main on line " << tempCST->token.lineNumber << endl;
    //     return tempCST->token.lineNumber;
    // }

    // FindMain(tempAST, tempCST->rightSibling);
    // cout << endl;
    // FindMain(tempAST->leftChild, tempCST->leftChild);
    return -1;
}
/*
void Interpreter::executeNode(ASTNode* node) {
    if (!node) return;

    updateProgramCounter(node);  // Update the program counter before execution

    switch (node->type) {
        case NodeType::FunctionCall:
            handleFunctionCall(node);
            break;
        case NodeType::Expression:
            evalStack.push(evaluatePostfixExpression(node->tokens));
            break;
        case NodeType::Assignment:
            if (node->children.size() > 1) {
                int value = evalStack.top(); evalStack.pop();
                symbolTable->updateSymbol(node->children[0]->token.value, value);
            }
            break;
        // Add other cases as necessary for different types of statements
    }

    // Recursively execute child nodes if not a control flow statement
    for (auto child : node->children) {
        executeNode(child);
    }
}

int Interpreter::evaluatePostfixExpression(const std::vector<Token>& tokens) {
    std::stack<int> stack;
    for (const auto& token : tokens) {
        if (token.isOperand()) {
            stack.push(stoi(token.value));
        } else {
            int rhs = stack.top(); stack.pop();
            int lhs = stack.top(); stack.pop();
            switch (token.type) {
                case TokenType::Plus: stack.push(lhs + rhs); break;
                case TokenType::Minus: stack.push(lhs - rhs); break;
                case TokenType::Multiply: stack.push(lhs * rhs); break;
                case TokenType::Divide: stack.push(lhs / rhs); break;
                // Handle other operators
            }
        }
    }
    return stack.top();
}

void Interpreter::handleFunctionCall(ASTNode* node) {
    // Placeholder for function call handling logic
}

void Interpreter::updateProgramCounter(ASTNode* node) {
    // Placeholder for program counter logic, e.g., handling loops, if-else conditions
}
*/

// int main() {
//     // Setup and execute the interpreter
//     // AbstractSyntaxTree* ast; // Assume AST is created elsewhere
//     // SymbolTable* symbolTable; // Assume SymbolTable is populated elsewhere

//     Interpreter interpreter(ast, symbolTable);
//     interpreter.execute();

//     return 0;
// }


