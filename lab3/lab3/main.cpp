#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <optional>
#include "CFileTree.h"

#include <stdio.h>
#include <conio.h>

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

    const std::string HELP_INFO = "Exit - [esc] | Open - [enter] | Back - [backspase] | Create - N | Delete - [delete] | Rename - R | Copy - C | Cut - X | Paste - V";
    const std::string TAB = "    ";

    enum class CommandsCodes : int
    {
        Exit = 27,
        Open = 13,
        Back = 8,
        Create = 110,
        Delete = 83, // TODO: узнать
        Rename = 114,
        Copy = 99,
        Cut = 120,
        Paste = 118,
        Up = 72,
        Down = 80,
    };

    size_t selectedItemIndex = 0;

    struct Args
    {
        std::string inputFileName;
        std::string outputFileName;
    };

    string Implode(std::vector<std::string> const& vector, char separator)
    {
        string result;
        for (auto word = vector.rbegin(); word != vector.rend(); word++)
        {
            result = result + *word + separator;
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
        std::vector<std::string> allParentsFolders = tree.GetAllParents();
        std::string path = Implode(allParentsFolders, '/');
        std::vector<std::string> children = tree.GetChildren();
        cout << HELP_INFO << endl << "--------------------------------------------------------------------------------------------------------------------------------------------" << endl
            << "path: " << path << endl << endl;
        for (size_t i = 0; i < children.size(); i++)
        {
            if (i == selectedItemIndex)
            {
                cout << " -> ";
            }
            else
            {
                cout << TAB;
            }
            cout << children[i] << endl;
        }
    }

    void HandleCommand(CFileTree& tree, bool& isWorking)
    {
        CommandsCodes pressedKey = static_cast<CommandsCodes>(_getch());
        std::string selectedChildName = tree.GetChildren()[selectedItemIndex];
        switch (pressedKey)
        {
        case CommandsCodes::Exit:
            isWorking = false;
            break;
        case CommandsCodes::Open:
        {
            tree.GoDown(selectedChildName);
            break;
        }
        case CommandsCodes::Back:
            tree.GoUp();
            break;
        case CommandsCodes::Create:
        {
            std::string name;
            cout << "Now enter file name and press Enter" << endl << "Caution! You will not see file name until it created";
            auto ch = _getch();
            while (ch != 13)
            {
                name += ch;
                ch = _getch();
            }
            tree.AddChild(name);
            break;
        }
        case CommandsCodes::Delete:
            break;
        case CommandsCodes::Rename:
            break;
        case CommandsCodes::Copy:
            cout << "copy";
            break;
        case CommandsCodes::Cut:
            break;
        case CommandsCodes::Paste:
            break;
        case CommandsCodes::Up:
            if (selectedItemIndex > 0) selectedItemIndex--;
            break;
        case CommandsCodes::Down:
            selectedItemIndex++;
            break;
        default:
            break;
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
        HandleCommand(tree, isWorking);
        /*std::string str;
        while (true)
        {
            printf("Press any key\n");
            while (!_kbhit());
            auto ch = _getch();
            printf("\n Key kod - %d\n", ch);
            str.push_back(ch);
            cout << str << endl;
        }*/
    }

    tree.SaveTree(output);

    if (input.bad() || !output.flush())
    {
        cout << "Failed to work with file" << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}