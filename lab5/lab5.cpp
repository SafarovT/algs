#include <iostream>
#include <fstream>
#include <optional>
#include <string>
#include "BTree.h"

/*
    ПС-22
    Сафаров Тахир Маратович

    ISO C++ 20
    Visual Studio 22

    Лабораторная № 5
    22) Составить   программу  поиска  записи  с  включением  в 
    сильно   ветвящемся   Б-дереве   порядка  N.  Б-дерево  должно 
    храниться и обрабатываться в файле с прямым доступом (12).

    Temporary solution:

    INPUT FILE TEMPLATE:
    <N>
    <X0> <x1> ...
*/

namespace
{
    using namespace std;

    struct Args
    {
        string treeFileName;
    };

    unsigned N = 0;


    optional<Args> ParseArgs(const int argc, char* argv[])
    {
        if (argc != 2)
        {
            cout << "Invalid arguments" << endl
                << "Usage: <program.exe> <outputFile.txt>" << endl;
            return nullopt;
        }
        Args args;
        args.treeFileName = argv[1];
        return args;
    }
}

int main(int argc, char* argv[])
{
    BTree t(3); // A B-Tree with minimum degree 3
    t.Insert(10);
    t.Insert(20);
    t.Insert(5);
    t.Insert(6);
    t.Insert(12);
    t.Insert(30);
    t.Insert(7);
    t.Insert(17);

    cout << "Traversal of the constructed tree is ";
    t.Traverse();

    int k = 6;
    (t.Search(k) != NULL) ? cout << "\nPresent" : cout << "\nNot Present";

    k = 15;
    (t.Search(k) != NULL) ? cout << "\nPresent" : cout << "\nNot Present";
    /*auto args = ParseArgs(argc, argv);
    if (!args)
    {
        return EXIT_FAILURE;
    }

    fstream treeFile(args->treeFileName);

    if (!treeFile.is_open())
    {
        return EXIT_FAILURE;
    }*/

    return EXIT_SUCCESS;
}