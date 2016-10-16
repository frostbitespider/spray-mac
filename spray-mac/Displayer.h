#include <pthread.h>
#include <iostream>
#include <stdlib.h>
#include <memory.h>
#include "Runnable.h"
#include "Receiver.h"
#include "Sender.h"
#pragma once
class Receiver;
class Displayer:public Runnable{
    public:
    Displayer();
    //Displayer(pthread_t& tid1,pthread_t& tid2);
    ~Displayer(){}
    Receiver& getReceiver();
    void Run();
    private:
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    Receiver* r;
};