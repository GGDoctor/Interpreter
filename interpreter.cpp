#include "interpreter.hpp"
using namespace std;


Interpreter::Interpreter(AbstractSyntaxTree ast, RecursiveDescentParser cst, SymbolTable symbolTable) {
    this->ast = ast.getAbstractSyntaxTree();
    this->cst = cst.getConcreteSyntaxTree();
    LCRS* abstract = this->ast;
    LCRS* concrete = this->cst;

    populateMappings(symbolTable.table, abstract, concrete);

    iterateMaps(astBySymbolTable, cstBySymbolTable, cstByAst);

}

/**
  * @param symbolTable - an stl linked list representing our symbol table
  * @param _ast - the abstract syntax tree
  * @param _cst - the concrete syntax tree
  * @brief takes a symbol table entry as input and adds it to ast and cst maps
  */
void Interpreter::populateMappings(list<TableEntry> symbolTable, LCRS *astNode, LCRS *cstNode) {
    LCRS *_ast = astNode;
    LCRS *_cst = cstNode;
    LCRS *abstract = astNode;
    LCRS *concrete = cstNode;
    // used for multiple variables declared on same line seperated by commas
    int commaCounter = 0;
    bool dontWorryAboutCommas = false;

    while (abstract && concrete) {
        cstByAst[abstract] = concrete;

        LCRS *tempConcrete = concrete;
        while (concrete->rightSibling) {
            if (concrete->token.character == "," && tempConcrete->token.character != "printf") 
                commaCounter++;
            concrete = concrete->rightSibling;
        }

        if (commaCounter > 0) {
            for (int i = 0; i < commaCounter; i++) {
                while (abstract->rightSibling) abstract = abstract->rightSibling;
                abstract = abstract->leftChild;
                cstByAst[abstract] = tempConcrete;
            }
        }

        if (tempConcrete->token.character == "for") {
            for (int i = 0; i < 2; i++) {
                while (abstract->rightSibling) abstract = abstract->rightSibling;
                abstract = abstract->leftChild;
                cstByAst[abstract] = tempConcrete;
            }
        }

        while (abstract->rightSibling) abstract = abstract->rightSibling;
        if (abstract) abstract = abstract->leftChild;
        if (concrete) concrete = concrete->leftChild;

        commaCounter = 0;
    }

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
            cstByAst[_ast] = _cst;

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

                if (commaCounter > 0) {
                    dontWorryAboutCommas = true;
                    commaCounter--;
                    while (_ast->rightSibling) _ast = _ast->rightSibling;
                    if (_ast->leftChild) _ast = _ast->leftChild;
                }
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

                if (dontWorryAboutCommas && commaCounter == 0) dontWorryAboutCommas = false;
                else {
                    while (tempCst->rightSibling) {
                        if (tempCst->token.character == ",") commaCounter++;
                        tempCst = tempCst->rightSibling;   
                    }
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

void Interpreter::printAstCstBySymbolTable() {
    for (auto [entry, astNode] : astBySymbolTable) {
        LCRS *cstNode = cstByAst[astNode];
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

        cout << "\n\tcst line " << cstNode->token.lineNumber << ": ";

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

void Interpreter::printCstByAst() {
    for (auto &[astNode, cstNode] : cstByAst) {
        LCRS *_ast = astNode;
        LCRS *_cst = cstNode;
        cout << "ast line " << _ast->token.lineNumber << ": ";

        while (_ast) {
            cout << _ast->token.character;
            if (_ast->rightSibling) {
                cout << " -> ";
            }
            _ast = _ast->rightSibling;
        }

        cout << "\ncst line " << _cst->token.lineNumber << ": ";

        while (_cst) {
            cout << _cst->token.character;
            if (_cst->rightSibling) {
                cout << " -> ";
            }
            _cst = _cst->rightSibling;
        }

        cout << "\n\n";
    }
}

void Interpreter::iterateMaps(unordered_map<TableEntry, LCRS*, TableEntryHash>astSym, unordered_map<TableEntry, LCRS*, TableEntryHash>cstSym, unordered_map<LCRS*, LCRS*>cstAst){
    for(auto[entry, cstNode]:cstSym){
        ProcessingStack workingStack;
        //iterates through the cst to grab every token in the line
        if(entry.identifierName == "main"){
            while(cstNode){
                //workingStack.Push(cstNode);
                cout << cstNode->token.character << " ";
                cstNode = cstNode->rightSibling;
                }
                cout << endl;
            }
            //executeStack(workingStack);
    }
}




void Interpreter::executeStack(ProcessingStack workingStack){
    while(workingStack.head != nullptr){
        cout << workingStack.Top()->astNode->token.character << " ";
        Processing_Node* temp;
        temp = workingStack.head->next;
        //workingStack.Pop();
         workingStack.head = temp;
    }
    cout << '\n';
    
/*    
    Stack enteredStack;
    
    Token mockToken1;
    mockToken1.character = "1";
    mockToken1.type = INTEGER;
    mockToken1.lineNumber = 1;

    Token mockToken2;
    mockToken2.character = "3";
    mockToken2.type = INTEGER;
    mockToken2.lineNumber = 1;

    Token mockToken3;
    mockToken3.character = "+";
    mockToken3.type = PLUS;
    mockToken3.lineNumber = 1;

    Token mockToken4;
    mockToken4.character = "2";
    mockToken4.type = INTEGER;
    mockToken4.lineNumber = 1;

    Token mockToken5;
    mockToken5.character = "*";
    mockToken5.type = ASTERISK;
    mockToken5.lineNumber = 1;

    enteredStack.Push(mockToken5);
    enteredStack.Push(mockToken4);
    enteredStack.Push(mockToken3);
    enteredStack.Push(mockToken2);
    enteredStack.Push(mockToken1);

    Stack mockStack;

    while (enteredStack.head != nullptr)
    {
        if(enteredStack.Top().type == INTEGER){
            mockStack.Push(enteredStack.Top());

            cout << "Integer Found: " << enteredStack.Top().character << endl;
            enteredStack.head = enteredStack.head->next;


        }
        else if(enteredStack.Top().type == PLUS){
            mockStack.Push(enteredStack.Top());
            //mockStack.Push(executeNumericalExpression(mockStack));
            cout << "Plus Found: " << enteredStack.Top().character << endl;
            enteredStack.head = enteredStack.head->next;
        }
        else if(enteredStack.Top().type == ASTERISK){
            mockStack.Push(enteredStack.Top());
            cout << "Asterisk Found: " << enteredStack.Top().character << endl;
            
            enteredStack.head = enteredStack.head->next;
        }
        
        
    }
    
    cout << endl; */
} 

// Token Interpreter::executeNumericalExpression(Stack numberStack){

// }

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

// int main() {
//     // Setup and execute the interpreter
//     // AbstractSyntaxTree* ast; // Assume AST is created elsewhere
//     // SymbolTable* symbolTable; // Assume SymbolTable is populated elsewhere

//     Interpreter interpreter(ast, symbolTable);
//     interpreter.execute();

//     return 0;
// }


