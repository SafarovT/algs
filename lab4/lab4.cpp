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

    struct VertexData
    {
        int vertex;
        int parent;
        int value;
        bool final;
    };

    struct VertexesData
    {
        VertexData first;
        VertexData second;
    };

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

        for (int i = 0; i < vertexesCount; i++)
        {
            graph.emplace_back();
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

    Graph graph = ParseGraph(input);;

    return EXIT_SUCCESS;
}
