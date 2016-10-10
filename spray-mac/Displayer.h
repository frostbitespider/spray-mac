#include <pthread.h>
#include <iostream>
#include <stdlib.h>
#include <memory.h>
#include "ThreadMan.h"
#include "Receiver.h"
#include "Sender.h"
#pragma once
class Receiver;
class Displayer{
    public:
    Displayer(pthread_t& tid1,pthread_t& tid2);
    ~Displayer(){}
    Receiver& getReceiver();
    //friend void background(Displayer* dis);
    void _RunThread();
    private:
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    Receiver* r;
    //void show(Receiver& r);
};
void startDisp(Displayer* dis);
void* StartDisp(void* args);
//void ThreadDisp(pthread_t* th,Displayer* disp);
