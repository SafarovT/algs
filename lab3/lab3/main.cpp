#include <iostream>
#include <string>
#include <Windows.h>
#include "CFileTree.h"

/*
    ПС-22
    Сафаров Тахир Маратович

    ISO C++ 20
    Visual Studio 22

    Лабораторная № 3
    26. Информация  о  файлах  на  жестких  дисках   компьютера
    записана  с  помощью  дерева.  Обеспечить выполнение следующих
    операций:
       1) загрузку дерева в память из файла;
       2) обход дерева папок в  режиме  диалога  (раскрытие папок,
          подъем на уровень и т. п.);
       3) корректировку  дерева при создании новых папок и файлов,
          их переименовании, копировании, переносе и удалении.
       4) сохранение дерева в файле (14).
*/

namespace
{
    using namespace std;
}

int main(int argc, char* argv[])
{
    CFileTree tree("root");
    cout << "Hello" << flush;
    string test("Bye");
    Sleep(1000);
    system("CLS");
    Sleep(1000);
    cout << test;

    return 0;
}