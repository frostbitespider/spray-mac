#pragma once

typedef struct{
    char uName[32];
    uint32_t uID;
    uint32_t type;
    uint32_t info;
} MessageHeader;
typedef struct{
    uint32_t seq;
    uint32_t ts;
    MessageHeader mh;
} Header;