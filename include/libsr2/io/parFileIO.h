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
            virtual datParserNode* prepParser(datParser* parser);
            virtual void afterLoad();
            virtual void beforeSave();
            virtual const char* getDirectory();
            virtual const char* getFileType();

            void setName(const char* name);

        protected:
            char* m_name;
            u32 m_flags;
    };
};