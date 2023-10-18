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
	CFileTree(std::string const& initValue)
		: m_root(new TreeNode( initValue, 0, nullptr, {} ))
		, m_currentNode(m_root)
	{}

	void LoadTree(std::istream const& input)
	{
		/*
			root
			.subFolder
			..subsubFolder
			...file.gpt
			..file1.html
			..file2.cpp
			..subsubFolder2
			...aFile.txt
			..file4.tsx
			.subFolder2
		*/
	}

	void SaveTree(std::ostream const& output) const
	{
		// Same, filtered already by uploading order
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

	void DeleteItem()
	{
		// Обход снизу вверх
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

		void RemoveChild(std::string const& childData)
		{
			auto findByData = [&childData](TreeNode* child)
			{
				return child->data == childData;
			};

			std::erase_if(children, findByData);
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

	void PasteAfterCopy()
	{
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
	}

	void PasteAfterCut()
	{
		m_currentNode->children.push_back(m_bufferNode);
		m_bufferNode->parent->RemoveChild(m_bufferNode->data);
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