#include "stack.hpp"
#include "string"

void Stack::Pop()
{
    //If the head is a null then break the code and tell user to stop that : bad user
    if(head==nullptr)
    {
        cerr<<"Stack is empty and you are breaking the code" << endl;
        exit(EXIT_FAILURE);
    }

    Node* temp = head;
    head = head->next;
    delete temp;    

}

/*
 *   Arguemnt: Whatever we are passing it
 *   returns nothing
 */
void Stack::Push(int new_data)
{
    // Add the new node to the top of the stack
    Node *new_node = new Node(new_data);
    new_node->next = head;
    head = new_node;
    return;
}

int Stack::Top()
{
    // Check if head is null and add the new node if it is
    if (head == nullptr)
    {
        assert(head != nullptr);
        cerr << "Head is null and you are breaking the code!!!" << endl;
        return -1;
    }

    return head->data;
}