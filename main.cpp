#include <stdio.h>
#include "main.h"
#include "Cache.h"
#include "Cache.cpp"

int h1(int k) {
	return k + 1;
}
int h2(int k) {
	return 2 * k + 1;
}
int h3(int k) {
	return 3 * k;
}
int h4(int k) {
	return 3 * k + 5;
}
Data* getData(string s) {
	stringstream ss;
	ss << s;
	int idata;
    float fdata;
    if (ss >> idata) 
		return new Int(idata);
    else if (ss >> fdata)
    	return new Float(fdata);
    else if (s.compare("true") || s.compare("false"))
    	return new Bool(s.compare("true"));
    else {
		s.resize(s.size()-1);
		return new Address(stoi(s));
	}
    return NULL;	
}
void simulate(string filename)
{
  	ifstream ifs;
  	ifs.open(filename, std::ifstream::in);
  	string s;
	SearchEngine* sr;
	ReplacementPolicy* rp;
	Cache* c;
  	while (getline(ifs, s))
  	{
    	stringstream ss(s);
    	string code,tmp;
    	ss >> code;
    	int addr;

    	switch (code[0]) {
			case 'M': //MAXSIZE
				ss >> addr;
				MAXSIZE = addr;
				break;
			case 'S': // Search Engine
				ss >> tmp;
				if (tmp[0] == 'A')
					sr = new AVL();
				else {
					int (*hash1)(int);
					int (*hash2)(int);
					hash1 = tmp[1]=='1'?h1:tmp[1]=='2'?h2:tmp[1]=='3'?h3:h4;
					hash2 = tmp[2]=='1'?h1:tmp[2]=='2'?h2:tmp[2]=='3'?h3:h4;
					ss >> addr;//size
					sr = new DBHashing(hash1,hash2,addr);
				}
				break;
			case 'T':// ReplacementPolicy
				ss >> addr;
				if (addr==1) rp = new LRU();
				else if (addr==2) rp = new LFU();
				else if (addr==3) rp = new FIFO();
				else rp = new MRU();
				c = new Cache(sr,rp);
				break;
    		case 'R': //read
			    ss >> addr; 
    			Data* res;
				res = c->read(addr);
    			if (res == NULL) {
    				ss >> tmp;
    				c->put(addr,getData(tmp));
    			} else {
					cout << res->getValue() << endl;
				}
    			break;
        	case 'U': //put
                ss >> addr;
                ss >> tmp;
                c->put(addr,getData(tmp));
                break;
    		case 'W': //write
                ss >> addr;
    			ss >> tmp;
    			c->write(addr,getData(tmp));
    			break;
    		case 'P': // print
    			cout << "Print replacement buffer\n";
    			c->printRP();
    			break;
    		case 'E': //
    			cout << "Print search buffer\n";
    			c->printSE();
    			break;
    	}
  	}
	delete c;
}
int main(int argc, char* argv[]) {
	if (argc < 2)
    	return 1;
  	const char *fileName = argv[1];
    simulate(string(fileName));
	return 0;
}

