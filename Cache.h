#ifndef CACHE_H
#define CACHE_H

#include "main.h"

/**
 * Name: Le Hong Phong
 * id: 1813518
*/

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
	NodeLFU(int addr)
	{
		this->countLFU = 1;
		this->addr = addr;
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
	int push(int addr);
	void printHeap()
	{
		cout << "Min Heap [ ";
		for (int i = 0; i < count; i++)
			cout << elements[i]->countLFU << "-" << elements[i]->addr << " ";
		cout << "]";
	}
	//coi co item nhu address
	bool isEmpty();
	bool contains(int item);
	int peek();
	bool pop();
	int size();
	//cau4
	int getItemByAddr(int address);
	int getItemByIndex(int idx);
	void remove(int item);
	//	int getItemIndex(int item);
	//void clear();

	//void ensureCapacity(int minCapacity);
	void reheapUp(int position);
	void reheapDown(int position);
	void updateCount(int key);
};

// Your code here

//for update to arr ->key , return key
int Heap::getItemByIndex(int idx)
{
	return elements[idx]->addr;
}

//return index
int Heap::push(int addr)
{
	NodeLFU *item = new NodeLFU(addr);
	int index = 0;

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

//get index folow address
int Heap::getItemByAddr(int address)
{
	// TODO: return the index of item in heap
	for (int i = 0; i < count; i++)
	{
		if (elements[i]->addr == address)
			return i;
	}
	return -1;
}

// int Heap::getItemIndex(int item)
// {
// 	// TODO: return the index of item in heap
// 	for (int i = 0; i < count; i++)
// 	{
// 		if (elements[i]->addr == item)
// 			return i;
// 	}
// 	return -1;
// }

void Heap::remove(int item)
{
	// TODO: remove the element with value equal to item
	int index = getItemByAddr(item);
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
	int idx = getItemByAddr(key);
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
	//luon return ve key cua node can xoa
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
		arr[idx] = e;
		access(idx, e);
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
			return -1;
		}
		else
		{
			int key_to_remove = arr[0]->addr;
			int last = count - 1;
			for (int i = 0; i < last; i++)
			{
				arr[i] = arr[i + 1];
			}
			count--;
			return key_to_remove;
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
	MRU()
	{
		count = 0;
		arr = new Elem *[MAXSIZE];
	}
	~MRU() {}
	int insert(Elem *e, int idx)
	{
		if (count == MAXSIZE)
		{
			return -1;
		}
		for (int i = count; i > 0; i--)
		{
			arr[i] = arr[i - 1];
		}
		arr[0] = e;
		count++;
		return 0;
	}
	void access(int idx, Elem *e)
	{
		updateByReadOrWriteChange(idx);
	}
	int remove()
	{
		if (count == 0)
		{
			return 0;
		}
		int key_to_remove = arr[0]->addr;
		delete arr[0];
		count--;
		for (int i = 0; i < count; i++)
		{
			arr[i] = arr[i + 1];
		}
		return key_to_remove;
	}
	void print()
	{
		for (int i = 0; i < count; i++)
		{
			arr[i]->print();
		}
	}
	void updateFromHeapToArr() {}
	Elem *getValue(int idx) { return arr[idx]; }
	//update khong thay doi count
	void updateByReadOrWriteChange(int idxInLRU)
	{
		Elem *temp = arr[idxInLRU];
		for (int i = idxInLRU; i > 0; i--)
		{
			arr[i] = arr[i - 1];
		}
		arr[0] = temp;
	}
};
class LRU : public MRU
{
public:
	int remove() override
	{
		if (count == 0)
		{
			return -1;
		}
		int key_to_remove = arr[count - 1]->addr;
		delete arr[count - 1];
		count--;

		return key_to_remove;
	}
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
			if (minHeap->contains(e->addr))
			{
				minHeap->updateCount(e->addr);
			}
			int index = minHeap->push(e->addr);
			//update truoc,insert sau
			arr[index] = e;
			count++;

			updateFromHeapToArr();
			//to test
			//cout << " test o dong 468 cache.h" << endl;
			//minHeap->printHeap();
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
		int key = minHeap->getItemByIndex(idx);
		minHeap->updateCount(key);
		updateFromHeapToArr();
		//cout << " test o dong 483 cache.h" << endl;
		//minHeap->printHeap();
	}
	//luon xoa node min
	int remove()
	{
		int key_to_remove = minHeap->getItemByIndex(0);
		bool isRemove = minHeap->pop();
		if (isRemove)
		{
			updateFromHeapToArr();
			count--;
			return key_to_remove;
		}
		return -1;
	}
	void print()
	{
		for (int i = 0; i < count; i++)
		{
			arr[i]->print();
		}
	}
	//ham rieng
	Elem *search(int key)
	{
		for (int i = 0; i < count; i++)
		{
			if (arr[i]->addr == key)
				return arr[i];
		}
		return NULL;
	}
	//update lien tuc cho minheap-> arr
	void updateFromHeapToArr()
	{
		Elem **arrTemp = new Elem *[MAXSIZE];
		for (int i = 0; i < count; i++)
		{
			int key = minHeap->getItemByIndex(i);
			arrTemp[i] = this->search(key);
		}
		delete[] arr;
		arr = arrTemp;
	}
	Elem *getValue(int idx)
	{
		//read cung lam thay doi heap
		return arr[idx];
	}
};

