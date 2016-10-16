#include <pthread.h>
#include "unp.h"
#include "Runnable.h"
#include <queue>
#include "Sender.h"
//#include "Displayer.h"
#pragma once
class Displayer;
class Receiver:public Runnable{
public:
    friend class Displayer;
    Receiver();
    ~Receiver();
    void lock();
    void unlock();
    char* getDisBuf() const;
    void Run();
    ///queue 不是线程安全的
    std::queue<Msg*> msgQueue;
private:
    void start();
    void initSocket();
    const int Length=1024;
    char* recvBuf;
    char* disBuf;
    int sockfd;
    SAI selfAddr;
    int curRecvLen;
    //pthread_t lisThread;
    //pthread_mutex_t Readmutex;
    pthread_mutex_t mutex;
    pthread_mutex_t sig_mutex;
    pthread_cond_t recv_cond;
    pthread_cond_t sig_cond;
    bool msgcoming;
};

void* KeepListen(void* r);
