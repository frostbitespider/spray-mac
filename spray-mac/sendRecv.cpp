#include "RTT.h"
#include "unp.h"
#include <setjmp.h>
#include <signal.h>
#define RTT_DEBUG
static RttInfo rttinfo;
static int rttinit=0;
static struct msghdr msgsend,msgrecv;
static struct hdr{
    uint32_t seq;
    uint32_t ts;
}sendhdr,recvhdr;

static void sig_alrm(int signo);
static sigjmp_buf jmpbuf;

ssize_t send_and_recv(int fd,void* outbuff,size_t outbytes,void* inbuff,size_t inbytes,SA* dstAddr,socklen_t dstlen)
{
    ssize_t n;
    struct iovec iovsend[2],iovrecv[2];
    if(rttinit==0){
        rtt_init(&rttinfo);
        rttinit=1;
        rtt_d_flag=1;
    }
    sendhdr.seq++;
    msgsend.msg_name=dstAddr;
    msgsend.msg_namelen=dstlen;
    msgsend.msg_iov=iovsend;
    msgsend.msg_iovlen=2;
    //这里改为messageheader
    iovsend[0].iov_base=&sendhdr;
    iovsend[0].iov_len=sizeof(struct hdr);
    iovsend[1].iov_base=outbuff;
    iovsend[1].iov_len=outbytes;
    msgrecv.msg_name=NULL;
    msgrecv.msg_namelen=0;
    msgrecv.msg_iov=iovrecv;
    msgrecv.msg_iovlen=2;
    //msgrecv{NULL,0,iovrecv,2};
    iovrecv[0].iov_base=&recvhdr;
    iovrecv[0].iov_len=sizeof(struct hdr);
    iovrecv[1].iov_base=inbuff;
    iovrecv[1].iov_len=inbytes;
    // iovrecv[0]{&recvhdr,sizeof(struct hdr)};
    // iovrecv[1]{inbuff,inbytes};
    //构造请求
    signal(SIGALRM,sig_alrm);//建立信号处理函数
    rtt_newpack(&rttinfo);//初始化rexmt到0
sendagain:
    sendhdr.ts=rtt_ts(&rttinfo);
    sendmsg(fd,&msgsend,0);
    printf("sending iovec:%s\n",(char*)msgsend.msg_iov[1].iov_base);
    alarm(rtt_start(&rttinfo));//set alarm for RTO seconds
    if(sigsetjmp(jmpbuf,1)!=0){
        if(rtt_timeout(&rttinfo)<0){//double RTO,retransmit enough
            //放弃
            printf("response timeout\n");
            rttinit=0;
            errno=ETIMEDOUT;
            return -1;
        }
        goto sendagain;//retransmit
    }

    do{
        n=recvmsg(fd,&msgrecv,0);
        printf("receive iovec:%s\n",msgrecv.msg_iov[1].iov_base);
    }while(n<sizeof(struct hdr)||recvhdr.seq!=sendhdr.seq);//序列号不对
    alarm(0);//stop timer
    rtt_stop(&rttinfo,rtt_ts(&rttinfo)-recvhdr.ts);//计算rtt，更新estimator
    return (n-sizeof(struct hdr));

}
static void sig_alrm(int signo){
    siglongjmp(jmpbuf,1);
}

int testsend(){
    int sockfd;
    SAI dstAddr;
    char inbuff[20];
    if((sockfd=socket(AF_INET,SOCK_DGRAM,0))==-1){
        return -1;
    }
    memset(&dstAddr,0,sizeof(dstAddr));
    //memset(&selfAddr,0,sizeof(selfAddr));
    const char* str="192.168.0.107";
   // const int on=1;
    dstAddr.sin_family=AF_INET;
    dstAddr.sin_port=htons(8080);
    inet_pton(AF_INET,str,&dstAddr.sin_addr);
    char m[20]="message";
    size_t l=strlen(m);
    //int fd,const void* outbuff,size_t outbytes,void* inbuff,size_t inbytes,const SA* dstAddr,socklen_t dstlen
    send_and_recv(sockfd,m,l,inbuff,20,(SA*)&dstAddr,sizeof(dstAddr));
    getchar();
    return 0;
}
