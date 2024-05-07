#include "interpreter.hpp"
using namespace std;

Interpreter::Interpreter(AbstractSyntaxTree ast, RecursiveDescentParser cst, SymbolTable symbolTable)
{
    this->ast = ast.getAbstractSyntaxTree();
    this->cst = cst.getConcreteSyntaxTree();
    LCRS *abstract = this->ast;
    LCRS *concrete = this->cst;

    populateMappings(symbolTable.table, abstract, concrete);

    iterateMaps(astBySymbolTable, cstBySymbolTable, cstByAst);
}

/**
 * @param symbolTable - an stl linked list representing our symbol table
 * @param _ast - the abstract syntax tree
 * @param _cst - the concrete syntax tree
 * @brief takes a symbol table entry as input and adds it to ast and cst maps
 */
void Interpreter::populateMappings(list<TableEntry> symbolTable, LCRS *astNode, LCRS *cstNode)
{
    LCRS *_ast = astNode;
    LCRS *_cst = cstNode;
    LCRS *abstract = astNode;
    LCRS *concrete = cstNode;
    // used for multiple variables declared on same line seperated by commas
    int commaCounter = 0;
    bool dontWorryAboutCommas = false;

    while (abstract && concrete)
    {
        
            
        cstByAst[abstract] = concrete;

        LCRS *tempConcrete = concrete;
        while (concrete->rightSibling)
        {
            if (concrete->token.character == "," && tempConcrete->token.character != "printf")
                commaCounter++;
            concrete = concrete->rightSibling;
        }

        if (commaCounter > 0)
        {
            for (int i = 0; i < commaCounter; i++)
            {
                while (abstract->rightSibling)
                    abstract = abstract->rightSibling;
                abstract = abstract->leftChild;
                cstByAst[abstract] = tempConcrete;
            }
        }

        if (tempConcrete->token.character == "for")
        {
            for (int i = 0; i < 2; i++)
            {
                while (abstract->rightSibling)
                    abstract = abstract->rightSibling;
                abstract = abstract->leftChild;
                cstByAst[abstract] = tempConcrete;
            }
        }

        while (abstract->rightSibling)
            abstract = abstract->rightSibling;
        if (abstract)
            abstract = abstract->leftChild;
        if (concrete)
            concrete = concrete->leftChild;

        commaCounter = 0;
    }

    for (const auto &entry : symbolTable)
    {
        Variable newVar;
        FunctionVariable newFuncVar;
        if (entry.identifierType == "function")
        {
            newFuncVar.scope = entry.scope;
            newFuncVar.value_name = entry.identifierName;
            newFuncVar.value = 0;
            newFuncVar.head = _ast;
            functionVariables.push_back(newFuncVar);

            newVar.scope = entry.scope;
            newVar.value_name = _cst->rightSibling->rightSibling->rightSibling->rightSibling->rightSibling->token.character;
            newVar.value = 0;
            variables.push_back(newVar);
            while (_cst->token.character != "function")
            {
                while (_cst->rightSibling)
                    _cst = _cst->rightSibling;
                while (_ast->rightSibling)
                    _ast = _ast->rightSibling;
                _cst = _cst->leftChild;
                _ast = _ast->leftChild;
            }
            
            
            cstBySymbolTable[entry] = _cst;
            astBySymbolTable[entry] = _ast;
            cstByAst[_ast] = _cst;
        }
        else if (entry.identifierType == "procedure")
        {

            while (_cst->token.character != "procedure")
            {
                while (_cst->rightSibling)
                    _cst = _cst->rightSibling;
                while (_ast->rightSibling)
                    _ast = _ast->rightSibling;
                _cst = _cst->leftChild;
                _ast = _ast->leftChild;
            }
            cstBySymbolTable[entry] = _cst;
            astBySymbolTable[entry] = _ast;
        }
        else if (entry.identifierType == "datatype")
        {

            if (entry.datatype == "int")
            {
                newVar.scope = entry.scope;
                newVar.value_name = entry.identifierName;
                newVar.value = 0;
                variables.push_back(newVar);

                if (commaCounter > 0)
                {
                    dontWorryAboutCommas = true;
                    commaCounter--;
                    while (_ast->rightSibling)
                        _ast = _ast->rightSibling;
                    if (_ast->leftChild)
                        _ast = _ast->leftChild;
                }
                else
                {
                    while (_cst->rightSibling)
                        _cst = _cst->rightSibling;
                    while (_ast->rightSibling)
                        _ast = _ast->rightSibling;
                    if (_cst->leftChild)
                        _cst = _cst->leftChild;
                    if (_ast->leftChild)
                        _ast = _ast->leftChild;
                }

                while (_cst->token.character != "int")
                {
                    while (_cst->rightSibling)
                        _cst = _cst->rightSibling;
                    while (_ast->rightSibling)
                        _ast = _ast->rightSibling;
                    _cst = _cst->leftChild;
                    _ast = _ast->leftChild;
                }
                cstBySymbolTable[entry] = _cst;
                astBySymbolTable[entry] = _ast;

                LCRS *tempCst = _cst;
                LCRS *tempAst = _ast;

                if (dontWorryAboutCommas && commaCounter == 0)
                    dontWorryAboutCommas = false;
                else
                {
                    while (tempCst->rightSibling)
                    {
                        if (tempCst->token.character == ",")
                            commaCounter++;
                        tempCst = tempCst->rightSibling;
                    }
                }
            }
            else if (entry.datatype == "char")
            {

                while (_cst->token.character != "char")
                {
                    while (_cst->rightSibling)
                        _cst = _cst->rightSibling;
                    while (_ast->rightSibling)
                        _ast = _ast->rightSibling;
                    _cst = _cst->leftChild;
                    _ast = _ast->leftChild;
                }
                cstBySymbolTable[entry] = _cst;
                astBySymbolTable[entry] = _ast;
            }
            else if (entry.datatype == "bool")
            {

                while (_cst->token.character != "bool")
                {
                    while (_cst->rightSibling)
                        _cst = _cst->rightSibling;
                    while (_ast->rightSibling)
                        _ast = _ast->rightSibling;
                    _cst = _cst->leftChild;
                    _ast = _ast->leftChild;
                }
                cstBySymbolTable[entry] = _cst;
                astBySymbolTable[entry] = _ast;
            }
        }
    }
}

