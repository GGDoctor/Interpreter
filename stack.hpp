#ifndef STACK_HPP
#define STACK_HPP

#include <iostream>
#include <vector>
#include <list>
#include <cassert>

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
class Stack{

    public:
        //Initialzier
        Stack() : head(nullptr) {};

        //Push an item onto our theoretical stack
        void Push(int new_data);
        
        //Pop an item off our theoretical stack
        //We will need to change the data type to match what we need it to return
        void Pop();

        //Get the top item off our theoretical stack
        int Top();

        //Clean up the stack once it is no longer needed. Free the memory
        ~Stack()
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


#endif /* STACK_HPP */