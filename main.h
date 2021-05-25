#ifndef MAIN_H
#define MAIN_H
#include<string>
#include <iostream>
#include <sstream>
#include <fstream>

class	ReplacementPolicy;
class		SearchEngine;

using namespace std; 
int MAXSIZE= 5;

class Data {
	public:
	virtual ~Data() = default;
	virtual string getValue() = 0;
};

class Int: public Data {
	int value;
	public:
	Int(int v):value(v){}
	string getValue() {
		return to_string(value);
	}
};
class Float: public Data {
	float value;
	public:
	Float(float v):value(v){}
	string getValue() {
		return to_string(value);
	}
};
class Bool:public Data {
	bool value;
	public:
	Bool(bool v):value(v){}
	string getValue() {
		return value?"true":"false";
	}
};
class Address:public Data {
	int value;
	public:
	Address(int v):value(v){}
	string getValue() {
		return to_string(value)+"A";
	}
};

class Elem {
	public:
		int addr;
		Data* data;
		bool sync;
		Elem(int a,Data* d,bool s):addr(a),data(d),sync(s){}
		~Elem() {delete data;}
		void print() {
			cout << addr << " " << data->getValue() << " " << (sync?"true":"false") << endl;
		}
};

class Cache {
		ReplacementPolicy* rp;
		SearchEngine* s_engine;
	public:
		Cache(SearchEngine* s,ReplacementPolicy* r);
		~Cache();
		Data* read(int addr);
		Elem* put(int addr, Data* cont);
		Elem* write(int addr, Data* cont);
		void printRP();
		void printSE();	
};

#endif