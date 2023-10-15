#include <fstream>
#include <stdexcept>
#include <set>
#include "Stack.h"
#include "task28.h"

/*
    Сафаров Тахир Маратович
    ПС-22
    ISO C++ 20
    Visual Studio 22

    Задание 28
    Программа на ПАСКАЛЕ включает такие  сочетания ключевых
    слов,    как    REPEAT..UNTIL,   IF..THEN..ELSE,   BEGIN..END,
    RECORD..END. Конец оператора  определяется  точкой  с  запятой
    (";").  Требуется  проверить  правильность  вложенности данных
    конструкций с учетом допустимости взаимных вложений. В случае
    ошибок указать номер первой некорректной строки (12).
*/

namespace
{
    using namespace std;

    int currentLine = 1;
    bool isSeparatorFound = false;

    string REPEAT = "REPEAT";
    string IF = "IF";
    string BEGIN = "BEGIN";
    string RECORD = "RECORD";
    string UNTIL = "UNTIL";
    string THEN = "THEN";
    string ELSE = "ELSE";
    string END = "END";

    set<string> START_WORDS = { REPEAT, IF, BEGIN, THEN, RECORD };
    set<string> END_WORDS = { UNTIL, THEN, END };

    string ParseArgs(int argc, char* argv[])
    {
        if (argc != 2)
        {
            throw new invalid_argument("Invalid argument count\nUsage: task28.exe <ProgramFile.txt>");
        }

        return argv[1];
    }

    bool IsLetter(char symbol)
    {
        bool isEngLowerCase = (symbol >= 'a') && (symbol <= 'z');
        bool isEngUpperCase = (symbol >= 'A') && (symbol <= 'Z');

        return isEngLowerCase || isEngUpperCase;
    }

    bool ReadKeyWord(istream& program, string& word)
    {
        isSeparatorFound = false;
        auto isEof = [&program]() { return program.eof(); };
        if (isEof())
        {
            return false;
        }
        char ch = ' ';
        auto getCh = [&program, &ch]()
        {
            ch = program.get();
            if (ch == '\n')
            {
                currentLine++;
            }
            if (ch == ';')
            {
                isSeparatorFound = true;
            }
        };
        getCh();
        while (!IsLetter(ch) && !isEof())
        {
            getCh();
        }
        if (isEof())
        {
            return false;
        }
        while (IsLetter(program.peek()) && !isEof())
        {
            word += ch;
            getCh();
        }
        word += ch;

        return true;
    }

    bool isIfThenCollapsePossible(Stack<string>& stack)
    {
        if (stack.GetSize() < 2)
        {
            return false;
        }
        string top = stack.GetTop();
        stack.Pop();
        string beforeTop = stack.GetTop();
        stack.Push(top);
        return (top == THEN) && (beforeTop == IF);
    }

    bool IsPushCorrect(string const& prev, string const& second)
    {
        return 
            (second == REPEAT && (prev == BEGIN || prev == REPEAT || prev == THEN || prev == ELSE))
            || (second == IF && (prev == BEGIN || prev == REPEAT || prev == THEN || prev == ELSE))
            || (second == BEGIN && (prev == BEGIN || prev == REPEAT || prev == THEN || prev == ELSE))
            || (second == RECORD && prev == RECORD)
            || (second == THEN && prev == IF);
    }

    bool IsPopCorrect(Stack<string>& stack, string const& start, string const& end)
    {
        string initialStart = start;
        while (initialStart == THEN)
        {
            if (stack.GetSize() < 2)
            {
                return false;
            }
            stack.Pop();
            if (stack.GetTop() == IF)
            {
                stack.Pop();
                initialStart = stack.GetTop();
            }
            else
            {
                return false;
            }
        }
        return
            (initialStart == REPEAT && end == UNTIL)
            || (initialStart == BEGIN && end == END)
            || (initialStart == RECORD && end == END);
    }
}

bool CheckIsProgramValid(istream& program)
{
    Stack<string> stack;
    string str;

    while (ReadKeyWord(program, str))
    {
        if (START_WORDS.contains(str))
        {
            if (!stack.Empty() && !IsPushCorrect(stack.GetTop(), str))
            {
                return false;
            }
            stack.Push(str);
        }
        else if (END_WORDS.contains(str))
        {
            if (stack.Empty() || !IsPopCorrect(stack, stack.GetTop(), str))
            {
                return false;
            }

            stack.Pop();
        }
        else if (str == ELSE)
        {
            if (stack.GetSize() < 2 || isSeparatorFound)
            {
                return false;
            }
            string operatorBefore = stack.GetTop();
            stack.Pop();
            string operatorBefore2nd = stack.GetTop();
            stack.Pop();
            if (!(operatorBefore == THEN) || !(operatorBefore2nd == IF))
            {
                return false;
            }
        }
        str.clear();
    }

    while (!stack.Empty() && (stack.GetTop() == THEN || stack.GetTop() == IF))
    {
        stack.Pop();
    }

    return stack.Empty();
}

bool CheckIsProgramValid(string programFileName)
{
    ifstream inputFile;
    inputFile.open(programFileName);
    if (!inputFile.is_open())
    {
        throw new exception("Failed to open file");
    }

    bool result = CheckIsProgramValid(inputFile);

    if (inputFile.bad())
    {
        throw new exception("Failed to work with file");
    }

    return result;
}

int main(int argc, char* argv[])
{
    try
    {
        string filePath = ParseArgs(argc, argv);
        if (CheckIsProgramValid(filePath))
        {
            std::cout << "Program is correct";
        }
        else
        {
            std::cout << "Error on line " << to_string(currentLine);
        }
        std::cout << endl;
    }
    catch (const exception& e)
    {
        std::cout << e.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}