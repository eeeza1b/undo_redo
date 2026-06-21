

#ifndef UNDOREDOSYSTEM_H
#define UNDOREDOSYSTEM_H

#include <string>
#include <unordered_map>
#include <stack> // thu vien dung de quan ly lich su dich chuyen con tro vi tri

using std::string;
using std::unordered_map;

struct Action
{
    string id;
    string name;
    string timestamp;
};

struct StackNode
{
    Action data;
    StackNode* next;
};

class MyStack
{
private:
    StackNode* top;
public:
    MyStack();
    ~MyStack();
    void push(Action act);
    Action pop();
    Action topElement();
    bool isEmpty();
    int size();
    void clear();
};

struct DLLNode
{
    Action data;
    DLLNode* prev;
    DLLNode* next;
};

class HistoryLog
{
private:
    DLLNode* head;
    DLLNode* tail;
    DLLNode* current;
    unordered_map<string, DLLNode*> idIndex;
public:
    HistoryLog();
    ~HistoryLog();
    void addLog(Action act);
    void clearFuture(DLLNode* node);
    void displayHistory();
    DLLNode* findById(string id);
    DLLNode* getCurrent();
    void setCurrent(DLLNode* node);
    DLLNode* getHead();
};

class UndoRedoManager
{
private:
    MyStack undoStack;
    MyStack redoStack;
    HistoryLog log;
    int actionCounter;
    string getCurrentTimeStr();

    // he thong luu vet lich su cho lenh jump
    // stack luu tru cap vi tri bien doi: <vi tri TRUOC KHI nhay, vi tri SAU KHI nhay>
    std::stack<std::pair<DLLNode*, DLLNode*>> jumpUndoStack;
    std::stack<std::pair<DLLNode*, DLLNode*>> jumpRedoStack;
    std::stack<string> actionTypeUndo;
    std::stack<string> actionTypeRedo;

public:
    UndoRedoManager();
    void executeAction(string actionName, bool silent = false);
    void undo();
    void redo();
    void showHistory();
    void jumpToComplex(string actionId);
};

#endif