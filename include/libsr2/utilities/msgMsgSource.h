#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class msgMsgSink;

    enum class MSG_TYPE : u16 {
        UNK120 =                        0x1,
        UNK119 =                        0x2,
        UNK40 =                         0x4,
        UNK41 =                         0x5,
        UNK19 =                         0x7,
        UNK92 =                         0x8,
        UNK20 =                         0x9,
        UNK52 =                         0xa,
        UNK51 =                         0x13,
        START_PRESSED =                 0x15,
        UNK74 =                         0x18,
        UNK73 =                         0x19,
        UNK108 =                        0x1e,
        UNK109 =                        0x1f,
        UNK107 =                        0x20,
        UNK135 =                        0x21,
        UNK87 =                         0x30,
        UNK88 =                         0x32,
        UNK132 =                        0x34,
        UNK111 =                        0x35,
        UNK110 =                        0x3e,
        UNK133 =                        0x3f,
        UNK134 =                        0x40,
        UNK75 =                         0x41,
        UNK116 =                        0x42,
        UNK117 =                        0x43,
        UNK118 =                        0x44,
        UNK67 =                         0x45,
        UNK76 =                         0x46,
        UNK127 =                        0x47,
        UNK125 =                        0x48,
        UNK126 =                        0x49,
        UNK122 =                        0x4a,
        UNK123 =                        0x4b,
        UNK124 =                        0x4c,
        UNK121 =                        0x50,
        UNK45 =                         0x51,
        UNK42 =                         0x52,
        UNK44 =                         0x53,
        UNK50 =                         0x56,
        UNK68 =                         0x57,
        UNK77 =                         0x59,
        UNK78 =                         0x5a,
        UNK128 =                        0x61,
        UNK129 =                        0x804,
        UNK65 =                         0x805,
        RIGHT_STICK_X_VALUE =           0x807,
        RIGHT_STICK_Y_VALUE =           0x808,
        UNK72 =                         0x809,
        RESET_GAME =                    0x1001,
        LOAD_GAME =                     0x3801,
        ENTER_GAME =                    0x3802,
        PAUSE_GAME =                    0x3803,
        ENTER_MENU =                    0x3805,
        QUIT_TO_MENU =                  0x3806,
        UNK66 =                         0x4001,
        UNK86 =                         0x4008,
        UNK85 =                         0x4009,
        UNK24 =                         0x400a,
        UNK34 =                         0x400b,
        UNK39 =                         0x400c,
        UNK35 =                         0x400d,
        UNK106 =                        0x400f,
        UNK53 =                         0x4010,
        UNK54 =                         0x4011,
        UNK105 =                        0x4012,
        UNK10 =                         0x4013,
        UNK13 =                         0x4014,
        UNK131 =                        0x4015,
        UNK113 =                        0x4017,
        UNK130 =                        0x4018,
        UNK69 =                         0x4019,
        UNK12 =                         0x401a,
        UNK90 =                         0x401b,
        UNK89 =                         0x401c,
        UNK91 =                         0x401d,
        UNK49 =                         0x401f,
        UNK103 =                        0x4020,
        UNK48 =                         0x4021,
        UNK102 =                        0x4022,
        UNK98 =                         0x4023,
        UNK95 =                         0x4024,
        UNK94 =                         0x4026,
        UNK84 =                         0x4027,
        UNK80 =                         0x4028,
        UNK79 =                         0x402a,
        UNK46 =                         0x402b,
        UNK47 =                         0x402c,
        UNK93 =                         0x402d,
        UNK43 =                         0x402e,
        UNK14 =                         0x4801,
        CONTRABAND_IMPACT_NOTBOMB_UNK = 0x4802,
        UNK64 =                         0x4803,
        UNK9 =                          0x4804,
        UNK8 =                          0x4805,
        UNK36 =                         0x4806,
        UNK115 =                        0x4807,
        UNK15 =                         0x4808,
        UNK63 =                         0x4809,
        UNK4 =                          0x480a,
        UNK6 =                          0x480b,
        UNK1 =                          0x480c,
        UNK3 =                          0x480d,
        UNK62 =                         0x480e,
        UNK37 =                         0x480f,
        MISSION_TIME_RUNNING_OUT =      0x4810,
        MISSION_TIME_EXPIRED =          0x4811,
        UNK21 =                         0x4812,
        UNK22 =                         0x4813,
        UNK23 =                         0x4814,
        UNK17 =                         0x4815,
        UNK18 =                         0x4816,
        UNK70 =                         0x4817,
        UNK11 =                         0x4818,
        CONTRABAND_IMPACT_BOMB_UNK =    0x4819,
        UNK7 =                          0x481a,
        UNK61 =                         0x481b,
        UNK16 =                         0x481c,
        UNK71 =                         0x481d,
        UNK25 =                         0x5001,
        UNK55 =                         0x5002,
        UNK56 =                         0x5003,
        UNK57 =                         0x5004,
        UNK58 =                         0x5005,
        UNK59 =                         0x5006,
        UNK60 =                         0x5007,
        UNK112 =                        0x5008,
        UNK104 =                        0x5009,
        UNK100 =                        0x500a,
        UNK99 =                         0x500c,
        FOLLOW_TOO_FAR =                0x500d,
        UNK81 =                         0x500e,
        UNK82 =                         0x500f,
        UNK38 =                         0x5010,
        UNK97 =                         0x5011,
        UNK5 =                          0x5012,
        UNK83 =                         0x5014,
        UNK114 =                        0x5015,
        UNK101 =                        0x5016,
        FOLLOW_TOO_CLOSE =              0x5017,
        UNK26 =                         0x5018,
        UNK27 =                         0x5019,
        UNK28 =                         0x501a,
        UNK29 =                         0x501b,
        UNK30 =                         0x501c,
        UNK31 =                         0x501d,
        UNK32 =                         0x501e,
        UNK33 =                         0x501f
    };

    struct msgMessage {
        MSG_TYPE type;
        u8 flags;
        u16 size;
    };

    class msgMsgSource {
        public:
            static void sendBroadcast(MSG_TYPE msg);
            static void sendBroadcast(MSG_TYPE msg, u32 data);
            static void sendBroadcast(MSG_TYPE msg, char* data);
            static void send(MSG_TYPE msg, u8 flags);
            static void sendGlobal(MSG_TYPE msg, u8 flags, u32 data);
            static void sendGlobal(MSG_TYPE msg, u8 flags, char* data);
            static void packSend(MSG_TYPE msg, u8 flags, const char* fmt, ...);
            static void send(MSG_TYPE msg, u8 flags, u8 sizeInWords, void* data);
            static void dispatch();
            static void registerSink(msgMsgSink* sink, u32 mask, u8 target);
            static void unregisterSink(msgMsgSink* sink);
            static void unregisterAll();
            static void unpack(msgMessage* msg, const char* fmt, ...);

        protected:
            static u8 sendIdx;
            static u32 sendTop;
            static u32 sinkCount;
            static msgMsgSink* sinks[32];
            static u32 masks[32];
            static u8 targets[32];
            static u8 buffer[2][4096];
    };
};