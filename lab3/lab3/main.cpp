#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <optional>
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

    const std::string HELP_INFO = "Exit - [esc] | Open - [enter] | Up - [backspase] | Create - N | Delete - [delete] | Rename - R | Delete - D | Copy - C | Cut - X | Paste - V";
    const std::string TAB = "    ";

    size_t selectedItemIndex = 0;

    struct Args
    {
        std::string inputFileName;
        std::string outputFileName;
    };

    string Implode(std::vector<std::string> const& vector, char separator)
    {
        string result = vector[0];
        for (auto translation = vector.begin() + 1; translation < vector.end(); translation++)
        {
            result = result + separator + *translation;
        }

        return result;
    }

    optional<Args> ParseArgs(int argc, char* argv[])
    {
        if (argc != 3)
        {
            cout << "Invalid arguments count" << endl
                << "Usage: program.exe <input tree file name> <output tree file name>" << endl;
            return nullopt;
        }
        Args args;
        args.inputFileName = argv[1];
        args.outputFileName = argv[2];
        return args;
    };

    void DisplayData(CFileTree const& tree)
    {
        std::vector<std::string> allParantFolders = tree.GetFilePath();
        std::string path = Implode(allParantFolders, '/');
        std::vector<std::string> children = tree.GetChildren();
        cout << HELP_INFO << endl << "-----------------------------------------------------" << endl
            << "path: " << path << endl;
        for (auto& child : children)
        {
            cout << TAB << "./" << child;
        }
    }
}

int main(int argc, char* argv[])
{
    auto args = ParseArgs(argc, argv);
    if (args == nullopt)
    {
        return EXIT_FAILURE;
    }

    ifstream input;
    input.open(args->inputFileName);
    ofstream output;
    output.open(args->outputFileName);

    if (!input.is_open() || !output.is_open())
    {
        cout << "Failed to open file" << endl;
        return EXIT_FAILURE;
    }

    bool isWorking = true;
    CFileTree tree(input);

    while (isWorking)
    {
        system("CLS");
        DisplayData(tree);
    }

    tree.SaveTree(output);

    if (input.bad() || !output.flush())
    {
        cout << "Failed to work with file" << endl;
        return EXIT_FAILURE;
    }

    cout << "Hello" << flush;
    string test("Bye");
    Sleep(1000);
    system("CLS");
    Sleep(1000);
    cout << test;

    return EXIT_SUCCESS;
}