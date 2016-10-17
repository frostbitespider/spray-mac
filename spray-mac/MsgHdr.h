#pragma once

typedef struct{
    char uName[32];
    uint32_t uID;
    uint32_t type;
    uint32_t info;
} Msghdr;
typedef struct{
    uint32_t seq;
    uint32_t ts;
    uint32_t ackseq;
    Msghdr mh;
} Hdr;
