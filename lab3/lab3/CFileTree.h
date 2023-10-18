#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>

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
				line = line.substr(level, line.length());
			}
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

	void SaveTree(std::ostream const& output) const
	{
		// Обход дерева сверху вниз
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

	std::vector<std::string> GetFilePath() const
	{
		TreeNode* nodeToNav = m_currentNode;
		std::vector<std::string> result;
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

		void AddChild(std::string const& name)
		{
			if (level == std::numeric_limits<size_t>::infinity())
			{
				return;
			}
			children.push_back
			(
				new TreeNode
				(
					name,
					0,
					nullptr,
					{}
				)
			);
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
		TreeNode* tempNode = m_currentNode;

	}

	void PasteAfterCut()
	{
		// Обход сверху вниз (для изменения уровней)
		// Уровень отсчитывать относительно места вставки
		// Сбросить m_isCutMode и n_bufferNode

		m_currentNode->children.push_back(m_bufferNode);

		std::string bufferNodeData = m_bufferNode->data;
		auto findByData = [&bufferNodeData](TreeNode* child)
		{
			return child->data == bufferNodeData;
		};
		std::erase_if(m_bufferNode->parent->children, findByData);

		m_bufferNode->parent = m_currentNode;
		
		size_t currentLevel = m_currentNode->level;


		m_isCutMode = false;
	}
};