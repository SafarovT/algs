#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <optional>

/*
    ПС-22 Сафаров Тахир Маратович
    ISO C++ 20
    Visual Studio 22

    Задание №25
    Задано  некоторое  слово.  Требуется  составить из букв
    этого  слова  максимальное  количество  других  слов,  которые
    имеются  в  словаре.  Каждая  буква  заданного   слова   может
    использоваться  только  один  раз.  Например,  если в заданном
    слове  имеется 2 буквы 'а', то слова словаря, в которых больше
    двух  букв 'а', не подходят. Каждое найденное слово из словаря
    оценивается  количеством очков, равным  длине слова. Результат
    игры определяется суммой  очков. Первая  строка входного файла
    содержит  заданное  слово. Далее  находятся слова словаря. Все
    слова  состоят   из   строчных  (маленьких)  латинских   букв.
    Количество слов словаря  не  превышает 100. В каждом  слове не
    более 20  букв. Длина  заданного  слова  также  не  больше  20
    символов. В первой строке выводится количество набранных очков.
    Начиная со второй строки, следуют найденные слова. Они  должны
    выводиться  по  одному  в  строке по убыванию количества букв.
    Слова  с  одинаковым  количеством  букв  должны  следовать  по
    алфавиту (9).
*/

namespace
{
    using namespace std;

    using Words = vector<string>;
    using PermutationsData = struct
    {
        size_t score;
        Words words;
    };
    using WordData = unordered_map<char, size_t>;

    optional<string> ParseArgs(int argc, char* argv[])
    {
        if (argc != 2)
        {
            cout << "Invalid argument count" << endl
                << "Usage: task25.exe <DictionaryFile.txt>" << endl;
            return nullopt;
        }

        return argv[1];
    }

    bool CompareStr(string const& left, string const& right)
    {
        if (left.size() != right.size())
        {
            return left.size() > right.size();
        }

        return left < right;
    }

    WordData ReadWordData(string word)
    {
        if (word.empty())
        {
            return WordData();
        }

        if (word.size() == 1)
        {
            return WordData({ {word[0], 1} });
        }

        WordData wordData;
        sort(word.begin(), word.end());
        char symbol = word[0];
        size_t symbolsCount = 1;
        size_t wordLength = word.size();
        for (size_t i = 1; i < wordLength; i++)
        {
            if (symbol != word[i])
            {
                wordData.emplace(symbol, symbolsCount);
                symbolsCount = 1;
                symbol = word[i];
            }
            else
            {
                symbolsCount++;
            }
        }
        wordData.emplace(symbol, symbolsCount);

        return wordData;
    }

    PermutationsData GetPermutations(ifstream& dictionary)
    {
        PermutationsData permutations = { 0, Words() };
        string sourceWord;
        if (!getline(dictionary, sourceWord))
        {
            return permutations;
        }
        WordData sourceWordData = ReadWordData(sourceWord);

        string dictionaryWord;
        while (getline(dictionary, dictionaryWord))
        {
            WordData dictionaryWordData = ReadWordData(dictionaryWord);
            bool isSubWord = true;
            for (auto symbolData : dictionaryWordData)
            {
                auto givenWordSymbolData = sourceWordData.find(symbolData.first);

                bool isSymbolFound = givenWordSymbolData != sourceWordData.end();
                if (!isSymbolFound)
                {
                    isSubWord = false;
                    break;
                }

                bool isSymbolsCountEnough = symbolData.second <= givenWordSymbolData->second;

                if (!isSymbolsCountEnough)
                {
                    isSubWord = false;
                }
            }

            if (isSubWord)
            {
                permutations.words.push_back(dictionaryWord);
                permutations.score += dictionaryWord.length();
            }
        }

        sort(permutations.words.begin(), permutations.words.end(), CompareStr);

        return permutations;
    }

    optional<PermutationsData> GetPermutations(string const& dictionaryPath)
    {
        ifstream dictionaryFile;
        dictionaryFile.open(dictionaryPath);
        if (!dictionaryFile.is_open())
        {
            cout << "Failed to open dictionary file" << endl;

            return nullopt;
        }

        PermutationsData result = GetPermutations(dictionaryFile);

        if (dictionaryFile.bad())
        {
            cout << "Work with dicionary file has failed" << endl;

            return nullopt;
        }
        dictionaryFile.close();

        return result;
    }

    void PrintResult(PermutationsData const& result)
    {
        cout << result.score << endl;
        for (string const& word : result.words)
        {
            cout << word << endl;
        }
    }
}

int main(int argc, char* argv[])
{
    auto dictionaryPath = ParseArgs(argc, argv);
    if (dictionaryPath == nullopt)
    {
        return EXIT_FAILURE;
    }

    auto result = GetPermutations(*dictionaryPath);
    if (result == nullopt)
    {
        return EXIT_FAILURE;
    }
    PrintResult(*result);

    return EXIT_SUCCESS;
}