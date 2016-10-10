#include "NetCommon.h"
int Socket(int family,int type,int protocol){
    int n;
    //printf("socket");
    if ((n=socket(family, type, protocol))<0) {
        return -1;
    }
    return n;
}
int setSockaddr(SAI * sockaddr,int family,const char* addr4,int port){
    //sockaddr->sin_len=sizeof(sockaddr);
    sockaddr->sin_family=family;
    //inet_pton(AF_INET, addr4, &sockaddr->sin_addr.s_addr);
    sockaddr->sin_port=htons(port);
    //printf("port=%d\n",sockaddr->sin_port);
    return inet_aton(addr4, &sockaddr->sin_addr);
}
char* dispAddr(const SA* sa,socklen_t salen){
    char portstr[8];
    static char str[128];
    switch (sa->sa_family) {
        case AF_INET:{
            SAI* s4=(SAI*)sa;
            if (inet_ntop(AF_INET, &s4->sin_addr, str, sizeof(str))==NULL) {
                return NULL;
            }
            if (ntohs(s4->sin_port)!=0) {
                snprintf(portstr, sizeof(portstr), ":%d",ntohs(s4->sin_port));
                strcat(str, portstr);
            }
            return str;
        }
        default:
            return str;
    }
}
char* get_network_addr(int sockfd,int opt){
    return NULL;
}
