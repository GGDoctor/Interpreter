// processingstack.cpp

#include "processingstack.hpp"

void ProcessingStack::Pop()
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

void ProcessingStack::Push(int new_data)
{
    // Add the new node to the top of the stack
    Node *new_node = new Node(new_data);
    new_node->next = head;
    head = new_node;
}


//please help actually getting our tokens linked.
int ProcessingStack::evaluateExpression(const string& expression) {
    istringstream iss(expression);
    stack<int> operands;
    char token;
    int operand1, operand2;

    while (iss >> token) {
        if (isdigit(token)) {
            operands.push(token - '0');
        } else if (token == '+' || token == '-' || token == '*' || token == '/') {
            if (operands.size() < 2) {
                cerr << "Invalid expression: not enough operands for operator\n";
                exit(EXIT_FAILURE);
            }
            operand2 = operands.top();
            operands.pop();
            operand1 = operands.top();
            operands.pop();

            switch (token) {
                case '+':
                    operands.push(operand1 + operand2);
                    break;
                case '-':
                    operands.push(operand1 - operand2);
                    break;
                case '*':
                    operands.push(operand1 * operand2);
                    break;
                case '/':
                    operands.push(operand1 / operand2);
                    break;
            }
        } else {
            cerr << "Invalid character in expression: " << token << endl;
            exit(EXIT_FAILURE);
        }
    }

    if (operands.size() != 1) {
        cerr << "Invalid expression: too many operands or operators\n";
        exit(EXIT_FAILURE);
    }

    return operands.top();
}
