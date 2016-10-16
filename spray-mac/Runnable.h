#include <pthread.h>
#include <stdio.h>
#pragma once
class Runnable{
public:
    bool isReady;
    virtual void Run()=0;
};
template <typename T,void (T::*Run)()>
void* _thread_t(void* args){
    T* This=(T*)args;
    This->Run();
    return NULL;
}

static void RunThread(pthread_t* tid,Runnable* r){
    pthread_create(tid,NULL,_thread_t<Runnable,&Runnable::Run>,r);
}
// class test:public Runnable{
//     void Run(){
//         printf("ddd\n");
//         getchar();
//     }
// };
// int main(){
//     test t;
//     pthread_t tid;
//     RunThread(&tid,&t);
//     pthread_exit(NULL);
//     return 0;
// }