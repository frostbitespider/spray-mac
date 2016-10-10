#include <pthread.h>
#include "NetCommon.h"
#include "ThreadMan.h"
#include <queue>
#include "Sender.h"
//#include "Displayer.h"
#pragma once
class Displayer;
class Receiver{
public:
    friend class Displayer;
    Receiver();
    ~Receiver();
    void lock();
    void unlock();
    //void copyto(char* buf,int len);
    char* getDisBuf() const;
    void _RunThread();
    void Run(pthread_t& tid);
    //void disp();
    //void setSigcond(pthread_cond_t* cond);
    ///queue 不是线程安全的
    std::queue<Msg*> msgQueue;
    //friend void* keepListen(Receiver* r);
private:
    void start();
    void initSocket();
    const int Length=1024;
    char* recvBuf;
    char* disBuf;
    int sockfd;
    SAI selfAddr;
    int curRecvLen;
    int curDisLen;
    //pthread_t lisThread;
    //pthread_mutex_t Readmutex;
    pthread_mutex_t mutex;
    pthread_cond_t recv_cond;
    pthread_cond_t* sig_cond;
};

void* KeepListen(void* r);
