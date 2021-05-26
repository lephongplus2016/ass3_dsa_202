#include "main.h"
Cache::Cache(SearchEngine *s, ReplacementPolicy *r) : rp(r), s_engine(s) {}
Cache::~Cache()
{
    delete rp;
    delete s_engine;
}
Data *Cache::read(int addr)
{
    //ko the tra trong rp, ma phai search trong e_engine
    int a = this->s_engine->search(addr);
    if (a != -1)
    {
        return a;
    }
    return NULL;
}
Elem *Cache::put(int addr, Data *cont)
{
    return NULL;
}
Elem *Cache::write(int addr, Data *cont)
{
    return NULL;
}
void Cache::printRP()
{
}
void Cache::printSE()
{
}
