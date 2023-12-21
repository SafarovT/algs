#include <algorithm>
#include <iostream>
#include <fstream>
#include <optional>
#include <string>
#include <vector>
#include <limits>
#include <stack>

/*
    ПС-22
    Сафаров Тахир Маратович

    ISO C++ 20
    Visual Studio 22

    Лабораторная № 4
    26) Имеется сеть автомобильных дорог. По  некоторым дорогам
    можно  проехать  только  в одном  направлении. Известна  длина 
    каждой дороги, причем она может быть разной в  зависимости  от 
    направления. Один  из  городов  является  столицей.  Требуется
    вывести список длин вторых по минимальности путей из столицы в 
    другие города. Допускается присутствие циклических путей (12)
*/

namespace
{
    using namespace std;
    using Graph = vector<vector<int>>;

    struct Args
    {
        string inputFileName;
        string outputFileName;
    };

    struct VertexDejkstraData
    {
        int vertex;
        int parent;
        int value;
        bool final;
    };

    struct VertexData
    {
        VertexDejkstraData first;
        VertexDejkstraData second;
    };

    using VertexesData = vector<VertexData>;

    const int MAX_PATH = std::numeric_limits<int>::max();
    size_t vertexesCount = 0;

    optional<Args> ParseArgs(const int argc, char* argv[])
    {
        if (argc != 3)
        {
            cout << "Invalid arguments" << endl
                << "Usage: <program.exe> <inputFile.txt> <outputFile.txt>" << endl;
            return nullopt;
        }
        Args args;
        args.inputFileName = argv[1];
        args.outputFileName = argv[2];
        return args;
    }

    Graph ParseGraph(ifstream& input)
    {
        Graph graph;
        input >> vertexesCount;

        graph.reserve(vertexesCount);
        for (int i = 0; i < vertexesCount; i++)
        {
            graph.emplace_back();
            graph[i].reserve(vertexesCount);
            for (int j = 0; j < vertexesCount; j++)
            {
                graph[i].emplace_back(0);
            }
        }

        size_t edgesCount;
        input >> edgesCount;
        
        int a;
        int b;
        int w;
        for (size_t i = 0; i < edgesCount; i++)
        {
            input >> a;
            input >> b;
            input >> w;
            graph[a - 1][b - 1] = w;
        }

        return graph;
    }

    bool CheckFinals(VertexesData& data)
    {
        for (auto& dataItem : data)
        {
            if (!dataItem.first.final || !dataItem.second.final)
            {
                return false;
            }
        }
        return true;
    }

    VertexesData GetDefaultTableRow()
    {
        VertexesData tableRow;
        tableRow.reserve(vertexesCount);
        tableRow.emplace_back(VertexData({
            {0, 0, 0, true},
            {0, 0, MAX_PATH, false},
            }));
        for (int i = 1; i < vertexesCount; i++)
        {
            tableRow.emplace_back(VertexData({
                {i, 0, MAX_PATH, false},
                {i, 0, MAX_PATH, false},
                }));
        }
        return tableRow;
    }

    void FindNextVertexIndex(size_t& currentVertexIndex, bool& isLookingSecond, bool& isLookingEnded, VertexesData& tableRow)
    {
        VertexDejkstraData defaultMin = { 0, 0, MAX_PATH, false };
        VertexDejkstraData* minPtr = &defaultMin;
        for (auto& data : tableRow)
        {
            if (!data.first.final && data.first.value < minPtr->value)
            {
                minPtr = &data.first;
                isLookingSecond = false;
            }
            if (!data.second.final && data.second.value < minPtr->value)
            {
                minPtr = &data.second;
                isLookingSecond = true;
            }
        }

        if (minPtr->value != MAX_PATH)
        {
            minPtr->final = true;
            currentVertexIndex = minPtr->vertex;
        }
        else
        {
            isLookingEnded = true;
        }
    }

    int GetFullEdgeValue(bool isLookingSecond, int value, VertexDejkstraData& first, VertexDejkstraData& second)
    {
        if (!isLookingSecond)
            return first.value + value;
        else
            return second.value + value;
    }

    void ComparePaths(VertexData* dist, int currentVertexIndex, int fullValue)
    {
        if (!dist->first.final && fullValue < dist->first.value)
        {
            dist->second = dist->first;
            dist->first.value = fullValue;
            dist->first.parent = currentVertexIndex;
        }
        else if (!dist->second.final && fullValue < dist->second.value)
        {
            dist->second.value = fullValue;
            dist->second.parent = currentVertexIndex;
        }
    }

    void FindPaths(Graph& graph, VertexesData& tableRow)
    {
        size_t currentVertexIndex = 0;
        bool isLookingSecond = false;
        bool isLookingEnded = false;
        while (!CheckFinals(tableRow) && !isLookingEnded)
        {
            for (int i = 0; i < vertexesCount; i++)
            {
                VertexData* dist = &tableRow[i];
                VertexData* current = &tableRow[currentVertexIndex];
                int value = graph[currentVertexIndex][i];
                if (value == 0)
                    continue;

                int fullValue = GetFullEdgeValue(isLookingSecond, value, current->first, current->second);
                ComparePaths(dist, currentVertexIndex, fullValue);
            }

            FindNextVertexIndex(currentVertexIndex, isLookingSecond, isLookingEnded, tableRow);
        }
    }

    void PrintResultDistanations(VertexesData& tableRow, ofstream& output)
    {
        for (int i = 1; i < vertexesCount; i++)
        {
            string value = "Not found";
            if (tableRow[i].first.vertex == 0)
            {
                continue;
            }
            if (tableRow[i].second.value != MAX_PATH)
            {
                value = to_string(tableRow[i].second.value);
            }
            output << "Dist(" << tableRow[i].second.vertex + 1 << ") = " << tableRow[i].second.value << endl;
        }
    }
}

int main(int argc, char* argv[])
{
    auto args = ParseArgs(argc, argv);
    if (!args)
    {
        return EXIT_FAILURE;
    }

    ifstream input(args->inputFileName);
    ofstream output(args->outputFileName);

    if (!input.is_open() || !output.is_open())
    {
        return EXIT_FAILURE;
    }

    Graph graph = ParseGraph(input);
    VertexesData tableRow = GetDefaultTableRow();

    FindPaths(graph, tableRow);
    PrintResultDistanations(tableRow, output);

    if (!output.flush())
    {
        cout << "Work with out file failed" << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
