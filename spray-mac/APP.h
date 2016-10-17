
#include <pthread.h>
#include "Displayer.h"
#include "MessageSender.h"
#include "Sender.h"
int test()
{
    // pthread_t thDisp;
    // Displayer dis;
    // ThreadDisp(&thDisp,&dis);
    pthread_t threads[10];
    Receiver* r=new Receiver();
    RunThread(threads,r);
    Displayer* disp=new Displayer(r);
    RunThread(threads+1,disp);
    getchar();
    return 0;

}
