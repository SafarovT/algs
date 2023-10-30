#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <stack>

class CFileTree
{
public:
	CFileTree(std::istream& input)
		: m_root(nullptr)
		, m_currentNode(m_root)
	{
		LoadTree(input);
	}

	void LoadTree(std::istream& input)
	{
		std::string line;
		if (!std::getline(input, line))
		{
			throw std::invalid_argument("No root found in file");
		}
		m_root = new TreeNode(line, 0, nullptr, {});
		m_currentNode = m_root;
		TreeNode* tempNode = m_currentNode;
		while (std::getline(input, line))
		{
			int level = 0;
			while (line[level] == '.')
			{
				level++;
			}
			line = line.substr(level, line.length());
			if (level > tempNode->level)
			{
				if (level > tempNode->level + 1)
				{
					tempNode = tempNode->children[tempNode->children.size() - 1];
				}
				tempNode->AddChild(line);
			}
			else if (level < tempNode->level)
			{
				while (tempNode->level >= level)
				{
					tempNode = tempNode->parent;
				}
				tempNode->AddChild(line);
			}
			else if (level == tempNode->level)
			{
				tempNode = tempNode->parent;
				tempNode->AddChild(line);
			}
		}
	}

	void SaveTree(std::ostream& output) const
	{
		std::stack<TreeNode*> nodeStack;
		nodeStack.push(m_root);

		while (!nodeStack.empty())
		{
			TreeNode* tempNode = nodeStack.top();
			nodeStack.pop();
			for (size_t i = 0; i < tempNode->level; i++) output << '.';
			output << tempNode->data << std::endl;
			for (auto& childNode : tempNode->children)
			{
				nodeStack.push(childNode);
			}
		}
	}

	void AddChild(std::string const& data)
	{
		TreeNode* alreadyExistngNode = m_currentNode->FindChild(data);
		if (alreadyExistngNode == nullptr)
		{
			m_currentNode->AddChild(data);
		}
	}

	void GoRoot()
	{
		m_currentNode = m_root;
	}

	void GoUp()
	{
		if (m_currentNode->parent != nullptr)
		{
			m_currentNode = m_currentNode->parent;
		}
	}

	void GoDown(std::string const& itemName)
	{
		TreeNode* nodeToGo = m_currentNode->FindChild(itemName);
		if (nodeToGo != nullptr)
		{
			m_currentNode = nodeToGo;
		}
	}

	void Rename(std::string const& oldName, std::string const& newName)
	{
		TreeNode* itemToRename = m_currentNode->FindChild(oldName);
		if (itemToRename != nullptr)
		{
			itemToRename->data = newName;
		}
	}

	std::vector<std::string> GetAllParents() const
	{
		TreeNode* nodeToNav = m_currentNode;
		std::vector<std::string> result;
		result.push_back(m_currentNode->data);
		while (nodeToNav->parent != nullptr)
		{
			nodeToNav = nodeToNav->parent;
			result.push_back(nodeToNav->data);
		}

		return result;
	}

	std::vector<std::string> GetChildren() const
	{
		std::vector<std::string> result;
		for (TreeNode*& childNode : m_currentNode->children)
		{
			result.push_back(childNode->data);
		}

		return result;
	}

	size_t GetLevel() const
	{
		return m_currentNode->level;
	}

	void CutItem(std::string const& nameItemToCut)
	{
		TreeNode* itemToCutPtr = m_currentNode->FindChild(nameItemToCut);
		if (itemToCutPtr != nullptr)
		{
			m_bufferNode = itemToCutPtr;
			m_isCutMode = true;
		}
	}

	void CopyItem(std::string const& nameItemToCopy)
	{
		TreeNode* itemToCopyPtr = m_currentNode->FindChild(nameItemToCopy);
		if (itemToCopyPtr != nullptr)
		{
			m_bufferNode = itemToCopyPtr;
			m_isCutMode = false;
		}
	}

	void PasteItem()
	{
		if (m_bufferNode == nullptr)
		{
			return;
		}
		if (m_isCutMode)
		{
			PasteAfterCut();
		}
		else
		{
			PasteAfterCopy();
		}
	}

	void DeleteItem(std::string const& name)
	{
		TreeNode* foundNode = m_currentNode->FindChild(name);
		if (foundNode != nullptr)
		{
			std::erase(m_currentNode->children, foundNode);
			DeleteItem(foundNode);
		}
	}

private:
	class TreeNode
	{
	public:
		TreeNode(std::string const& defData, size_t defLevel, TreeNode* defParent, std::vector<TreeNode*> const& defChildren)
			: data(defData)
			, level(defLevel)
			, parent(defParent)
			, children(defChildren)
		{}

		TreeNode* FindChild(std::string const& childData) const
		{
			for (auto& treeNodePtr : children)
			{
				if (treeNodePtr->data == childData)
				{
					return treeNodePtr;
				}
			}
			return nullptr;
		}

		TreeNode* AddChild(std::string const& name)
		{
			TreeNode* newTreeNode = new TreeNode(name, level + 1, this, {});
			children.push_back(newTreeNode);

			return newTreeNode;
		}

		std::string data;
		size_t level;
		TreeNode* parent;
		std::vector<TreeNode*> children;
	};

	TreeNode* m_root;
	TreeNode* m_currentNode;
	TreeNode* m_bufferNode = nullptr;
	bool m_isCutMode = false;

	void DeleteItem(TreeNode* node)
	{
		for (auto& childNode : node->children)
		{
			DeleteItem(childNode);
		}
		delete node;
	}

	void PasteAfterCopy()
	{
		std::stack<TreeNode*> nodeStack;
		nodeStack.push(m_bufferNode);
		std::string newNodeData = m_bufferNode->data;
		if (m_currentNode->FindChild(newNodeData) != nullptr)
		{
			newNodeData += "_copy";
		}
		TreeNode* tempNode = m_currentNode->AddChild(newNodeData);

		while (!nodeStack.empty())
		{
			TreeNode* nodeToCopyChildren = nodeStack.top();
			nodeStack.pop();
			for (auto& childNode : nodeToCopyChildren->children)
			{
				tempNode->AddChild(childNode->data);
				nodeStack.push(childNode);
			}
		}
	}

	void PasteAfterCut()
	{
		if (m_currentNode->FindChild(m_bufferNode->data) != nullptr)
		{
			m_bufferNode->data += "_new";
		}
		m_currentNode->children.push_back(m_bufferNode);

		std::string bufferNodeData = m_bufferNode->data;
		auto findByData = [&bufferNodeData](TreeNode* child)
		{
			return child->data == bufferNodeData;
		};
		std::erase_if(m_bufferNode->parent->children, findByData);

		m_bufferNode->parent = m_currentNode;
		
		std::stack<TreeNode*> nodeStack;
		nodeStack.push(m_bufferNode);

		while (!nodeStack.empty())
		{
			TreeNode* tempNode = nodeStack.top();
			nodeStack.pop();
			tempNode->level = tempNode->parent->level + 1;
			for (auto& childNode : tempNode->children)
			{
				nodeStack.push(childNode);
			}
		}
		
		m_isCutMode = false;
		m_bufferNode = nullptr;
	}
};