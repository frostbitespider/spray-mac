#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/errno.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <arpa/inet.h>
#define SA struct sockaddr
#define SAI struct sockaddr_in
#define byte unsigned char
#define Malloc(type) malloc(sizeof(type))
#pragma once
char* dispAddr(const SA* sa,socklen_t len);
char* get_network_addr(int sockfd,int opt);
int Socket(int family,int type,int protocol);
int setSockaddr(SAI * sockaddr,int family,const char* addr4,int port);

