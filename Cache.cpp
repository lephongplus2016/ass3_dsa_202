#include "main.h"
Cache::Cache(SearchEngine* s,ReplacementPolicy* r):rp(r),s_engine(s) {}
Cache::~Cache(){
	delete rp;
	delete s_engine;
}
Data* Cache::read(int addr) {
    return NULL;
}
Elem* Cache::put(int addr, Data* cont) {
    return NULL;
}
Elem* Cache::write(int addr, Data* cont) {
    return NULL;
}
void Cache::printRP() {
}
void Cache::printSE() {
}
