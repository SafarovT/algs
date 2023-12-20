#pragma once

class BTreeNode
{
public:
	BTreeNode(int _t, bool _leaf);

	void InsertNotFull(int k);
	void SplitChild(int i, BTreeNode* y);
	void Traverse();
	BTreeNode* Search(int k);

	friend class BTree;

private:
	int* keys;
	int t;
	BTreeNode** C;
	int n;
	bool leaf;
};

class BTree
{
public:
	BTree(int _t)
		: t(_t)
	{
		root = NULL;
	}

	void Traverse()
	{
		if (root != NULL)
			root->Traverse();
	}

	BTreeNode* Search(int k)
	{
		return (root == NULL)
			? NULL
			: root->Search(k);
	}

	void Insert(int k);

private:
	BTreeNode* root;
	int t;
};
