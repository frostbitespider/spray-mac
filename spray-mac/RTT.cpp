#include "RTT.h"
int rtt_d_flag=0;

inline float rtt_rtocalc(RttInfo* rttinfo){
    return rttinfo->rtt_srtt+4.0*rttinfo->rtt_rttvar;
}
static float rtt_minmax(float rto){
    if(rto<RTT_RXTMIN)
        rto=RTT_RXTMIN;
    else if(rto>RTT_RXTMAX)
        rto=RTT_RXTMAX;
    return rto;
}

void rtt_debug(RttInfo* ptr);
void rtt_init(RttInfo* ptr){
    struct timeval tv;
    gettimeofday(&tv,NULL);
    ptr->rtt_base=tv.tv_sec;
    ptr->rtt_rtt=0;
    ptr->rtt_srtt=0;
    ptr->rtt_rttvar=0.75;
    ptr->rtt_rto=rtt_minmax(rtt_rtocalc(ptr));
    //first rto is 3 seconds
}
void rtt_newpack(RttInfo* ptr){
    ptr->rtt_nrexmt=0;
}
int rtt_start(RttInfo* ptr){
    return (int)(ptr->rtt_rto+0.5);
}
void rtt_stop(RttInfo* ptr,uint32_t ms){
    double delta;
    ptr->rtt_rtt=ms/1000.0;
    delta=ptr->rtt_rtt-ptr->rtt_srtt;
    ptr->rtt_srtt+=delta/8;
    if(delta<0.0){
        delta=-delta;
    }
    ptr->rtt_rttvar+=(delta-ptr->rtt_rttvar)/4;
    ptr->rtt_rto=rtt_minmax(rtt_rtocalc(ptr));
}
int rtt_timeout(RttInfo* ptr){
    ptr->rtt_rto*=2;
    if(++ptr->rtt_nrexmt>RTT_MAXNREXMT)
        return -1;
    return 0;
}
uint32_t rtt_ts(RttInfo* ptr){
    uint32_t ts;
    struct timeval tv;
    gettimeofday(&tv,NULL);
    ts=((tv.tv_sec-ptr->rtt_base)*1000)+(tv.tv_usec/1000);
    return ts;
}
