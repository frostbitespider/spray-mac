#include <pthread.h>
#pragma once
template <typename T,void (T::*_RunThread)()>
void* _thread_t(void* args){
    T* This=(T*)args;
    This->_RunThread();
    return NULL;
}