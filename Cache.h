#ifndef CACHE_H
#define CACHE_H

#include "main.h"

class ReplacementPolicy
{
protected:
	int count;
	Elem **arr;

public:
	virtual int insert(Elem *e, int idx) = 0; //insert e into arr[idx] if idx != -1 else into the position by replacement policy
	virtual void access(int idx) = 0;		  //idx is index in the cache of the accessed element
	virtual int remove() = 0;
	virtual void print() = 0;

	bool isFull() { return count == MAXSIZE; }
	bool isEmpty() { return count == 0; }
	Elem *getValue(int idx) { return arr[idx]; }
	void replace(int idx, Elem *e)
	{
		delete arr[idx];
		access(idx);
		arr[idx] = e;
	}
};

class SearchEngine
{
public:
	virtual int search(int key) = 0; // -1 if not found
	virtual void insert(int key, Elem *e) = 0;
	virtual void deleteNode(int key) = 0;
	virtual void print(ReplacementPolicy *r) = 0;
};

class FIFO : public ReplacementPolicy
{
public:
	FIFO()
	{
		count = 0;
		arr = new Elem *[MAXSIZE];
	}
	~FIFO() {}
	int getCount()
	{
		return count;
	}
	int insert(Elem *e, int idx)
	{
		if (count == MAXSIZE)
		{
			return 0;
		}
		else
		{
			arr[idx] = e;
			count++;
			return 1;
		}
	}
	void access(int idx) {}
	int remove()
	{ // luon xoa o dau
		if (count == 0)
		{
			return 0;
		}
		else
		{
			int last = count - 1;
			for (int i = 0; i < last; i++)
			{
				arr[i] = arr[i + 1];
			}
			count--;
			return 1;
		}
	}
	void print()
	{
		for (int i = 0; i < count; i++)
			arr[i]->print();
	}
};

class MRU : public ReplacementPolicy
{
public:
	MRU() {}
	~MRU() {}
	int insert(Elem *e, int idx) { return 0; }
	void access(int idx) {}
	int remove() { return 0; }
	void print() {}
};
class LRU : public MRU
{
public:
	int remove() override { return 0; }
};

class LFU : public ReplacementPolicy
{
public:
	LFU() {}
	~LFU() {}
	int insert(Elem *e, int idx) { return 0; }
	void access(int idx) {}
	int remove() { return 0; }
	void print() {}
};

class DBHashing : public SearchEngine
{
public:
	DBHashing(int (*h1)(int), int (*h2)(int), int s) {}
	~DBHashing() {}
	void insert(int key, Elem *e) {}
	void deleteNode(int key) {}
	void print(ReplacementPolicy *q) {}
	int search(int key) { return 0; }
};
//--------------implement avl -node ------------------
class Node
{
public:
	int key; // chinh la addr
	Elem *ele;
	Node *left;
	Node *right;
	int height;
};
int max(int a, int b);

int height(Node *N)
{
	if (N == NULL)
		return 0;
	return N->height;
}

int max(int a, int b)
{
	return (a > b) ? a : b;
}

Node *newNode(Elem *elem)
{
	Node *node = new Node();
	node->key = elem->addr;
	node->ele = elem;
	node->left = NULL;
	node->right = NULL;
	node->height = 1;

	return (node);
}

Node *rightRotate(Node *y)
{
	Node *x = y->left;
	Node *T2 = x->right;

	x->right = y;
	y->left = T2;

	// Update heights
	y->height = max(height(y->left),
					height(y->right)) +
				1;
	x->height = max(height(x->left),
					height(x->right)) +
				1;

	return x;
}

Node *leftRotate(Node *x)
{
	Node *y = x->right;
	Node *T2 = y->left;

	y->left = x;
	x->right = T2;

	// Update heights
	x->height = max(height(x->left),
					height(x->right)) +
				1;
	y->height = max(height(y->left),
					height(y->right)) +
				1;

	return y;
}

// Get Balance
int getBalance(Node *N)
{
	if (N == NULL)
		return 0;
	return height(N->left) -
		   height(N->right);
}

