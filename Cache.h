#ifndef CACHE_H
#define CACHE_H

#include "main.h"


class ReplacementPolicy {
	protected:
	int count;
	Elem** arr;
	
	public:
	virtual int insert(Elem* e,int idx) = 0;//insert e into arr[idx] if idx != -1 else into the position by replacement policy
	virtual void access(int idx) = 0;//idx is index in the cache of the accessed element 
	virtual int remove() = 0; 
	virtual void print() = 0;
	
	bool isFull() {return count == MAXSIZE;}
	bool isEmpty() {return count == 0;}
	Elem* getValue(int idx){return arr[idx];}
	void replace(int idx,Elem* e) {
		delete arr[idx];
		access(idx);
		arr[idx] = e;
	}
};

class SearchEngine {
	public:
	virtual int search(int key) = 0; // -1 if not found
	virtual void insert(int key,int idx) = 0;
	virtual	void deleteNode(int key) = 0;
	virtual void print(ReplacementPolicy* r) = 0;
};

class FIFO : public ReplacementPolicy {
	public:
	FIFO(){count = 0; arr = new Elem*[MAXSIZE];}
	~FIFO(){}
	int insert(Elem* e,int idx){return 0;}
	void access(int idx) {}
	int remove(){return 0;}
	void print(){}
};

class MRU : public ReplacementPolicy {
	public:
	MRU(){}
	~MRU(){}
	int insert(Elem* e,int idx){return 0;}
	void access(int idx){}
	int remove(){return 0;}
	void print(){}
};
class LRU: public MRU {
	public:
	int remove() override {return 0;}
};

class LFU: public ReplacementPolicy {
	public:
	LFU(){}
	~LFU(){}
	int insert(Elem* e,int idx){return 0;}
	void access(int idx){}
	int remove(){return 0;}
	void print(){}
};

class DBHashing: public SearchEngine {public:
	DBHashing(int (*h1)(int),int (*h2)(int),int s){}
	~DBHashing(){}
	void insert(int key,int i){}
	void deleteNode(int key){}
	void print(ReplacementPolicy* q){}
	int search(int key) {return 0;}
};
class AVL : public SearchEngine {
	public:
		AVL() {}
		~AVL() {}
		void insert(int key,int i){}
		void deleteNode(int key){}
		void print(ReplacementPolicy* q) {}
		int search(int key){return 0;}
};






#endif