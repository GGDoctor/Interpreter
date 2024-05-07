#include "interpreter.hpp"
using namespace std;


//Constructor for our interpreter class
Interpreter::Interpreter(AbstractSyntaxTree ast, RecursiveDescentParser cst, SymbolTable symbolTable)
{
    
    //assigns the ast and cst to the corresponding members of interpreter class
    this->ast = ast.getAbstractSyntaxTree(); 
    this->cst = cst.getConcreteSyntaxTree();

    //Creating pointers and intilializing them with cst & ast
    LCRS *abstract = this->ast;
    LCRS *concrete = this->cst;


    //calling the functions
    populateMappings(symbolTable.table, abstract, concrete);
    iterateMaps(astBySymbolTable, cstBySymbolTable, cstByAst);
}


/**
 * @param symbolTable - an stl linked list representing our symbol table
 * @param _ast - the abstract syntax tree
 * @param _cst - the concrete syntax tree
 * @brief takes a symbol table entry as input and adds it to ast and cst maps
 */

//This function is responsible for populating mappings betweeen symbol table
//entries and nodes in AST and CST.
//It iterates through AST and CST simultaneously
void Interpreter::populateMappings(list<TableEntry> symbolTable, LCRS *astNode, LCRS *cstNode)
{
    //declaring pointers so that we can manipulate then
    LCRS *_ast = astNode;
    LCRS *_cst = cstNode;
    LCRS *abstract = astNode;
    LCRS *concrete = cstNode;

    // used for multiple variables declared on same line seperated by commas
    int commaCounter = 0;
    bool dontWorryAboutCommas = false;

    //Our main while loop to iterate through the AST and CST node
    while (abstract && concrete)
    {
        //map AST to to CST node
        cstByAst[abstract] = concrete;

        LCRS *tempConcrete = concrete;

        //loop through the Concrete Syntax tree nodes
        while (concrete->rightSibling)
        {
            //checking for commas and updating the comma counter
            if (concrete->token.character == "," && tempConcrete->token.character != "printf")
                commaCounter++;
            concrete = concrete->rightSibling;
        }

        //Handling comma token
        if (commaCounter > 0)
        {
            for (int i = 0; i < commaCounter; i++)
            {
                //move to the left child of abstract node
                while (abstract->rightSibling)
                    abstract = abstract->rightSibling;
                abstract = abstract->leftChild;
                cstByAst[abstract] = tempConcrete;
            }
        }
        //Handling "for" token
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

        //reset the comma counter
        commaCounter = 0;
    }

    //looping through symbol table
    for (const auto &entry : symbolTable)
    {
        Variable newVar;
        FunctionVariable newFuncVar;
        //handle function entries
        if (entry.identifierType == "function")
        {
            //creating a new function variable and setting it's attributes
            newFuncVar.scope = entry.scope;
            newFuncVar.value_name = entry.identifierName;
            newFuncVar.value = 0;
            newFuncVar.head = _ast;
            //Push the variable to the function variables vector
            functionVariables.push_back(newFuncVar);
            //grabs the parameter of the function and adds it to the variables vector
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
        //handle procedure
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
        //handle a datatype
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


//This function prints the mappings between symbol table entries and their corresponding
//AST and CST nodes. It iterates through the symbol table entries and AST nodes and prints
//all the details such as identifies name, etc. For each entry it prints the AST node an 
//CST node details
void Interpreter::printAstCstBySymbolTable()
{
    //Iterate through the symbol table entries and the AST node
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

        //traverse AST node and print characters
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

        //traverse CST node and printing their characters
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



// printCstByAst prints the mappings between AST nodes and their corresponding CST nodes.
// It iterates through the mappings between AST and CST nodes, and prints the AST node details
// followed by the corresponding CST node details.
void Interpreter::printCstByAst()
{
    //iterate through the mappings between AST and CST nodes
    for (auto &[astNode, cstNode] : cstByAst)
    {
        //initializing pointers to AST and CST nodes
        LCRS *_ast = astNode;
        LCRS *_cst = cstNode;

        //print ast node details
        cout << "ast line " << _ast->token.lineNumber << ": ";

        //traversing the AST nodes and printing characters
        while (_ast)
        {
            cout << _ast->token.character;
            if (_ast->rightSibling)
            {
                cout << " -> ";
            }
            _ast = _ast->rightSibling;
        }

        //print CST node details
        cout << "\ncst line " << _cst->token.lineNumber << ": ";

        //Traverse the CST nodes and print characters
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

//iterateMaps iterates through the maps and prints out the variable list.
//It also executes the "main" function if it's found in astSym
void Interpreter::iterateMaps(unordered_map<TableEntry, LCRS *, TableEntryHash> astSym, unordered_map<TableEntry, LCRS *, TableEntryHash> cstSym, unordered_map<LCRS *, LCRS *> cstAst)
{
    //printing out the variable list
    cout << "Variable List" << endl;
    for(auto vars: variables){
        cout << vars.value_name << endl;
    }
    
    //iterate through astSyn map
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


//Execute the main function, traverses the AST to execute each line of code within main 
void Interpreter::executeMain(LCRS *abstractSyntaxTree, int scope)
{
    // iterate throught the syntax tree in order to start executing each line inside of main
    while (abstractSyntaxTree)
    {
        ProcessingStack workingStack;
        // find the assignment type and does whatever math it needs to do to reassign each value.
        
        if (abstractSyntaxTree->token.character == "Assignment" || abstractSyntaxTree->token.character == "return" )
        {
            //executes almost the exact same thing, except it doesn't skip the first token like in "Assignment"
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
        //Returns when a return statement is read, so it doesn't keep reading the rest of the file
        if(abstractSyntaxTree->token.character == "return"){
            return;
        }
        else if(abstractSyntaxTree->rightSibling == nullptr)
        {
            abstractSyntaxTree = abstractSyntaxTree->leftChild;
        }

        /* 
        *  Started working on the printf
        *  Once it finds printf it takes the next sibling and saves the print statement
        *  Need to go though the print statemnt and remove the %d  
        *  Replace with the values that are given
        *
        */
        else if(abstractSyntaxTree->token.character == "printf")
        {
            // cout <<"I am in the print statements" << endl;
            
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
                    cout << abstractSyntaxTree->rightSibling->token.character << endl;
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


//Performs mathematical operations based on the AST nodes
//Updates variable values and handles function calls
void Interpreter::doMath(ProcessingStack workingStack, int scope)
{
    //vector to store values of operands and results of operations
    vector<int> maths;
    //index of the variable (this will be changed)
    int returnVar = 0;
    //Index of the function variable
    int returnFuncVar = 0;

    bool firstVar = true;
    bool firstFuncVar = true;
    
    //Looping until the stack is empty
    while(workingStack.Top()){
        //iterating through variables
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

                
                cout << "FOUND VARIABLE: "  << variables.at(varz).value_name << endl;
                //workingStack.Pop();
                break;
            }
        }
        for(int funcVarz = 0; funcVarz < functionVariables.size(); funcVarz++){
            //searches for a function variable
            if(functionVariables.at(funcVarz).value_name == workingStack.Top()->astNode->token.character){
                cout << "FOUND FUNCTION VARIABLE " << functionVariables.at(funcVarz).value_name << endl;
                int paramIndex = 0;
                int inputIndex = 0;
                //checks and find the index of the variable of the parameter
                for(int varz = 0; varz < variables.size(); varz ++){
                    if(((cstByAst.at(functionVariables.at(funcVarz).head)->rightSibling->rightSibling->rightSibling->rightSibling->rightSibling->token.character) 
                    == variables.at(varz).value_name) 
                    && variables.at(varz).scope == functionVariables.at(funcVarz).scope){
                        cout << "TEST 1: " << variables.at(varz).value << endl;
                        paramIndex = varz;
                    }
                    //checks and finds the index of the input parameter
                    else if(((cstByAst.at(functionVariables.at(funcVarz).head)->rightSibling->rightSibling->rightSibling->rightSibling->rightSibling->token.character) 
                    == variables.at(varz).value_name) 
                    && variables.at(varz).scope != functionVariables.at(funcVarz).scope){
                        inputIndex = varz;
                    }
                }
                //sets the function parameter value equal to the input parameter value
                variables.at(paramIndex).value = variables.at(inputIndex).value;
                cout << "TEST 2: " << variables.at(paramIndex).value << endl;
                //will execute the function and updates its value
                executeMain(functionVariables.at(funcVarz).head, functionVariables.at(funcVarz).scope);
                // adds the new value to maths vector
                maths.push_back(functionVariables.at(funcVarz).value);
                if(firstFuncVar){
                    returnFuncVar = funcVarz;
                    firstFuncVar = false;
                }
            }

        }
        //checks if the top token character is a digit
        if(isdigit(workingStack.Top()->astNode->token.character[0]))
        {
            cout << "FOUND DIGIT: " << stoi(workingStack.Top()->astNode->token.character) << endl;
            //push the digit to the math vector
            maths.push_back(stoi(workingStack.Top()->astNode->token.character));
            //pop the top from stack
            workingStack.Pop();
        }
        //Handling the assignment operator
        else if (workingStack.Top()->astNode->token.character == "=")
        {
            //sets the first value in maths to the (hypothetically) only other value in maths
            cout << "FOUND EQUAL: " << workingStack.Top()->astNode->token.character << endl;
            maths.at(0) = maths.at(1);
            maths.pop_back();
            variables.at(returnVar).value = maths.at(0);
            workingStack.Pop();
        }
        //Handling the multiplication operator
        else if (workingStack.Top()->astNode->token.character == "*")
        {
            cout << "FOUND ASTERISK: " << workingStack.Top()->astNode->token.character << endl;
            maths.at(maths.size()-2) = maths.at(maths.size()-2) * maths.at(maths.size()-1);
            maths.pop_back();
            workingStack.Pop();
        }
        //Handling the division operator
        else if (workingStack.Top()->astNode->token.character == "/")
        {
            cout << "FOUND DIVIDE: " << workingStack.Top()->astNode->token.character << endl;
            maths.at(maths.size()-2) = maths.at(maths.size()-2) / maths.at(maths.size()-1);
            maths.pop_back();
            workingStack.Pop();
        }
        //Handling the addition operator
        else if (workingStack.Top()->astNode->token.character == "+")
        {
            cout << "FOUND PLUS: " << workingStack.Top()->astNode->token.character << endl;
            maths.at(maths.size()-2) = maths.at(maths.size()-2) + maths.at(maths.size()-1);
            maths.pop_back();
            workingStack.Pop();
        }
        //Handling the subtraction operator
        else if (workingStack.Top()->astNode->token.character == "-")
        {
            cout << "FOUND MINUS: " << workingStack.Top()->astNode->token.character << endl;
            maths.at(maths.size()-2) = maths.at(maths.size()-2) - maths.at(maths.size()-1);
            maths.pop_back();
            workingStack.Pop();
        }

        //Handling the return statement
        else if(workingStack.Top()->astNode->token.character == "return"){
            //updates the function value with the value of the return variable;
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


