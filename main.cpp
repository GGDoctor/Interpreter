/**
 * @file main.cpp
 * @brief Main function for Project 3 - Recursive Descent Parser
 * @authors Jacob Franco, Zach Gassner, Haley Joerger, Adam Lyday 
 */

#include "IgnoreComments.hpp"
#include "Tokenization.hpp"
#include "RecursiveDescentParser.hpp"
#include "symboltable.hpp"
#include "abstractsyntaxtree.hpp"
#include "interpreter.hpp"
#include "infixtopostfix.cpp"
#include <iostream>
#include <sstream>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 2 ) {
        cerr << "Error! There is not a correct number of command line arguments.\n";
        return 1;
    }

    string fileName = argv[1];

    IgnoreComments ignoreComments(fileName);
    //cout << ignoreComments << '\n'; 

    // converting the output stream to a string for Tokenization constructor
    stringstream ss;
    ss << ignoreComments;
    Tokenization tokenization(ss.str());
    vector<Token> tokens = tokenization.getTokens();
    //cout << tokenization << '\n';
        
    RecursiveDescentParser recursiveDescentParser(tokens);
    //cout << recursiveDescentParser;

    SymbolTable symbolTable(recursiveDescentParser);
    //cout << symbolTable;

    AbstractSyntaxTree abstractSyntaxTree(recursiveDescentParser, symbolTable);
    //cout << abstractSyntaxTree;
    
    Interpreter interpreter(abstractSyntaxTree, recursiveDescentParser, symbolTable);
    interpreter.printCstByAst();
    cout << "\n\n\n\n";
    interpreter.printAstCstBySymbolTable();

    return 0;
}