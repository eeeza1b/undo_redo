

#include "UndoRedoSystem.h"
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;

void showMenu()
{
    cout << "\n      MO PHONG CHUC NANG UNDO - REDO\n\n";
    cout << "1. Nhap hanh dong moi (Execute Action)\n";
    cout << "2. Quay lai buoc truoc (Undo)\n";
    cout << "3. Lam lai buoc vua xoa (Redo)\n";
    cout << "4. Xem toan bo nhat ky lich su (History)\n";
    cout << "5. Nhay toi mot hanh dong bat ky (Jump)\n";
    cout << "6. Nap du lieu mau file Demo\n";
    cout << "7. Danh gia hieu suat file Test\n";
    cout << "0. Thoat chuong trinh\n\n";
}

ifstream openTestFile(string baseName)
{
    ifstream file(baseName);
    if (file.is_open()) return file;
    string absolutePath = "D:\\VSCC\\project_undo_redo\\project_undo_redo\\" + baseName;
    return ifstream(absolutePath);
}

int main()
{
    UndoRedoManager manager;
    int choice;
    string inputStr;

    while (true)
    {
        showMenu();
        cout << "Chon chuc nang (0-7): ";
        if (!(cin >> choice)) 
        {
            cin.clear();  // loi nhap lieu se vo hieu hoa chuong trinh
            cin.ignore(10000, '\n');  // bo qua nhap sai de den luot nhap moi
            continue;
        }
        cin.ignore(); // xoa bo nho dem sau khi nhap so

        if (choice == 0) 
        {
            cout << "Thoat chuong trinh\n";
            break;
        }

        switch (choice)
        {
        case 1:
        {
            cout << "Nhap ten hanh dong: ";
            getline(cin, inputStr);
            if (!inputStr.empty()) manager.executeAction(inputStr);
            break;
        }
        case 2:
            manager.undo();
            break;
        case 3:
            manager.redo();
            break;
        case 4:
            manager.showHistory();
            break;
        case 5:
        {
            cout << "Nhap ID hanh dong muon nhay toi (vi du: ACT5): ";
            cin >> inputStr;
            manager.jumpToComplex(inputStr);
            break;
        }
        case 6:
        {
            // file path demo data
            string filePath = "D:\\VSCC\\project_undo_redo\\project_undo_redo\\DemoData_Nhom2.txt";
            ifstream file(filePath);

            if (!file.is_open())
            {
                cout << "Khong mo duoc file DemoData_Nhom2.txt" << endl;
            }
            else
            {
                string actionLine;
                int count = 0;
                while (getline(file, actionLine))
                {
                    if (!actionLine.empty())
                    {
                        manager.executeAction(actionLine);
                        count++;
                    }
                }
                file.close();
                cout << "Da nap thanh cong " << count << " hanh dong tu file du lieu\n";
            }
            break;
        }
        case 7:
        {
            cout << "\n       DANH GIA HIEU SUAT NAP FILE DU LIEU       \n";

            string files[] = { "TestData_Nhom2_small.txt", "TestData_Nhom2_medium.txt", "TestData_Nhom2_large.txt" };
            string labels[] = { "SMALL", "MEDIUM", "LARGE" };

            for (int i = 0; i < 3; i++)
            {
                ifstream testFile = openTestFile(files[i]);
                if (!testFile.is_open())
                {
                    cout << "[" << labels[i] << "] Khong tim thay hoac khong mo duoc file: " << files[i] << endl;
                    continue;
                }

                // 1. bat dau bam gio
                auto start = std::chrono::high_resolution_clock::now();

                string actionLine;
                int count = 0;
                while (getline(testFile, actionLine))
                {
                    if (!actionLine.empty())
                    {
                        manager.executeAction(actionLine, true);
                        count++;
                    }
                }
                testFile.close();

                // 2. ket thuc bam gio
                auto end = std::chrono::high_resolution_clock::now();

                // 3. tinh toan khoang thoi gian chenh lech (mili-giay)
                std::chrono::duration<double, std::milli> duration = end - start;

                cout << "-> [" << labels[i] << "] Nap thanh cong: " << count << " hanh dong" << endl;
                cout << "   Thoi gian thuc thi: " << duration.count() << " ms\n";
                cout << "\n";
            }
            break;
        }
        default:
            cout << "Lua chon khong hop le  -  Vui long chon lai\n";
            break;
        }
    }
    return 0;
}