#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class Data {
        public:
            Data(u8* ptr, u64 size, bool copyData = true);
            Data(const char* srcFilePath);
            ~Data();

            void read(void* dest, u64 sz);
            u16 read_str(char* str, u16 bufSz);
            u64 size() const;
            void position(u64 pos);
            u64 position() const;
            Data* isolate(u64 sz, bool copy = true);
            void* raw();
            void take_ownership();

            template <typename t>
            void read(t& o) {
                read(&o, sizeof(t));
            }

            template <typename t>
            t read() {
                t o;
                read(&o, sizeof(t));
                return o;
            }

        protected:
            u8* m_data;
            u64 m_size;
            u64 m_position;
            bool m_ownsData;
    };
};