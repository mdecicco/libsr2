#pragma once
#include <libsr2/types.h>

namespace sr2 {
    struct msgMessage;

    class msgMsgSink {
        public:
            msgMsgSink();
            virtual ~msgMsgSink();

            virtual void onMessage(msgMessage* message);
    };
};