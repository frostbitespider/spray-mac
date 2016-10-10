#include "Receiver.h"

void Receiver::lock()
{
    pthread_mutex_lock(&mutex);
}
void Receiver::unlock()
{
    pthread_mutex_unlock(&mutex);
}
//void Receiver::setSigcond(pthread_cond_t* cond){
//    sig_cond=cond;
//}
Receiver::Receiver()
{
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&recv_cond,NULL);
    recvBuf = (char *)malloc(Length);
    disBuf = (char *)malloc(Length);
    memset(recvBuf, 0, Length);
    memset(disBuf, 0, Length);
    //pthread_exit(NULL);
}
char* Receiver::getDisBuf() const{
    return disBuf;
}
/////约有1/3信息recv但没有disp
//void Receiver::disp()
//{
//    pthread_detach(pthread_self());
//    int i=0;
//    while (1)
//    {
//        //lock();
//        //pthread_cond_wait(&recv_cond,&mutex);
//        pthread_cond_wait(&recv_cond,&mutex);
//        //lock();
//        memcpy(disBuf, recvBuf, curRecvLen); //dest source size_t
//        //disBuf[curRecvLen] = 0;
//        //for(int a=0;a<1000;a++);//do something
//        printf("DISPLAY %d:%s\n", i++,disBuf);
//        //unlock();
//        //lock();
//    }
//}
void Receiver::initSocket(){
    if((sockfd=Socket(AF_INET,SOCK_DGRAM,0))==-1){
        return;
    }
    int n=220*1024;
    memset(&selfAddr,0,sizeof(selfAddr));
    //selfAddr.
    const int on=1;
    //setSockaddr(&selfAddr,AF_INET,str,8000);
    selfAddr.sin_family=AF_INET;
    selfAddr.sin_port=htons(8080);
    selfAddr.sin_addr.s_addr=htonl(INADDR_ANY);
    setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &n, sizeof(n));
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
    if(bind(sockfd,(SA*)&selfAddr,sizeof(selfAddr))==-1){
        printf("bind error\n");
        return;
    }
}
void Receiver::Run(pthread_t& tid){
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
    int err=pthread_create(&tid, &attr, _thread_t<Receiver,&Receiver::_RunThread>, this);
    //int err=pthread_create(&lisThread, NULL, KeepListen, this);
    if (err != 0){
        printf("can't create thread: %s\n", strerror(err));
    }
    pthread_attr_destroy(&attr);
}
void Receiver::start()
{
    initSocket();
    //SA srcAddr;
    socklen_t socklen=sizeof(selfAddr);
    SA recvAddr;
    memset(&recvAddr,0,sizeof(recvAddr));
    printf("Receiver start\n");
    int i=0;
    int sendsockfd=Socket(AF_INET, SOCK_DGRAM, 0);
    while(1)
    {
        //lock();
        curRecvLen=recvfrom(sockfd,recvBuf,Length,0,&recvAddr,&socklen);
        recvBuf[curRecvLen]=0;
        char* t=(char*)malloc(curRecvLen);
        memcpy(t,recvBuf,curRecvLen);
        Msg m(t,curRecvLen);
        lock();
        msgQueue.push(&m);
        pthread_cond_signal(&recv_cond);
        unlock();
        printf("RECV %d:%s\n",i++,recvBuf);
        for(int t=0;t<curRecvLen;t++)
            printf("%c ",recvBuf[t]);
        printf("\n");

        //sleep(8);
        //sendto(sendsockfd, recvBuf, curRecvLen, 0, &recvAddr, sizeof(recvAddr));
        //pthread_mutex_trylock(&mutex);

        //lock();
        //lock();
        //unlock(mutex);
        //pthread_cond_signal(&recv_cond);
        //unlock();
    }
}
//启动监听
void Receiver::_RunThread(){
    this->start();
}

Receiver::~Receiver()
{
    close(sockfd);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&recv_cond);
    pthread_exit(NULL);
}
