#include <libsr2/io/parFileIO.h>
#include <libsr2/io/datParser.h>
#include <libsr2/utilities/utils.h>

namespace sr2 {
    parFileIO::parFileIO() {
        m_name = nullptr;
        m_flags = 0;
    }

    parFileIO::~parFileIO() {
        if (m_name) delete [] m_name;
        m_name = nullptr;
    }

    bool parFileIO::save() {
        datParser parser(file_type());
        prep_parser(&parser);
        before_save();
        return parser.save(directory(), m_name, file_type());
    }

    bool parFileIO::load() {
        datParser parser(file_type());
        prep_parser(&parser);
        bool status = parser.load(directory(), m_name, file_type());
        if (status) after_load();
        return status;
    }

    datParserNode* parFileIO::prep_parser(datParser* parser) {
        return nullptr;
    }

    void parFileIO::after_load() {
    }

    void parFileIO::before_save() {
    }

    const char* parFileIO::directory() {
        return "tune";
    }

    const char* parFileIO::file_type() {
        return "parFileIO";
    }

    void parFileIO::set_name(const char* name) {
        if (m_name) delete [] m_name;
        m_name = duplicate_string(name);
    }
};