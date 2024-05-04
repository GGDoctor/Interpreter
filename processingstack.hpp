#ifndef PROCESSINGSTACK_HPP
#define PROCESSINGSTACK_HPP

#include <iostream>
#include <vector>
#include <list>
#include <cassert>
#include <sstream>
#include <stack>
#include "interpreter.hpp"

using namespace std;

//Linked list node

struct Node {
    //Can add a data variable here need to figure out the type
    int data; 
    //Pointer to the next node in our stack
    Node* next;

    //Constructor for the node 
    Node(int val) : data(val), next(nullptr) {}
};

/*
*   This is our stack where we will be handling all the pushing and popping
*   
*/
class ProcessingStack{

    public:
        //Initialzier
        ProcessingStack() : head(nullptr) {};

        //Push 
        void Push(int new_data);
        
        //Pop 
        //We will need to change the data type to match what we need it to return
        void Pop();

        // Evaluate an expression represented as a string
        int evaluateExpression(const string& expression);

        //Clean up the stack once it is no longer needed. Free the memory
        ~ProcessingStack()
        {
            while(head!=nullptr)
            {
                Pop();
            }
        }

    private:
        
        //Pointer to the head of the stack
        Node* head;
};

#endif /* PROCESSINGSTACK_HPP */
