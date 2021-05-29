#ifndef CACHE_H
#define CACHE_H

#include "main.h"

class Node;
class NodeLFU;

class Heap;
int parent(int i)
{
	return (i - 1) / 2;
}
int left(int i)
{
	return 2 * i + 1;
}
int right(int i)
{
	return 2 * i + 2;
}

//--------------------------------------------------heap begin---------------------------------------
class NodeLFU
{
public:
	int countLFU;
	int addr;
	Elem *e;
	NodeLFU(Elem *e)
	{
		this->countLFU = 1;
		this->addr = e->addr;
		this->e = e;
	}
};

class Heap
{
protected:
	NodeLFU **elements;
	int capacity;
	int count;

public:
	Heap(int size)
	{
		this->capacity = size;
		this->count = 0;
		this->elements = new NodeLFU *[capacity];
	}
	~Heap()
	{
		delete[] elements;
	}
	int push(Elem *item);
	void printHeap()
	{
		cout << "Min Heap [ ";
		for (int i = 0; i < count; i++)
			cout << elements[i]->countLFU << "-" << elements[i]->addr << " ";
		cout << "]";
	}
	void printForRP()
	{
		for (int i = 0; i < count; i++)
		{
			this->elements[i]->e->print();
		}
	}
	//coi co item nhu address
	bool isEmpty();
	bool contains(int item);
	int peek();
	bool pop();
	int size();
	//cau4
	Elem *getItem(int item);
	Elem *getItemByIndex(int idx);
	void remove(int item);
	int getItemIndex(int item);
	//void clear();

	//void ensureCapacity(int minCapacity);
	void reheapUp(int position);
	void reheapDown(int position);
	void updateCount(int key);
};

// Your code here

//for update to arr
Elem *Heap::getItemByIndex(int idx)
{
	return elements[idx]->e;
}

//return index
int Heap::push(Elem *item2)
{
	NodeLFU *item = new NodeLFU(item2);
	int index = 0;
	// if (count == capacity)
	// {
	//     ensureCapacity(count + 1);
	//     capacity++;
	// }
	if (count == 0)
	{
		elements[0] = item;
		index = 0;
	}
	else
	{
		index = this->count;
		elements[index] = item;
		reheapUp(index);
	}
	count++;
	return index;
}

// void Heap::ensureCapacity(int minCapacity)
// {
//     int *newheap = new int[minCapacity];
//     for (int i = 0; i < count; i++)
//     {
//         newheap[i] = elements[i];
//     }
//     this->elements = newheap;
// }

void Heap::reheapUp(int position)
{
	int p = (position - 1) / 2;
	if (elements[p]->countLFU > elements[position]->countLFU)
	{
		swap(elements[p], elements[position]);
		reheapUp(p);
	}
}

int Heap::size()
{
	return count;
}

bool Heap::isEmpty()
{
	return count == 0;
}

int Heap::peek()
{
	return this->elements[0]->addr;
}

bool Heap::contains(int item)
{
	for (int i = 0; i < count; i++)
	{
		if (elements[i]->addr == item)
			return true;
	}
	return false;
}

bool Heap::pop()
{
	if (count == 0)
	{
		return false;
	}
	if (count == 1)
	{
		this->count--;
		elements[0] = 0;
	}
	elements[0] = elements[count - 1];
	count--;
	this->reheapDown(0);
	return true;
}

//cau4
// neu bang lay ben trai
void Heap::reheapDown(int position)
{
	int l = left(position);
	int r = right(position);
	int largest = position;

	if (l < count && r < count && elements[l]->countLFU == elements[r]->countLFU && elements[r]->countLFU <= elements[largest]->countLFU)
	{
		largest = l;
	}
	else
	{
		if (l < count && elements[l]->countLFU <= elements[largest]->countLFU)
		{
			largest = l;
		}
		if (r < count && elements[r]->countLFU <= elements[largest]->countLFU)
		{
			largest = r;
		}
	}
	if (largest != position)
	{
		swap(elements[largest], elements[position]);
		reheapDown(largest);
	}
}

//get folow address
Elem *Heap::getItem(int item)
{
	// TODO: return the index of item in heap
	for (int i = 0; i < count; i++)
	{
		if (elements[i]->addr == item)
			return elements[i]->e;
	}
	return NULL;
}

