#include "processingstack.hpp"
#include "RecursiveDescentParser.hpp" // Include RecursiveDescentParser.hpp

//Pop the head off the stack FIFO
void ProcessingStack::Pop()
{
    // If the head is null, indicate that the stack is empty
    if (head == nullptr)
    {
        cerr << "Stack is empty and you are trying to pop" << endl;
        exit(EXIT_FAILURE);
    }
    Processing_Node* temp = head;
    head = head->next;

    if(head == nullptr){
        tail=nullptr;
    }
    /* Screw the memory leak: the user can buy more :) */
    // delete temp;
}

//Push an item on the tail of the stack FIFO
void ProcessingStack::Push(LCRS* new_data)
{
    // Add the new node to the top of the stack
    Processing_Node* new_node = new Processing_Node(new_data);
    //Processing_Node* temp = tail;
    if(tail == nullptr){
        tail = new_node;
        head = tail;
    }
    else{
        tail->next = new_node;
        tail = new_node;
        tail->next = nullptr;
    }
}


/*
//Work in progress
int ProcessingStack::evaluateExpression(const LCRS* node) {
    // Handle evaluating an expression represented by an AST node
    if (!node) {
        cerr << "Invalid AST node" << endl;
        exit(EXIT_FAILURE);
    }

    // Assuming NodeType and related definitions are accessible from RecursiveDescentParser.hpp
    if (node->type == NodeType::Operator) {
        int operand1 = evaluateExpression(node->leftChild);
        int operand2 = evaluateExpression(node->rightChild);

        switch (node->token.character) {
            case '+':
                return operand1 + operand2;
            case '-':
                return operand1 - operand2;
            case '*':
                return operand1 * operand2;
            case '/':
                return operand1 / operand2;
            default:
                cerr << "Unknown operator: " << node->token.character << endl;
                exit(EXIT_FAILURE);
        }
    } else if (node->type == NodeType::Operand) {
        // Assuming operand nodes contain integer values
        return stoi(node->token.character);
    } else {
        cerr << "Invalid AST node type" << endl;
        exit(EXIT_FAILURE);
    }
}
*/
