#ifndef PROCESSINGSTACK_HPP
#define PROCESSINGSTACK_HPP

#include <iostream>
#include <stack>
//#include "interpreter.hpp"
#include "RecursiveDescentParser.hpp"

using namespace std;

// Linked list node
struct Processing_Node {
    // Pointer to the AST node
    LCRS* astNode;
    // Pointer to the next node in the stack
    Processing_Node* next;

    // Constructor for the node
    Processing_Node(LCRS* node) : astNode(node), next(nullptr) {}
};

/*
*   This is our stack where we will be handling all the pushing and popping
*   
*/
class ProcessingStack {
public:
    // Initializer
    ProcessingStack() : head(nullptr) {}

    // Push
    void Push(LCRS* new_data);

    // Pop
    void Pop();

    Processing_Node* Top(){
        return head;
    }

    // Evaluate an expression represented as an AST node
    int evaluateExpression(const LCRS* node);

    // Clean up the stack once it is no longer needed. Free the memory
    ~ProcessingStack() {
        while (head != nullptr) {
            Pop();
        }
    }
    friend class Interpreter;
private:
    // Pointer to the head of the stack
    Processing_Node* head;
};

#endif /* PROCESSINGSTACK_HPP */