int Heap::getItemIndex(int item)
{
	// TODO: return the index of item in heap
	for (int i = 0; i < count; i++)
	{
		if (elements[i]->addr == item)
			return i;
	}
	return -1;
}

void Heap::remove(int item)
{
	// TODO: remove the element with value equal to item
	int index = getItemIndex(item);
	int last = count - 1;
	if (count == 0)
		return;
	else if (index == last)
	{
		count--;
	}
	else if (index < last)
	{
		swap(elements[index], elements[last]);
		count--;
		reheapDown(index);
	}
}

// void Heap::clear()
// {
//     // TODO: delete all elements in heap
//     delete[] elements;
//     count = 0;
// }

void Heap::updateCount(int key)
{
	int idx = getItemIndex(key);
	elements[idx]->countLFU++;
	this->reheapDown(idx);
}
//------------------end heap-----------------------------------------

class ReplacementPolicy
{
protected:
	int count;
	Elem **arr;

public:
	virtual int insert(Elem *e, int idx) = 0;  //insert e into arr[idx] if idx != -1 else into the position by replacement policy
	virtual void access(int idx, Elem *e) = 0; //idx is index in the cache of the accessed element
	virtual int remove() = 0;
	virtual void print() = 0;

	//moi them
	int getCount()
	{
		return count;
	}
	//rieng cho LFU
	virtual void updateFromHeapToArr() = 0;
	virtual Elem *getValue(int idx) = 0;

	bool isFull() { return count == MAXSIZE; }
	bool isEmpty() { return count == 0; }

	void replace(int idx, Elem *e)
	{
		//access giong nhu 1 ham update lai index cho no vay
		delete arr[idx];
		access(idx, e);
		arr[idx] = e;
	}
};

class SearchEngine
{

public:
	virtual int search(int key) = 0; // -1 if not found
	virtual void insert(int key, int idx) = 0;
	virtual void deleteNode(int key) = 0;
	virtual void print(ReplacementPolicy *r) = 0;
	//moi them
	virtual void InManHinh() = 0;
	virtual void updateIndex(ReplacementPolicy *r) = 0;
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
	int insert(Elem *e, int idx)
	{
		if (count == MAXSIZE)
		{
			return -1;
		}
		else
		{
			arr[idx] = e;
			count++;
			return idx;
		}
	}
	void access(int idx, Elem *e) {}
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
	void updateFromHeapToArr() {}
	Elem *getValue(int idx) { return arr[idx]; }
};

class MRU : public ReplacementPolicy
{
public:
	MRU() {}
	~MRU() {}
	int insert(Elem *e, int idx) { return 0; }
	void access(int idx, Elem *e) {}
	int remove() { return 0; }
	void print() {}
	void updateFromHeapToArr() {}
	Elem *getValue(int idx) { return arr[idx]; }
};
class LRU : public MRU
{
public:
	int remove() override { return 0; }
};

class LFU : public ReplacementPolicy
{
public:
	Heap *minHeap;
	LFU()
	{
		count = 0;
		arr = new Elem *[MAXSIZE];
		minHeap = new Heap(MAXSIZE);
	}
	~LFU() {}
	//cache se insert idx theo return cua insert
	int insert(Elem *e, int idx)
	{
		if (count < MAXSIZE)
		{
			int index = minHeap->push(e);
			count++;
			if (minHeap->contains(e->addr))
			{
				minHeap->updateCount(e->addr);
			}
			updateFromHeapToArr();
			return index;
		}
		else
		{
			return -1;
		}
	}
	void access(int idx, Elem *e)
	{
		//xu ly khi write, moi chi replace arr, ko dong bo cho heap
		Elem *temp = minHeap->getItemByIndex(idx);
		temp = e;
		minHeap->updateCount(temp->addr);
		updateFromHeapToArr();
	}
	//luon xoa node min
	int remove()
	{
		bool isRemove = minHeap->pop();
		if (isRemove)
		{
			return 1;
			updateFromHeapToArr();
		}
		return 0;
	}
	void print()
	{
		minHeap->printForRP();
	}
	//update lien tuc cho minheap-> arr
	void updateFromHeapToArr()
	{
		for (int i = 0; i < count; i++)
		{
			Elem *e = minHeap->getItemByIndex(i);
			arr[i] = e;
		}
	}
	Elem *getValue(int idx)
	{
		//read cung lam thay doi heap
		int key = arr[idx]->addr;
		minHeap->updateCount(key);
		updateFromHeapToArr();
		return arr[idx];
	}
};

