#include <setjmp.h>
#include <sys/time.h>
#include <stdint.h>
#include <pthread.h>
#pragma once
#define RTT_RXTMIN 2//min retransmit timeout value, in seconds
#define RTT_RXTMAX 60//max retransmit timeout value, in seconds
#define RTT_MAXNREXMT 5//max # times to retransmit
typedef struct {
    float rtt_rtt;//most recent measured rtt, in seconds
    float rtt_srtt;//smoothed rtt estimator, in seconds
    float rtt_rttvar;//smoothed mean deviation, in seconds
    float rtt_rto;//current rto to use, in seconds
    int rtt_nrexmt;//times retransmitted:0,1,2...
    uint32_t rtt_base;//seconds since 1/1/1970 at start
}RttInfo;

void rtt_debug(RttInfo*);
void rtt_init(RttInfo*);
void rtt_newpack(RttInfo*);
int rtt_start(RttInfo*);
void rtt_stop(RttInfo*,uint32_t);
int rtt_timeout(RttInfo*);
uint32_t rtt_ts(RttInfo*);

extern int rtt_d_flag;//can be set to nonzero for addl info


/*
delta=rtt-srtt
srtt=srtt+g*delta
rttvar=rttvar+h(abs(delta)-rttvar)
rto=srtt+4*rttvar



struct msghdr {
               void         *msg_name;       / optional address /
//               socklen_t     msg_namelen;    / size of address /
//               struct iovec *msg_iov;        / scatter/gather array /
//               size_t        msg_iovlen;     / # elements in msg_iov 
//               void         *msg_control;    / ancillary data, see below 
//              size_t        msg_controllen; / ancillary data buffer len 
//               int           msg_flags;      / flags on received message 
//           }
*/
