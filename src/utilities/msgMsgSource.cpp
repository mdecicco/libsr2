#include <libsr2/utilities/msgMsgSource.h>
#include <libsr2/utilities/msgMsgSink.h>

#include <string.h>
#include <stdarg.h>

namespace sr2 {
    u8 msgMsgSource::sendIdx = 0;
    u32 msgMsgSource::sendTop = 0;
    u32 msgMsgSource::sinkCount = 0;
    msgMsgSink* msgMsgSource::sinks[32] = { 0 };
    u32 msgMsgSource::masks[32] = { 0 };
    u8 msgMsgSource::targets[32] = { 0 };
    u8 msgMsgSource::buffer[2][4096] = { { 0 }, { 0 } };

    void msgMsgSource::sendBroadcast(MSG_TYPE msg) {
        send(msg, 0x80, 0, nullptr);
    }
    
    void msgMsgSource::sendBroadcast(MSG_TYPE msg, u32 data) {
        send(msg, 0x80, 1, &data);
    }
    
    void msgMsgSource::sendBroadcast(MSG_TYPE msg, char* data) {
        send(msg, 0x80, (strlen(data) + 4) / 4, data);
    }

    void msgMsgSource::send(MSG_TYPE msg, u8 flags) {
        send(msg, flags, 0, nullptr);
    }
    
    void msgMsgSource::sendGlobal(MSG_TYPE msg, u8 flags, u32 data) {
        send(msg, flags, 1, &data);
    }
    
    void msgMsgSource::sendGlobal(MSG_TYPE msg, u8 flags, char* data) {
        send(msg, flags, (strlen(data) + 4) / 4, data);
    }
    
    void msgMsgSource::packSend(MSG_TYPE msg, u8 flags, const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);

        u8* dst = (&buffer[sendIdx][sendTop]) + sizeof(msgMessage);
        u8 sizeInWords = 0;

        while (*fmt != 0) {
            if (*fmt == 'i') {
                *((u32*)dst) = va_arg(args, u32);
                dst += sizeof(u32);
                sizeInWords += 1;
            } else if (*fmt == 's') {
                char* data = va_arg(args, char*);
                size_t len = strlen(data);
                memcpy(dst, data, len);

                u32 szWords = (len + 4) / 4;
                dst += szWords * 4;
                sizeInWords += szWords;
            }

            fmt++;
        }

        va_end(args);

        send(msg, flags, sizeInWords, nullptr);
    }

    void msgMsgSource::send(MSG_TYPE msg, u8 flags, u8 sizeInWords, void* data) {
        msgMessage* head = (msgMessage*)(&buffer[sendIdx][sendTop]);
        u8* body = ((u8*)head) + sizeof(msgMessage);

        head->type = msg;
        head->flags = flags;
        head->size = sizeInWords * sizeof(u32);

        if (sizeInWords > 0 && data) memcpy(body, data, sizeInWords * sizeof(u32));
        sendTop += sizeof(msgMessage) + head->size;

        if (flags & 0x40) dispatch();
    }

    void msgMsgSource::dispatch() {
        u32 off = 0;
        while (off < sendTop) {
            msgMessage* msg = (msgMessage*)&buffer[sendIdx][off];

            for (u32 s = 0;s < sinkCount;s++) {
                if (!(masks[s] & 1 << (u16(msg->type) >> 0xb))) continue;
                if (((msg->flags & 0x1f) != (targets[s] & 0x1f)) && !(targets[s] & 0x80)) continue;

                sinks[s]->onMessage(msg);
            }

            off += msg->size;
        }

        sendIdx ^= 1;
        sendTop = 0;
    }
    
    void msgMsgSource::registerSink(msgMsgSink* sink, u32 mask, u8 target) {
        sinks[sinkCount] = sink;
        masks[sinkCount] = mask;
        targets[sinkCount] = target;
        sinkCount++;
    }
    
    void msgMsgSource::unregisterSink(msgMsgSink* sink) {
        bool found = false;
        for (u32 i = 0;i < sinkCount;i++) {
            if (sinks[i] == sink) {
                found = true;
                sinkCount--;
                if (i == sinkCount) break;
            }
            if (!found) continue;

            sinks[i] = sinks[i + 1];
            masks[i] = masks[i + 1];
            targets[i] = targets[i + 1];
        }
    }
    
    void msgMsgSource::unregisterAll() {
        for (u32 i = 0;i < 32;i++) {
            sinks[i] = nullptr;
            masks[i] = 0;
            targets[i] = 0;
        }

        sinkCount = 0;
        sendTop = 0;
    }

    void msgMsgSource::unpack(msgMessage* msg, const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);

        const u8* src = (&buffer[sendIdx][sendTop]) + sizeof(msgMessage);
        u8 sizeInWords = 0;

        while (*fmt != 0) {
            if (*fmt == 'i') {
                u32* dst = va_arg(args, u32*);
                *dst = *((u32*)src);
                src += sizeof(u32);
                sizeInWords += 1;
            } else if (*fmt == 's') {
                char* dst = va_arg(args, char*);

                size_t len = strlen((const char*)src);
                memcpy(dst, src, len);

                u32 szWords = (len + 4) / 4;
                src += szWords * 4;
                sizeInWords += szWords;
            }

            fmt++;
        }

        va_end(args);
    }
};