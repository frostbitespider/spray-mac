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
    pthread_mutex_init(&sig_mutex, NULL);
    pthread_cond_init(&recv_cond,NULL);
    pthread_cond_init(&sig_cond, NULL);
    recvBuf = (char *)malloc(Length);
    disBuf = (char *)malloc(Length);
    memset(recvBuf, 0, Length);
    memset(disBuf, 0, Length);
}
char* Receiver::getDisBuf() const{
    return disBuf;
}

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
    isReady=true;
}
// void Receiver::Run(){
//     pthread_attr_t attr;
//     pthread_attr_init(&attr);
//     pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
//     int err=pthread_create(&tid, &attr, _thread_t<Receiver,&Receiver::_RunThread>, this);
//     //int err=pthread_create(&lisThread, NULL, KeepListen, this);
//     if (err != 0){
//         printf("can't create thread: %s\n", strerror(err));
//     }
//     pthread_attr_destroy(&attr);
// }
void Receiver::start()
{
    //SA srcAddr;
    socklen_t socklen=sizeof(selfAddr);
    SA recvAddr;
    memset(&recvAddr,0,sizeof(recvAddr));
    printf("Receiver start\n");
    int i=0;
    int sendsockfd=Socket(AF_INET, SOCK_DGRAM, 0);
    isReady=true;
//    pthread_mutex_lock(&sig_mutex);
//    pthread_cond_signal(&sig_cond);
//    pthread_mutex_unlock(&sig_mutex);
    while(1)
    {
        msgcoming=false;
        //lock();
        curRecvLen=recvfrom(sockfd,recvBuf,Length,0,&recvAddr,&socklen);
        msgcoming=true;
        recvBuf[curRecvLen]=0;
        //printf("receive %d\n",curRecvLen);
        if(curRecvLen<=0)
            continue;
#ifdef DEBUGMOD
        if(curRecvLen<sizeof(Hdr)){
            for(int t=0;t<curRecvLen;t++)
                printf("%02x",recvBuf[t]);
            printf("\n");
            continue;
        }
#endif
        char* t=(char*)malloc(curRecvLen);
        memcpy(t,recvBuf+3*sizeof(uint32_t),curRecvLen-3*sizeof(uint32_t));
        Msg m(t,curRecvLen);
        std::shared_ptr<Msg> sm=std::make_shared<Msg>(m);
        lock();
        msgQueue.push(sm);
        pthread_cond_signal(&recv_cond);
        unlock();
        printf("RECV %d:%s\n",i++,recvBuf);

    }
}
//启动监听
void Receiver::Run(){
    if(!isReady){
        initSocket();
        this->start();
    }
    this->start();
}

Receiver::~Receiver()
{
    close(sockfd);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&recv_cond);
    pthread_mutex_destroy(&sig_mutex);
    pthread_cond_destroy(&sig_cond);
    printf("receiver dtor\n");
    //pthread_exit(NULL);
}