//--------------------node for DBhash
class NodeDB
{
public:
	int key;
	int index;
	NodeDB(int key, int index)
	{
		this->key = key;
		this->index = index;
	}
};

enum STATUS_TYPE
{
	NIL,
	NON_EMPTY,
	DELETED
};
//--------------------end node for DBhash

class DBHashing : public SearchEngine
{
public:
	int (*h1)(int);
	int (*h2)(int);
	NodeDB **data;
	int count;
	int size;
	STATUS_TYPE *status;
	DBHashing(int (*h1)(int), int (*h2)(int), int s)
	{
		this->h1 = h1;
		this->h2 = h2;
		this->size = s;
		this->count = 0;
		this->data = new NodeDB *[size];
		this->status = new STATUS_TYPE[size];
		for (int i = 0; i < size; i++)
		{
			this->status[i] = NIL;
		}
	}
	~DBHashing() {}
	//to test
	void updateIndex(ReplacementPolicy *r)
	{
		r->updateFromHeapToArr();
		int size1 = r->getCount();
		int key = 0;
		//int idx = 0;
		int slot = 0;
		Elem *e;
		for (int i = 0; i < size1; i++)
		{
			//tim nodebd theo index
			e = r->getValue(i);
			key = e->addr;
			slot = this->searchSlotByKey(key);
			this->data[slot]->index = i;
		}
	}
	void InManHinh()
	{
		cout << "Day la in man hinh tu 547 cacheh:" << endl;
		for (int i = 0; i < size; i++)
		{
			if (status[i] == NON_EMPTY)
			{
				cout << "Tai index db " << i << "--key:" << data[i]->key << "--index that: " << data[i]->index << endl;
			}
		}
	}
	void insert(int key, int i)
	{
		int k = 0;
		while (k < size)
		{
			int slot = doubleHashing(key, k);
			if (status[slot] == NIL || status[slot] == DELETED)
			{
				NodeDB *temp = new NodeDB(key, i);
				data[slot] = temp;
				status[slot] = NON_EMPTY;
				count++;
				return;
			}
			k++;
		}
		int slot = 8;
		NodeDB *temp = new NodeDB(key, i);
		data[slot] = temp;
		status[slot] = NON_EMPTY;
		count++;
		return;
	}
	void deleteNode(int key)
	{
		int slot = searchSlotByKey(key);
		if (slot == -1)
			return;
		delete data[slot];
		data[slot] = NULL;
		status[slot] = DELETED;
		count--;
	}
	void print(ReplacementPolicy *q)
	{
		cout << "Prime memory:\n";
		for (int i = 0; i < this->size; i++)
		{
			if (status[i] == NON_EMPTY)
			{
				int idx = data[i]->index;
				q->getValue(idx)->print();
			}
		}
	}
	//return index real
	int search(int key)
	{
		int i = 0;
		while (i < size)
		{
			int slot = doubleHashing(key, i);
			if (status[slot] == NIL)
			{
				return -1;
			}
			else if (status[slot] == DELETED)
			{
				i++;
				continue;
			}
			else
			{
				if (data[slot]->key == key)
					return data[slot]->index;
			}
			i++;
		}
		//return data[8]->index;
		return -1;
	}

	//ham rieng
	int doubleHashing(int key, int i)
	{
		int a1 = this->h1(key);
		int a2 = this->h2(key);
		return (a1 + i * a2) % size;
	}
	int searchSlotByKey(int key)
	{
		int i = 0;
		while (i < size)
		{
			int slot = doubleHashing(key, i);
			if (status[slot] == NIL)
			{
				return -1;
			}
			else if (status[slot] == DELETED)
			{
				i++;
				continue;
			}
			else
			{
				if (data[slot]->key == key)
					return slot;
			}
			i++;
		}
		return 8;
		//return -1;
	}
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
		cout << "Print AVL in preorder:\n";
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
		q->updateFromHeapToArr();
		int size = q->getCount();
		int key = 0;
		//int idx = 0;
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