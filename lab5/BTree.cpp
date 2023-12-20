#include <iostream>
#include "BTree.h"

BTreeNode::BTreeNode(int _t, bool _leaf)
{
	t = _t;
	leaf = _leaf;

	keys = new int[2 * t - 1];
	C = new BTreeNode * [2 * t];

	n = 0;
}

void BTreeNode::Traverse()
{
	int i;
	for (i = 0; i < n; i++)
	{
		if (leaf == false)
			C[i]->Traverse();
		std::cout << " " << keys[i];
	}

	if (leaf == false)
		C[i]->Traverse();
}

BTreeNode* BTreeNode::Search(int k)
{
	int i = 0;
	while (i < n && k > keys[i])
		i++;
	if (keys[i] == k)
		return this;
	if (leaf == true)
		return NULL;
	return C[i]->Search(k);
}

void BTreeNode::InsertNotFull(int k)
{
	int i = n - 1;
	if (leaf == true)
	{
		while (i >= 0 && keys[i] > k)
		{
			keys[i + 1] = keys[i];
			i--;
		}
		keys[i + 1] = k;
		n = n + 1;
	}
	else
	{
		while (i >= 0 && keys[i] > k)
			i--;
		if (C[i + 1]->n == 2 * t - 1)
		{
			SplitChild(i + 1, C[i + 1]);
			if (keys[i + 1] < k)
				i++;
		}
		C[i + 1]->InsertNotFull(k);
	}
}

void BTreeNode::SplitChild(int i, BTreeNode* y)
{
	BTreeNode* z = new BTreeNode(y->t, y->leaf);
	z->n = t - 1;
	for (int j = 0; j < t - 1; j++)
		z->keys[j] = y->keys[j + 1];
	if (y->leaf == false)
	{
		for (int j = 0; j < t; j++)
			z->C[j] = y->C[j + t];
	}
	y->n = t - 1;
	for (int j = n; j >= i + 1; j--)
		C[j + 1] = C[j];
	C[i + 1] = z;
	for (int j = n - 1; j >= i; j--)
		keys[j + 1] = keys[j];
	keys[i] = y->keys[t - 1];
	n = n + 1;
}

void BTree::Insert(int k)
{
	if (root == NULL)
	{
		root = new BTreeNode(t, true);
		root->keys[0] = k;
		root->n = 1;
	}
	else
	{
		if (root->n == 2 * t - 1)
		{
			BTreeNode* s = new BTreeNode(t, false);
			s->C[0] = root;
			s->SplitChild(0, root);
			int i = 0;
			if (s->keys[0] < k)
				i++;
			s->C[i]->InsertNotFull(k);
			root = s;
		}
		else
		{
			root->InsertNotFull(k);
		}
	}
}