class DBHashing : public SearchEngine
{
public:
	DBHashing(int (*h1)(int), int (*h2)(int), int s) {}
	~DBHashing() {}
	//to test
	void updateIndex(ReplacementPolicy *r) {}
	void InManHinh() {}
	void insert(int key, int i) {}
	void deleteNode(int key) {}
	void print(ReplacementPolicy *q) {}
	int search(int key) { return 0; }
};
//--------------implement avl -node ------------------
class Node
{
public:
	int key;   // chinh la addr
	int index; // trong arr trong rp
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

Node *newNode(int addr, int index)
{
	Node *node = new Node();
	node->key = addr;
	node->index = index;
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

Node *insertNode(Node *node, int key, int index)
{

	if (node == NULL)
		return (newNode(key, index));
	//int key = ele->addr;
	if (key < node->key)
		node->left = insertNode(node->left, key, index);
	else if (key >= node->key) //neu bang day sang phai
		node->right = insertNode(node->right, key, index);
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
//for update for print
void setNode(Node *node, int idx_new)
{
	if (node != NULL)
	{
		node->index = idx_new;
	}
}

void updateIndexRecursive(Node *root)
{
	if (root != NULL)
	{
		root->index--;
		updateIndexRecursive(root->left);
		updateIndexRecursive(root->right);
	}
}

void preOrderAVL(Node *root, ReplacementPolicy *q)
{
	if (root != NULL)
	{
		int index = root->index;
		q->getValue(index)->print();
		//root->ele->print();
		preOrderAVL(root->left, q);
		preOrderAVL(root->right, q);
	}
}

void preOrder_test(Node *root)
{
	if (root != NULL)
	{
		cout << "index = " << root->index << " key= " << root->key << endl;
		preOrder_test(root->left);
		preOrder_test(root->right);
	}
}

void inOrderAVL(Node *root, ReplacementPolicy *q)
{
	if (root != NULL)
	{
		inOrderAVL(root->left, q);
		int index = root->index;
		q->getValue(index)->print();
		//root->ele->print();
		inOrderAVL(root->right, q);
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
private:
	int p;

public:
	Node *avlRoot;
	AVL()
	{
		//arr = new Elem *[MAXSIZE];
		p = 0;
		avlRoot = NULL;
	}
	~AVL()
	{
		//delete[] arr;
	}
	void InManHinh()
	{
		preOrder_test(this->avlRoot);
	}

	//change para
	void insert(int key, int idx)
	{
		this->avlRoot = insertNode(avlRoot, key, idx);
		//cout << "\navl ne: o file cache.h nhe dong 424: " << avlRoot->index << endl;
		p++;
	}
	void deleteNode(int key)
	{
		avlRoot = deleteN(avlRoot, key);
		p--;
		//updateIndex(); // sau khi xoa can phai cap nhat index cho dong bo voi arr trong rp
	}
	void print(ReplacementPolicy *q)
	{

		//INORDER
		cout << "Print AVL in inorder:\n";
		inOrderAVL(avlRoot, q);
		//PREORDER
		cout << "In AVL in preorder:\n";
		preOrderAVL(avlRoot, q);
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
			return temp->index;
		}
	}
	void updateIndex(ReplacementPolicy *q)
	{
		int size = q->getCount();
		int key = 0;
		int idx = 0;
		Elem *e;
		for (int i = 0; i < size; i++)
		{
			//elem dang xem xet
			e = q->getValue(i);
			//key de tim elem do trong cache
			key = e->addr;
			//tim node co key do trong cache
			Node *temp = searchNode(avlRoot, key);
			//cap nhat index moi trong node co key do -> dong bo s_engine voi rp
			setNode(temp, i);
		}
	}
};

#endif