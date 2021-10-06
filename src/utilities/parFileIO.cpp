#include <libsr2/utilities/parFileIO.h>
#include <libsr2/utilities/datParser.h>
#include <libsr2/utilities/GameArchive.h>

#include <exception>
#include <stdio.h>

namespace sr2 {
    parFileIO::parFileIO() {
        filename = nullptr;
        field_0x8 = nullptr;
    }

    parFileIO::~parFileIO() {
        if (filename) {
            delete [] filename;
            filename = nullptr;
        }
    }

    const char* parFileIO::directory() {
        return "tune";
    }

    const char* parFileIO::file_type() {
        return "parFileIO";
    }

    Parser::node* parFileIO::prepare_parser(Parser* p) {
        return nullptr;
    }

    bool parFileIO::load() {
        Data* file = nullptr;
        try {
            file = g_Archives->open(directory(), filename == nullptr ? "default" : filename, file_type());
        } catch (const std::exception& e) {
            printf("%s\n", e.what());
            // throw e
            return false;
        }

        datParser p;
        prepare_parser(&p);
        return p.deserialize(file);
    }

    bool parFileIO::save() {
        // this is implemented in the game, though not currently needed
        return false;
    }
};
