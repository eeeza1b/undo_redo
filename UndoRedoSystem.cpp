

#include "UndoRedoSystem.h"
#include <iostream>
#include <ctime>

using std::cout;
using std::endl;
using std::to_string;

UndoRedoManager::UndoRedoManager()
{
    actionCounter = 0;
}

string UndoRedoManager::getCurrentTimeStr()
{
    time_t now = time(0);
    tm ltm;
    localtime_s(&ltm, &now);
    char buf[80];
    strftime(buf, sizeof(buf), "%H:%M:%S", &ltm);
    return string(buf);
}

void UndoRedoManager::executeAction(string actionName, bool silent)
{
    actionCounter++;
    string id = "ACT" + to_string(actionCounter);
    string timestamp = getCurrentTimeStr();

    Action act = { id, actionName, timestamp };

    log.clearFuture(log.getCurrent());  
    // xoa sach tat ca cac hanh dong nam phia sau con tro hien tai (vd: A-> B-> C neu undo 2 lan thi se tro lai A 
    // thuc thi hanh dong moi la D. khi lich su se la A va D <-(hien tai) 

    log.addLog(act);  //  them hanh dong moi ngay sau con tro hien tai
    undoStack.push(act); // cho vao stack khi can hanh dong se ly ra dau tien
    actionTypeUndo.push("NORMAL");

    redoStack.clear();
    while (!actionTypeRedo.empty()) actionTypeRedo.pop(); // xoa sach lich su redo hanh dong jump truoc do

    if (!silent)
    {
        cout << "Da thuc hien: [" << id << "] " << actionName << endl;
    }
}

void UndoRedoManager::jumpToComplex(string actionId)
{
    DLLNode* target = log.findById(actionId);   // tim kiem con tro nut trong danh sach lien ket kep dua theo ID truyen vao
    if (target == nullptr)
    {
        cout << "Khong tim thay hanh dong co ID: " << actionId << endl;
        return;
    }

    DLLNode* oldNode = log.getCurrent();  // ghi lai vi tri con tro hien tai truoc khi thuc hien cu nhay

    log.setCurrent(target);
    jumpUndoStack.push({ oldNode, target });
    actionTypeUndo.push("JUMP"); // stack song song danh dau su kien nay thuoc loai jump giup ham undo, redo nhan biet la buoc nhay

    // do co hanh dong nhay moi duoc sinh ra -> huy toan bo nhanh redo phia truoc
    while (!jumpRedoStack.empty()) jumpRedoStack.pop();
    while (!actionTypeRedo.empty()) actionTypeRedo.pop();
    cout << "Da nhay den hanh dong: [" << target->data.id << "] " << target->data.name << endl;
}

void UndoRedoManager::undo()
{
    if (actionTypeUndo.empty())
    {
        cout << "Khong the Undo  -  Khong co hanh dong nao de quay lai" << endl;
        return;
    }

    // kiem tra loai hanh dong nam tren dinh stack undo
    string type = actionTypeUndo.top();
    actionTypeUndo.pop();

    if (type == "JUMP")
    {
        auto nodes = jumpUndoStack.top();
        jumpUndoStack.pop();  // dat con tro ve vi tri cu ban dau

        // dua con tro he thong quay nguoc ve vi tri truoc khi bam nut nhay (vd: tu ACT5 ve lai ACT10)
        log.setCurrent(nodes.first);

        // day thong tin buoc nhay nay sang stack redo
        jumpRedoStack.push(nodes);
        actionTypeRedo.push("JUMP");

        string oldId = (nodes.first != nullptr) ? nodes.first->data.id : "Goc";
        cout << "Da hoan tac (Undo Jump): Tra con tro tro ve [" << oldId << "]" << endl;
    }
    else
    {
        // 2. xu ly hoan tac hanh dong sua doi thong thuong
        Action act = undoStack.pop();
        redoStack.push(act);
        actionTypeRedo.push("NORMAL");

        if (log.getCurrent() != nullptr)
        {
            log.setCurrent(log.getCurrent()->prev);
        }

        cout << "Da hoan tac (Undo): " << act.name << endl;
    }
}

void UndoRedoManager::redo()
{
    if (actionTypeRedo.empty())
    {
        cout << "Khong the Redo  -  Khong co hanh dong nao de lam lai" << endl;
        return;
    }

    // kiem tra loai hanh dong nam tren dinh stack redo
    string type = actionTypeRedo.top();
    actionTypeRedo.pop();

    if (type == "JUMP")
    {
        // 1. xu ly lam lai cho lenh jump
        auto nodes = jumpRedoStack.top();
        jumpRedoStack.pop();

        // ep con tro he thong thuc hien lai cu nhay tien den vi tri dich (vd: tu ACT10 nhay xuong ACT5)
        log.setCurrent(nodes.second);

        // day nguoc lai vao stack undo phong truong hop nguoi dung lai bam tiep undo
        jumpUndoStack.push(nodes);
        actionTypeUndo.push("JUMP");

        cout << "Da lam lai (Redo Jump): Thuc hien lai buoc nhay den [" << nodes.second->data.id << "]" << endl;
    }
    else
    {
        // 2. xu ly lam lai hanh dong sua doi thong thuong
        Action act = redoStack.pop();
        undoStack.push(act);
        actionTypeUndo.push("NORMAL");

        if (log.getCurrent() == nullptr)
        {
            log.setCurrent(log.getHead());
        }
        else
        {
            log.setCurrent(log.getCurrent()->next);
        }

        cout << "Da lam lai (Redo): " << act.name << endl;
    }
}

void UndoRedoManager::showHistory()
{
    log.displayHistory();
}