#include "Displayer.h"
#include "MessageSender.h"
//tid1 to start display
//tid2 to start receiver

Displayer::Displayer(Receiver* re):r(re){
    isReady=true;
}
// Displayer::Displayer(pthread_t& tid1,pthread_t& tid2){
//     r=new Receiver();
//     r->Run(tid2);
//     pthread_create(&tid1,NULL,_thread_t<Displayer,&Displayer::_Run>,this);
// }
void Displayer::Run(){
    //pthread_detach(pthread_self());
    //MessageSender sender(SINGLECAST,ONCE);
    int i=0;
//    pthread_mutex_lock(&(r->sig_mutex));
//    pthread_cond_wait(&(r->sig_cond),&(r->sig_mutex));
//    pthread_mutex_unlock(&(r->sig_mutex));
    while (1)
    {
        //pthread_cond_wait(&recv_cond,&mutex);
        r->lock();
//        while(!r->msgcoming){
//            pthread_cond_wait(&(r->recv_cond),&(r->mutex));
//            //printf("FUCK2\n");
//        }
        //r->lock();
        pthread_cond_wait(&(r->recv_cond),&(r->mutex));
        while (r->msgQueue.size()>0) {
            Msg* m=r->msgQueue.front();
            printf("DISPLAY %d:%s\n", i++,m->buf);
//            Msg ack{"ack"};
//            sender.send(ack);
            //dosomething
            //文件传输不应该使用udp
            //r->lock();
            r->msgQueue.pop();
            delete m;

            //r->unlock();
        }
        r->unlock();
    }
    //receiver->disp();
}
