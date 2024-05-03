#include "interpreter.hpp"
using namespace std;


Interpreter::Interpreter(AbstractSyntaxTree ast, RecursiveDescentParser cst, SymbolTable symbolTable) {
    this->ast = ast.getAbstractSyntaxTree();
    this->cst = cst.getConcreteSyntaxTree();
    LCRS* abstract = this->ast;
    LCRS* concrete = this->cst;

    populateMappings(symbolTable.table, abstract, concrete);

}

/**
 * @param symbolTableEntry - entry in the symbol table
 * @brief takes a symbol table entry as input and adds it to ast and cst maps
 */
void Interpreter::populateMappings(list<TableEntry> symbolTable, LCRS *_ast, LCRS *_cst) {
    LCRS* abstract = _ast;
    LCRS* concrete = _cst;

    while (abstract && concrete) {
        cstByAst[abstract] = concrete;
        
        while (abstract->rightSibling) abstract = abstract->rightSibling;
        while (concrete->rightSibling) concrete = concrete->rightSibling;
        abstract = abstract->leftChild;
        concrete = concrete->leftChild;
    }

    // used for multiple variables declared on same line seperated by commas
    int commaCounter = 0;
    
    for (const auto &entry : symbolTable) {
        if (entry.identifierType == "function") {

            while (_cst->token.character != "function") {
                while (_cst->rightSibling) _cst = _cst->rightSibling;
                while (_ast->rightSibling) _ast = _ast->rightSibling;
                _cst = _cst->leftChild;
                _ast = _ast->leftChild;
            }
            cstBySymbolTable[entry] = _cst;
            astBySymbolTable[entry] = _ast;

        } else if (entry.identifierType == "procedure") {

            while (_cst->token.character != "procedure") {
                while (_cst->rightSibling) _cst = _cst->rightSibling;
                while (_ast->rightSibling) _ast = _ast->rightSibling;
                _cst = _cst->leftChild;
                _ast = _ast->leftChild;
            }
            cstBySymbolTable[entry] = _cst;
            astBySymbolTable[entry] = _ast;

        } else if (entry.identifierType == "datatype") {

            if (entry.datatype == "int") {

                if (commaCounter > 0) commaCounter--;
                else {
                    while (_cst->rightSibling) _cst = _cst->rightSibling;
                    while (_ast->rightSibling) _ast = _ast->rightSibling;
                    if (_cst->leftChild) _cst = _cst->leftChild;
                    if (_ast->leftChild) _ast = _ast->leftChild;
                }

                while (_cst->token.character != "int") {
                    while (_cst->rightSibling) _cst = _cst->rightSibling;
                    while (_ast->rightSibling) _ast = _ast->rightSibling;
                    _cst = _cst->leftChild;
                    _ast = _ast->leftChild;

                }
                cstBySymbolTable[entry] = _cst;
                astBySymbolTable[entry] = _ast;

                LCRS* tempCst = _cst;
                LCRS* tempAst = _ast;

                while (tempCst->rightSibling) {
                    if (tempCst->token.character == ",") commaCounter++;
                    tempCst = tempCst->rightSibling;
                }

            } else if (entry.datatype == "char") {

                while (_cst->token.character != "char") {
                    while (_cst->rightSibling) _cst = _cst->rightSibling;
                    while (_ast->rightSibling) _ast = _ast->rightSibling;
                    _cst = _cst->leftChild;
                    _ast = _ast->leftChild;
                }
                cstBySymbolTable[entry] = _cst;
                astBySymbolTable[entry] = _ast;

            } else if (entry.datatype == "bool") {

                while (_cst->token.character != "bool") {
                    while (_cst->rightSibling) _cst = _cst->rightSibling;
                    while (_ast->rightSibling) _ast = _ast->rightSibling;
                    _cst = _cst->leftChild;
                    _ast = _ast->leftChild;
                }
                cstBySymbolTable[entry] = _cst;
                astBySymbolTable[entry] = _ast;

            }
        }
    }   

}

void Interpreter::printCstBySymbolTable() {
    for (auto [entry, cstNode] : cstBySymbolTable) {
        cout << "IDENTIFIER_NAME: " << entry.identifierName << '\n';
        cout << "IDENTIFIER_TYPE: " << entry.identifierType << '\n';
        cout << "DATATYPE: " << entry.datatype << '\n';
        cout << "DATATYPE_IS_ARRAY: ";
        if (entry.datatypeIsArray)
            cout << "yes\n";
        else
            cout << "no\n";
        cout << "DATATYPE_ARRAY_SIZE: " << entry.datatypeArraySize << '\n';
        cout << "SCOPE: " << entry.scope << '\n';
        cout << '\n';

        cout << "\tast line " << cstNode->token.lineNumber << ": ";

        while (cstNode) {
            cout << cstNode->token.character;
            if (cstNode->rightSibling) {
                cout << " -> ";
            }
            cstNode = cstNode->rightSibling;
        }

        cout << "\n\n";
    }
}

void Interpreter::printAstBySymbolTable() {
    for (auto [entry, astNode] : astBySymbolTable) {
        cout << "IDENTIFIER_NAME: " << entry.identifierName << '\n';
        cout << "IDENTIFIER_TYPE: " << entry.identifierType << '\n';
        cout << "DATATYPE: " << entry.datatype << '\n';
        cout << "DATATYPE_IS_ARRAY: ";
        if (entry.datatypeIsArray)
            cout << "yes\n";
        else
            cout << "no\n";
        cout << "DATATYPE_ARRAY_SIZE: " << entry.datatypeArraySize << '\n';
        cout << "SCOPE: " << entry.scope << '\n';
        cout << '\n';

        cout << "\tast line " << astNode->token.lineNumber << ": ";

        while (astNode) {
            cout << astNode->token.character;
            if (astNode->rightSibling) {
                cout << " -> ";
            }
            astNode = astNode->rightSibling;
        }

        cout << "\n\n";
    }
}



/*
Interpreter::Interpreter(LCRS* ast, SymbolTable symbolTable)
    : abstractSyntaxTree(ast), symbolTable(symbolTable), programCounter(0) {}

void Interpreter::execute() {

}

*/
/*
Interpreter::Interpreter(LCRS* ast, SymbolTable* symbolTable)
    : abstractSyntaxTree(ast), symbolTable(symbolTable), programCounter(0) {}

void Interpreter::execute() {
    if (abstractSyntaxTree) {
        // Start execution from the main procedure
        executeNode(abstractSyntaxTree);
    }
}

void Interpreter::executeNode(LCRS* node) {
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
    for (auto child : node->leftChild) {
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

int main() {
    // Setup and execute the interpreter
    AbstractSyntaxTree* ast; // Assume AST is created elsewhere
    SymbolTable* symbolTable; // Assume SymbolTable is populated elsewhere

    Interpreter interpreter(ast, symbolTable);
    interpreter.execute();

    return 0;
}
*/