void Interpreter::printAstCstBySymbolTable()
{
    for (auto [entry, astNode] : astBySymbolTable)
    {
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

        while (astNode)
        {
            cout << astNode->token.character;
            if (astNode->rightSibling)
            {
                cout << " -> ";
            }
            astNode = astNode->rightSibling;
        }

        cout << "\n\tcst line " << cstNode->token.lineNumber << ": ";

        while (cstNode)
        {
            cout << cstNode->token.character;
            if (cstNode->rightSibling)
            {
                cout << " -> ";
            }
            cstNode = cstNode->rightSibling;
        }

        cout << "\n\n";
    }
}

void Interpreter::printCstByAst()
{
    for (auto &[astNode, cstNode] : cstByAst)
    {
        LCRS *_ast = astNode;
        LCRS *_cst = cstNode;
        cout << "ast line " << _ast->token.lineNumber << ": ";

        while (_ast)
        {
            cout << _ast->token.character;
            if (_ast->rightSibling)
            {
                cout << " -> ";
            }
            _ast = _ast->rightSibling;
        }

        cout << "\ncst line " << _cst->token.lineNumber << ": ";

        while (_cst)
        {
            cout << _cst->token.character;
            if (_cst->rightSibling)
            {
                cout << " -> ";
            }
            _cst = _cst->rightSibling;
        }

        cout << "\n\n";
    }
}

void Interpreter::iterateMaps(unordered_map<TableEntry, LCRS *, TableEntryHash> astSym, unordered_map<TableEntry, LCRS *, TableEntryHash> cstSym, unordered_map<LCRS *, LCRS *> cstAst)
{
    cout << "Variable List" << endl;
    for(auto vars: variables){
        cout << vars.value_name << endl;
    }
    
    for (auto [entry, astNode] : astSym)
    {
        ProcessingStack workingStack;
        // iterates through the astSym map to find and execute main
        if (entry.identifierName == "main")
        {
            executeMain(astNode, entry.scope);
        }
    }
}



