#include "unp.h"
#pragma once
enum Strategy{
    ONCE,
    NEEDACK
};
enum SendType{
    BROADCAST,
    GROUPCAST,
    SINGLECAST
};
class Msg{
    public:
    Msg():_size(0){}
    Msg(const char* m){
        _str=(void*)m;
        _size=strlen(m);
    }
    Msg(char* c,int l):_str(c),_size(l){}
    ~Msg(){
        printf("msg dtor\n");
        free(_str);
    }
    void* _str;
    int _size;
};
typedef struct{
    enum Strategy strategy;
    int WaitTime;
    enum SendType sendtype;
}Attr;
class Sender{
    public:
    virtual int send(Msg&)=0;
    virtual int abort()=0;
   //virtual void sock_close()=0;
    virtual ~Sender(){}
    Sender(){}
};
