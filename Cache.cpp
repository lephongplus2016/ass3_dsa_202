#include "main.h"
Cache::Cache(SearchEngine *s, ReplacementPolicy *r) : rp(r), s_engine(s) {}
Cache::~Cache()
{
    delete rp;
    delete s_engine;
}
Data *Cache::read(int addr)
{
    //tim theo key, tra ve theo index
    s_engine->updateIndex(rp);
    //ko the tra trong rp, ma phai search trong e_engine
    int a = this->s_engine->search(addr);
    if (a != -1)
    {

        Data *temp = rp->getValue(a)->data;
        //cap nhat truy cap
        rp->access(a, NULL);
        return temp;
    }
    return NULL;
}
Elem *Cache::put(int addr, Data *cont)
{
    if (rp->isFull())
    {

        Elem *temp = rp->getValue(0);
        int addr_temp = temp->addr;
        rp->remove();
        s_engine->deleteNode(addr_temp);
        //them node
        Elem *e = new Elem(addr, cont, true);
        int idx = rp->getCount();
        int idx_new = rp->insert(e, idx);

        s_engine->insert(addr, idx_new);
        //to test

        return temp;
    }
    else
    { // not full
        Elem *temp = new Elem(addr, cont, true);
        int idx = rp->getCount();
        int idx_new = rp->insert(temp, idx); //chinh la vi tri rong tiep theo can them
        //them vao s_engine
        //rp->print();
        s_engine->insert(addr, idx_new);
        //cout << " insert node" << addr << " idx " << idx_new << endl;
        //s_engine->InManHinh();
        return NULL;
    }
}

//return null neu khong co node nao bi xoa
//return elem* node vua xoa
Elem *Cache::write(int addr, Data *cont)
{
    s_engine->updateIndex(rp);
    int idx = s_engine->search(addr);
    if (idx != -1)
    { //tim thay
        Elem *e = new Elem(addr, cont, false);
        rp->replace(idx, e);
    }
    else                  //temp->delete , e->insert
    {                     //khong tim thay, tao node moi
        if (rp->isFull()) //neu full
        {
            Elem *temp = rp->getValue(0);
            int addr_t = temp->addr;
            rp->remove();
            s_engine->deleteNode(addr_t);
            //them node
            Elem *e = new Elem(addr, cont, false);
            int idx = rp->getCount();
            int idx_new = rp->insert(e, idx);
            s_engine->insert(addr, idx_new);

            return temp;
        }
        else
        { // them nhung khong can xoa
            Elem *e = new Elem(addr, cont, false);
            int idx = rp->getCount();

            int idx_new = rp->insert(e, idx); //chinh la vi tri rong tiep theo can them

            s_engine->insert(addr, idx_new);
        }
    }
    return NULL;
}
void Cache::printRP()
{
    //RP
    rp->print();
}
void Cache::printSE()
{
    s_engine->updateIndex(rp);
    s_engine->print(rp);
}
