#include "Sender.h"
#include "MsgHdr.h"
//#include "SenderEx.h"
class MessageSender{
    public:
    MessageSender(SendType t,Strategy s);
    MessageSender(const char* ip4,int port,SendType t,Strategy s);
    //MessageSender(SAI& addr,char* message,int length);
    ~MessageSender();
    ///how to send
    ///if type is broadcast,dstAddr can be null
    int send(Msg m);
    int abort();
    void sock_close();
    void setDst(const char* ip4,int port);
    private:
    int sockfd;
    SAI selfAddr;
    SAI dstAddr;
    SendType type;
    Strategy strategy;
};
