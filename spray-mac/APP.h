
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
    char* buff=(char*)malloc(1024);
//    while(gets(buff)!=NULL){
//        Msg m{buff};
//        MessageSender* s=new MessageSender("192.168.1.242",8080,SINGLECAST,ONCE);
//        s->send(buff);
//        s->sock_close();
//    }
    getchar();
    return 0;

}