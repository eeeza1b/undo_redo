

#include "UndoRedoSystem.h"

MyStack::MyStack() 
{
    top = nullptr;
}

MyStack::~MyStack() 
{
    clear();
}

void MyStack::push(Action act) 
{
    StackNode* newNode = new StackNode;
    newNode->data = act;
    newNode->next = top;
    top = newNode;
}

Action MyStack::pop() 
{
    if (isEmpty())
        return { "", "", "" };

    StackNode* temp = top;
    Action act = temp->data;
    top = top->next;
    delete temp;
    return act;
}

Action MyStack::topElement() 
{
    if (isEmpty())
        return { "", "", "" };
    return top->data;
}

bool MyStack::isEmpty() 
{
    return top == nullptr;
}

int MyStack::size() 
{
    int count = 0;
    StackNode* cur = top;
    while (cur != nullptr) 
    {
        count++;
        cur = cur->next;
    }
    return count;
}

void MyStack::clear() 
{
    while (!isEmpty()) 
    {
        pop();
    }
}