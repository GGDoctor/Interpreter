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
 * @param symbolTableEntry - entry in the symbol table
 * @brief takes a symbol table entry as input and adds it to ast and cst maps
 */
void Interpreter::populateMappings(list<TableEntry> symbolTable, LCRS *_ast, LCRS *_cst) {
    //Create two LCRS's to map to eachother
    LCRS* abstract = _ast;
    LCRS* concrete = _cst;

    while (abstract && concrete) {
        //adds to the map cstByAst with abstract being the Key, and concrete being the value
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
            //maps the data FUNCTION table entry to the node of either the _cst and _ast
            cstBySymbolTable[entry] = _cst;
            astBySymbolTable[entry] = _ast;

        } else if (entry.identifierType == "procedure") {

            while (_cst->token.character != "procedure") {
                while (_cst->rightSibling) _cst = _cst->rightSibling;
                while (_ast->rightSibling) _ast = _ast->rightSibling;
                _cst = _cst->leftChild;
                _ast = _ast->leftChild;
            }
            //maps the data PROCEDURE table entry to the node of either the _cst and _ast
            cstBySymbolTable[entry] = _cst;
            astBySymbolTable[entry] = _ast;

        } else if (entry.identifierType == "datatype") {

            if (entry.datatype == "int") {

                
                if (commaCounter > 0) commaCounter--;
                //move all the way to the right, to shift the left child over by one
                else {
                    while (_cst->rightSibling) _cst = _cst->rightSibling;
                    while (_ast->rightSibling) _ast = _ast->rightSibling;
                    if (_cst->leftChild) _cst = _cst->leftChild;
                    if (_ast->leftChild) _ast = _ast->leftChild;
                }

                //finds the declaration
                while (_cst->token.character != "int") {
                    while (_cst->rightSibling) _cst = _cst->rightSibling;
                    while (_ast->rightSibling) _ast = _ast->rightSibling;
                    _cst = _cst->leftChild;
                    _ast = _ast->leftChild;

                }
                //maps the data INT table entry to the node of either the _cst and _ast
                cstBySymbolTable[entry] = _cst;
                astBySymbolTable[entry] = _ast;

                LCRS* tempCst = _cst;
                LCRS* tempAst = _ast;

                //adds to commaCounter in order for more datatypes to be added?
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
                //maps the data CHAAR table entry to the node of either the _cst and _ast
                cstBySymbolTable[entry] = _cst;
                astBySymbolTable[entry] = _ast;

            } else if (entry.datatype == "bool") {

                while (_cst->token.character != "bool") {
                    while (_cst->rightSibling) _cst = _cst->rightSibling;
                    while (_ast->rightSibling) _ast = _ast->rightSibling;
                    _cst = _cst->leftChild;
                    _ast = _ast->leftChild;
                }
                //maps the data BOOL table entry to the node of either the _cst and _ast
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


void Interpreter::iterateMaps(unordered_map<TableEntry, LCRS*, TableEntryHash>astSym, unordered_map<TableEntry, LCRS*, TableEntryHash>cstSym, unordered_map<LCRS*, LCRS*>cstAst){
    for(auto[entry, astNode]:astSym){
        if(entry.identifierName == "main"){
            //cout << "TEST " << cstAst.at(astNode)->token.lineNumber << endl;
            cout << "TEST " << cstSym.at(entry)->token.character << endl;
        }
    }
}




void Interpreter::executeStack(/*Stack testStack*/){
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
            
            string print_string;
            for(char c : abstractSyntaxTree->rightSibling->token.character)
            {  
                print_string += c; 
            }
            for (int i = 0; i < print_string.size(); i ++)
            {
                if (print_string.at(i) == '%' && print_string.at(i+1) == 'd')
                {
                    //Do work

                   // cout << abstractSyntaxTree->rightSibling->token.character << endl;
                }
            }
            cout << print_string << endl;
            // cout << abstractSyntaxTree->rightSibling->token.character << endl;
            return;
        }
        else
        {
            abstractSyntaxTree = abstractSyntaxTree->rightSibling;
        }
    }
}

void Interpreter::doMath(ProcessingStack workingStack, int scope)
{
    vector<int> maths;
    int returnVar = 0;
    int returnFuncVar = 0;
    bool firstVar = true;
    bool firstFuncVar = true;
    while(workingStack.Top()){
        for (int varz = 0; varz < variables.size(); varz ++)
        {
        //cout << "Token character: " <<workingStack.Top()->astNode->token.character << endl;
        //cout << "Variable Name: "<< variables.at(varz).value_name << endl;
            if((variables.at(varz).value_name == workingStack.Top()->astNode->token.character) && (variables.at(varz).scope == scope))
            {
                //pushes the value of the variable within scope to the maths vector
                maths.push_back(variables.at(varz).value);
                //marks the first variable being the variable that will be changed
                if(firstVar){
                    returnVar = varz;
                    firstVar = false;
                }

        
    }
    

}

