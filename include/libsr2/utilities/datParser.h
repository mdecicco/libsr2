#pragma once
#include <libsr2/utilities/parser.h>

namespace sr2 {
    class Data;

    class datParser : public Parser {
        public:
            datParser();
            ~datParser();

            virtual bool deserialize(Data* src);
            virtual bool serialize(Data* dst);
    };
};