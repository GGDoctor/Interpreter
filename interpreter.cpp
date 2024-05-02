#include "interpreter.hpp"
using namespace std;


Interpreter::Interpreter(AbstractSyntaxTree ast, SymbolTable symbolTable, RecursiveDescentParser concreteSyntaxTree): 
ast(ast), symbolTable(symbolTable), concreteSyntaxTree(concreteSyntaxTree), programCounter(0) {

    execute();
}

void Interpreter::execute() {
    // Start execution from the main procedure
    list<TableEntry> newTable = symbolTable.table;
    for (auto i : newTable) {
        if (i.identifierName == "main") {
            cout << "Main procedure found in the symbol table." << endl;

            // Call FindMain to get the line number of the main procedure
            int mainLine = FindMain(ast.abstractSyntaxTree, concreteSyntaxTree.getConcreteSyntaxTree());
            if (mainLine != -1) {
                cout << "Main procedure found at line " << mainLine << "." << endl;
            } else {
                cout << "Main procedure not found in the concrete syntax tree." << endl;
            }

            break;
        }
    }
}


int Interpreter::FindMain(LCRS* mainAST, LCRS* mainCST) {
    if (!mainCST) return -1;  // Check if the CST is null

    // Traverse the CST
    while (mainCST) {
        // Check if the current node contains the main procedure
        if (mainCST->token.character == "main") {
            return mainCST->token.lineNumber;  // Return the line number of the main procedure
        }

        // Recursively traverse child nodes
        int result = FindMain(mainAST, mainCST->leftChild);
        if (result != -1) {
            return result;  // Return the result if main is found in the child
        }

        // Move to the next sibling
        mainCST = mainCST->rightSibling;
    }

    return -1;  // Return -1 if main is not found
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


