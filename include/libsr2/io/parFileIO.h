#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class datParser;
    struct datParserNode;

    class parFileIO {
        public:
            parFileIO();
            virtual ~parFileIO();

            virtual bool save();
            virtual bool load();
            virtual datParserNode* prep_parser(datParser* parser);
            virtual void after_load();
            virtual void before_save();
            virtual const char* directory();
            virtual const char* file_type();

            void set_name(const char* name);

        protected:
            char* m_name;
            u32 m_flags;
    };
};