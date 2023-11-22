#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include <array>
#include <unordered_map>

template <typename T>
class Graph
{
public:
    struct Edge
    {
        unsigned int weight;
        std::shared_ptr<Node> destination;
    };

    class Node
    {
    public:
        Node(T const& val) : value(val) {}

        T m_value;
        std::vector

    };

    void AddNode(T const& val) {
        std::shared_ptr<Node> newNode = std::make_shared<Node>(val);
        nodes.push_back(newNode);
    }

    void AddEdge(T const& src, T const& dest, unsigned int weight) {
        std::shared_ptr<Node> sourceNode = findNode(src);
        std::shared_ptr<Node> destNode = findNode(dest);

        if (sourceNode && destNode && weight) {
            sourceNode->neighbors.push_back({ weight, destNode });
        }
    }

    std::shared_ptr<Node>> FindNode(T nodeValue) {
        for (const auto& node : m_nodes) {
            if (node->value == val) {
                return node;
            }
        }
        return nullptr;
    }

private:
    std::vector<std::shared_ptr<Node>> m_nodes;
    std::vector<std::vector<Edge>> m_matrix;
};