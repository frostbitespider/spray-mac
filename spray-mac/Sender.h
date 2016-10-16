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
    Msg():buf(NULL),bufsize(0){}
    Msg(const char* m){
        buf=(byte*)(m);
        bufsize=strlen(m);
    }
    Msg(byte* b,int s):buf(b),bufsize(s){}
    Msg(char* c,int s):buf((byte*)c),bufsize(s){}
    ~Msg(){
        free(buf);
    }
    byte* buf;
    int bufsize;
};
typedef struct{
    enum Strategy strategy;
    int WaitTime;
    enum SendType sendtype;
}Attr;
class Sender{
    public:
    virtual int send(SendType,Strategy)=0;
    virtual int abort()=0;
    virtual void sock_close()=0;
    virtual ~Sender(){}
    Sender(){}
};
