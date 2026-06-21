

#include "UndoRedoSystem.h"
#include <iostream>

using std::cout;
using std::endl;

HistoryLog::HistoryLog() 
{
    head = nullptr;
    tail = nullptr;
    current = nullptr;
}

HistoryLog::~HistoryLog() 
{
    DLLNode* cur = head;
    while (cur != nullptr) 
    {
        DLLNode* nextNode = cur->next;
        delete cur;
        cur = nextNode;
    }
}

DLLNode* HistoryLog::getCurrent() { return current; }
void HistoryLog::setCurrent(DLLNode* node) { current = node; }
DLLNode* HistoryLog::getHead() { return head; }

void HistoryLog::addLog(Action act) 
{
    DLLNode* newNode = new DLLNode;
    newNode->data = act;
    newNode->next = nullptr;
    newNode->prev = nullptr;

    if (head == nullptr) 
    {
        head = tail = current = newNode;
    }
    else 
    {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
        current = newNode;
    }
    idIndex[act.id] = newNode;
}

void HistoryLog::clearFuture(DLLNode* node) 
{
    DLLNode* toDelete = nullptr;

    if (node == nullptr) 
    {
        toDelete = head;
        head = tail = current = nullptr;
    }
    else 
    {
        toDelete = node->next;
        node->next = nullptr;
        tail = node;
    }

    while (toDelete != nullptr) 
    {
        DLLNode* nextNode = toDelete->next;
        idIndex.erase(toDelete->data.id);
        delete toDelete;
        toDelete = nextNode;
    }
}

void HistoryLog::displayHistory() 
{
    if (head == nullptr) 
    {
        cout << "Nhat ky trong (Khong co hanh dong nao)" << endl;
        return;
    }

    DLLNode* cur = head;
    while (cur != nullptr) 
    {
        cout << "[" << cur->data.id << "] " << cur->data.name << " (" << cur->data.timestamp << ")";
        if (cur == current) 
        {
            cout << " <--- (Hien tai)";
        }
        cout << endl;
        cur = cur->next;
    }
}

DLLNode* HistoryLog::findById(string id) 
{
    auto it = idIndex.find(id);
    if (it != idIndex.end()) 
    {
        return it->second;
    }
    return nullptr;
}