void Interpreter::executeMain(LCRS *abstractSyntaxTree, int scope)
{
    // iterate throught the syntax tree in order to start executing each line inside of main
    while (abstractSyntaxTree)
    {
        ProcessingStack workingStack;
        // find the assignment type and does whatever math it needs to do to reassign each value.
        
        if (abstractSyntaxTree->token.character == "Assignment" || abstractSyntaxTree->token.character == "return" )
        {
            if(abstractSyntaxTree->token.character == "return"){
            cout << "ITS HERE I FOUND RETURN" << endl;
            LCRS *temp = abstractSyntaxTree;
            while (temp)
            {
                workingStack.Push(temp);
                temp = temp->rightSibling;
            }
            cout << "Going into math " << workingStack.head->astNode->token.character << endl;
            doMath(workingStack, scope);
            }
            else{
                LCRS *temp = abstractSyntaxTree;
                temp = temp->rightSibling;
                while (temp)
                {
                    workingStack.Push(temp);
                    temp = temp->rightSibling;
                }
                cout << "Going into math " << workingStack.head->astNode->token.character << endl;
                doMath(workingStack, scope);
            }
        }
        if(abstractSyntaxTree->token.character == "return"){
            return;
        }
        else if(abstractSyntaxTree->rightSibling == nullptr)
        {
            abstractSyntaxTree = abstractSyntaxTree->leftChild;
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
    //cout << scope << endl;
    
    // this doesn't work. But math wise, it is supposed to iterate through the working stack and add the
    // value of each variable to number stack. When it finds an operator, it will perform that specific
    // operation to the 2 most recent numbers in the numberStack. However we have no way of storing
    // values for the variables.
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
                maths.push_back(variables.at(varz).value);
                if(firstVar){
                    returnVar = varz;
                    firstVar = false;
                }

                
                cout << "FOUND VARIABLE: "  << variables.at(varz).value_name << endl;
                //workingStack.Pop();
                break;
            }
        }
        for(int funcVarz = 0; funcVarz < functionVariables.size(); funcVarz++){
            if(functionVariables.at(funcVarz).value_name == workingStack.Top()->astNode->token.character){
                cout << "FOUND FUNCTION VARIABLE " << functionVariables.at(funcVarz).value_name << endl;
                int paramIndex = 0;
                int inputIndex = 0;
                for(int varz = 0; varz < variables.size(); varz ++){
                    if(((cstByAst.at(functionVariables.at(funcVarz).head)->rightSibling->rightSibling->rightSibling->rightSibling->rightSibling->token.character) 
                    == variables.at(varz).value_name) 
                    && variables.at(varz).scope == functionVariables.at(funcVarz).scope){
                        cout << "TEST 1: " << variables.at(varz).value << endl;
                        paramIndex = varz;
                    }
                    else if(((cstByAst.at(functionVariables.at(funcVarz).head)->rightSibling->rightSibling->rightSibling->rightSibling->rightSibling->token.character) 
                    == variables.at(varz).value_name) 
                    && variables.at(varz).scope != functionVariables.at(funcVarz).scope){
                        inputIndex = varz;
                    }
                }
                variables.at(paramIndex).value = variables.at(inputIndex).value;
                cout << "TEST 2: " << variables.at(paramIndex).value << endl;
                executeMain(functionVariables.at(funcVarz).head, functionVariables.at(funcVarz).scope);
                maths.push_back(functionVariables.at(funcVarz).value);
                if(firstFuncVar){
                    returnFuncVar = funcVarz;
                    firstFuncVar = false;
                }
            }

        }
        if(isdigit(workingStack.Top()->astNode->token.character[0]))
        {
            cout << "FOUND DIGIT: " << stoi(workingStack.Top()->astNode->token.character) << endl;
            maths.push_back(stoi(workingStack.Top()->astNode->token.character));
            workingStack.Pop();
        }
        else if (workingStack.Top()->astNode->token.character == "=")
        {
            cout << "FOUND EQUAL: " << workingStack.Top()->astNode->token.character << endl;
            maths.at(0) = maths.at(1);
            maths.pop_back();
            variables.at(returnVar).value = maths.at(0);
            workingStack.Pop();
        }
        else if (workingStack.Top()->astNode->token.character == "*")
        {
            cout << "FOUND ASTERISK: " << workingStack.Top()->astNode->token.character << endl;
            maths.at(maths.size()-2) = maths.at(maths.size()-2) * maths.at(maths.size()-1);
            maths.pop_back();
            // variables.at(returnVar).value = maths.at(0);
            workingStack.Pop();
        }
        else if (workingStack.Top()->astNode->token.character == "/")
        {
            cout << "FOUND DIVIDE: " << workingStack.Top()->astNode->token.character << endl;
            maths.at(maths.size()-2) = maths.at(maths.size()-2) / maths.at(maths.size()-1);
            maths.pop_back();
            // variables.at(returnVar).value = maths.at(0);
            workingStack.Pop();
        }
        else if (workingStack.Top()->astNode->token.character == "+")
        {
            cout << "FOUND PLUS: " << workingStack.Top()->astNode->token.character << endl;
            maths.at(maths.size()-2) = maths.at(maths.size()-2) + maths.at(maths.size()-1);
            maths.pop_back();
            // variables.at(returnVar).value = maths.at(0);
            workingStack.Pop();
        }
        else if (workingStack.Top()->astNode->token.character == "-")
        {
            cout << "FOUND MINUS: " << workingStack.Top()->astNode->token.character << endl;
            maths.at(maths.size()-2) = maths.at(maths.size()-2) - maths.at(maths.size()-1);
            maths.pop_back();
            // variables.at(returnVar).value = maths.at(0);
            workingStack.Pop();
        }
        else if(workingStack.Top()->astNode->token.character == "return"){
            cout << "FOUND RETURN" << endl;
            for(auto variable: variables){
                if((variable.value_name == workingStack.Top()->next->astNode->token.character) && (variable.scope = functionVariables.at(returnFuncVar).scope)){
                    functionVariables.at(returnFuncVar).value = variable.value;
                    return;
                }

            }
        }
        else{
            //Pops off the top of the stack if a variable was found.
            workingStack.Pop();
        }
    cout << variables.at(returnVar).value_name << ": " << variables.at(returnVar).value << endl;
    }
    
    // for (auto [entry, astNode] : astBySymbolTable)
    // {
    //     if ((entry.identifierName == workingStack.Top()->astNode->token.character) && (entry.scope == scope))
    //     {
    //         // numberStack.Push(stoi(entry.));
    //     }
    // }

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