Node *insertNode(Node *node, Elem *ele)
{

	if (node == NULL)
		return (newNode(ele));
	int key = ele->addr;
	if (key < node->key)
		node->left = insertNode(node->left, ele);
	else if (key >= node->key) //neu bang day sang phai
		node->right = insertNode(node->right, ele);
	else
		return node;

	node->height = 1 + max(height(node->left),
						   height(node->right));

	int balance = getBalance(node);

	// Left Left Case
	if (balance > 1 && key < node->left->key)
		return rightRotate(node);

	// Right Right Case
	if (balance < -1 && key > node->right->key)
		return leftRotate(node);

	// Left Right Case
	if (balance > 1 && key > node->left->key)
	{
		node->left = leftRotate(node->left);
		return rightRotate(node);
	}

	// Right Left Case
	if (balance < -1 && key < node->right->key)
	{
		node->right = rightRotate(node->right);
		return leftRotate(node);
	}

	return node;
}

Node *minValueNode(Node *node)
{
	Node *current = node;

	while (current->left != NULL)
		current = current->left;

	return current;
}

Node *deleteN(Node *root, int key)
{

	if (root == NULL)
		return root;

	if (key < root->key)
		root->left = deleteN(root->left, key);

	else if (key > root->key)
		root->right = deleteN(root->right, key);

	else
	{
		//co 1 hoac ko co con
		if ((root->left == NULL) ||
			(root->right == NULL))
		{
			Node *temp = root->left ? root->left : root->right;

			// khong co con
			if (temp == NULL)
			{
				temp = root;
				root = NULL;
			}
			else // co 1 con
				*root = *temp;

			delete temp;
		}
		else
		{ //khi co du 2 con
			Node *temp = minValueNode(root->right);

			root->key = temp->key;

			root->right = deleteN(root->right,
								  temp->key);
		}
	}
	// can bang lai
	if (root == NULL)
		return root;

	root->height = 1 + max(height(root->left),
						   height(root->right));

	int balance = getBalance(root);

	// Left Left Case
	if (balance > 1 &&
		getBalance(root->left) >= 0)
		return rightRotate(root);

	// Left Right Case
	if (balance > 1 &&
		getBalance(root->left) < 0)
	{
		root->left = leftRotate(root->left);
		return rightRotate(root);
	}

	// Right Right Case
	if (balance < -1 &&
		getBalance(root->right) <= 0)
		return leftRotate(root);

	// Right Left Case
	if (balance < -1 &&
		getBalance(root->right) > 0)
	{
		root->right = rightRotate(root->right);
		return leftRotate(root);
	}

	return root;
}

Node *searchNode(Node *root, int key)
{
	// Base Cases
	if (root == NULL || root->key == key)
		return root;

	if (root->key < key)
		return searchNode(root->right, key);

	return searchNode(root->left, key);
}

void preOrderAVL(Node *root)
{
	if (root != NULL)
	{
		root->ele->print();
		preOrderAVL(root->left);
		preOrderAVL(root->right);
	}
}

void inOrderAVL(Node *root)
{
	if (root != NULL)
	{
		inOrderAVL(root->left);
		root->ele->print();
		inOrderAVL(root->right);
	}
}

//-------------- avl -node end------------------
// case 'U': //put
//                 ss >> addr;
//                 ss >> tmp;
//                 c->put(addr,getData(tmp));
//                 break;
//-- put function truyen vao addr->key, getData(tmp)-> i
class AVL : public SearchEngine
{
	Elem **arr;
	int p;
	Node *avlRoot;

public:
	AVL()
	{
		//arr = new Elem *[MAXSIZE];
		p = 0;
		avlRoot = NULL;
	}
	~AVL()
	{
		delete[] arr;
	}
	//change para
	void insert(int key, Elem *e)
	{
		avlRoot = insertNode(avlRoot, e);
	}
	void deleteNode(int key)
	{
		avlRoot = deleteN(avlRoot, key);
	}
	void print(ReplacementPolicy *q)
	{
		//RP

		//INORDER
		inOrderAVL(avlRoot);
		//PREORDER
		preOrderAVL(avlRoot);
	}
	int search(int key)
	{
		Node *temp = searchNode(avlRoot, key);
		if (temp == NULL)
		{
			return -1;
		}
		else
		{
			temp->ele->data;
		}
	}
};

#endif