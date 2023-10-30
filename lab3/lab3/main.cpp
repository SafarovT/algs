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
        Delete = 83,
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

    void DisplayData(CFileTree const& tree, std::string const& additionalInfo)
    {
        system("CLS");
        std::vector<std::string> allParentsFolders = tree.GetAllParents();
        std::string path = Implode(allParentsFolders, '/');
        std::vector<std::string> children = tree.GetChildren();
        if (selectedItemIndex > children.size() - 1) selectedItemIndex = 0;
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
        cout << TAB << additionalInfo << endl;
    }

    void DisplayData(CFileTree const& tree)
    {
        DisplayData(tree, "");
    }

    void HandleCommand(CFileTree& tree, bool& isWorking)
    {
        CommandsCodes pressedKey = static_cast<CommandsCodes>(_getch());
        std::vector<std::string> files = tree.GetChildren();
        size_t filesAmount = files.size();
        std::string selectedChildName;
        if (filesAmount > 0) selectedChildName = files[selectedItemIndex];
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
            DisplayData(tree, "[NEW] _\n  Press enter to submit");

            auto ch = _getch();
            while (ch != 13)
            {
                if (ch == 8)
                {
                    if (name.size() != 0) name.pop_back();
                }
                else name += ch;
                DisplayData(tree, "[NEW] " + name + "\n  Press enter to submit");
                ch = _getch();
            }
            if (name.size() != 0)
            {
                tree.AddChild(name);
            }
            break;
        }
        case CommandsCodes::Delete:
            tree.DeleteItem(selectedChildName);
            break;
        case CommandsCodes::Rename:
        {
            std::string newName;
            DisplayData(tree, "\n  " + selectedChildName + " --[RENAME]-> _");
            auto ch = _getch();
            while (ch != 13)
            {
                if (ch == 8)
                {
                    if (newName.size() != 0) newName.pop_back();
                }
                else newName += ch;
                DisplayData(tree, "\n  " + selectedChildName + " --[RENAME]-> " + newName);
                ch = _getch();
            }
            if (newName.size() != 0)
            {
                tree.Rename(selectedChildName, newName);
            }
            break;
        }
        case CommandsCodes::Copy:
            tree.CopyItem(selectedChildName);
            break;
        case CommandsCodes::Cut:
            tree.CutItem(selectedChildName);
            break;
        case CommandsCodes::Paste:
            tree.PasteItem();
            break;
        case CommandsCodes::Up:
            if (selectedItemIndex > 0) selectedItemIndex--;
            break;
        case CommandsCodes::Down:
            if (selectedItemIndex < filesAmount - 1) selectedItemIndex++;
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
    ofstream output(args->outputFileName);

    if (!input.is_open() || !output.is_open())
    {
        cout << "Failed to open file" << endl;
        return EXIT_FAILURE;
    }

    bool isWorking = true;
    CFileTree tree(input);

    while (isWorking)
    {
        DisplayData(tree);
        HandleCommand(tree, isWorking);
    }
    tree.SaveTree(output);
    output.close();

    if (input.bad() || !output.flush())
    {
        cout << "Failed to work with file" << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}