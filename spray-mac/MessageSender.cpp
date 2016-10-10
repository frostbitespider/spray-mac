#include "MessageSender.h"
MessageSender::MessageSender(SendType t,Strategy s):type(t),strategy(s){};
MessageSender::MessageSender(const char* ip4,int port,SendType t,Strategy s):type(t),strategy(s){
    setDst(ip4, port);
    //setSockaddr(&selfAddr,AF_INET,str,8000);
    //setSockaddr(&dstAddr,AF_INET,ip4,8080);
    //setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
}
MessageSender::~MessageSender(){
    close(sockfd);
}
void MessageSender::sock_close(){
    close(sockfd);
}
void MessageSender::setDst(const char* ip4,int port){
    if((sockfd=Socket(AF_INET,SOCK_DGRAM,0))==-1){
        return;
    }
    memset(&dstAddr,0,sizeof(dstAddr));
    //memset(&selfAddr,0,sizeof(selfAddr));
    //const char* str="127.0.0.1";
    const int on=1;
    dstAddr.sin_family=AF_INET;
    dstAddr.sin_port=htons(8080);
    inet_pton(AF_INET,ip4,&dstAddr.sin_addr);
}
//没有显式bind,内核选择一个临时端口
int MessageSender::send(Msg m)
{
    //printf("%s", m.buf);
    if (type == BROADCAST)
    {
        const char *broadcast_addr = "192.168.1.1";
        const int on = 1;
        setSockaddr(&dstAddr, AF_INET, broadcast_addr, 8080);
        if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) == -1)
        {
            printf("set sockopt fail\n");
            return -1;
        }
        printf("BROADCASTING\n");
        sendto(sockfd, (char*)m.buf, m.bufsize, 0, (SA *)&dstAddr, sizeof(dstAddr));
    }
    else if(type==SINGLECAST){
        if(strategy==TILRECV){
            while(1){
                printf("SENDING:%s\n",(char*)m.buf);
                sendto(sockfd, (char*)m.buf, m.bufsize, 0, (SA *)&dstAddr, sizeof(dstAddr));
                sleep(2);
            }
        }
        else if (strategy == ONCE)
        {
            printf("SENDING:%s\n", (char *)m.buf);
            sendto(sockfd, (char *)m.buf, m.bufsize, 0, (SA *)&dstAddr, sizeof(dstAddr));
        }
    }
    sock_close();
    return 0;
}
int MessageSender::abort(){
    return 0;
}
// int MSmain(int argc,char* argv[]){
//      Msg m{"this is a message"};
//     // MessageSender ms(m,4);
//     // ms.send();
//     Sender* s=new MessageSender(m,"192.168.1.242",8080);
//     //s->send(BROADCAST,ONCE);
//     s->send(SINGLECAST,TILRECV);
//     return 0;
// }