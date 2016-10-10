#include "MessageSender.h"
#pragma once
class SenderFactory{
    public:
    SenderFactory(){}
    static senderFactory;
    Sender* getSender(){
         if(1){
             return new MessageSender(m,8);
         }
     }
}
// void how_to_use(){
//     tr1::shared_ptr<Sender> pSender(getSender());
// }
