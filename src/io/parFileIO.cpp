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
        datParser parser(getFileType());
        prepParser(&parser);
        beforeSave();
        return parser.save(getDirectory(), m_name, getFileType());
    }

    bool parFileIO::load() {
        datParser parser(getFileType());
        prepParser(&parser);
        bool status = parser.load(getDirectory(), m_name, getFileType());
        if (status) afterLoad();
        return status;
    }

    void parFileIO::prepParser(datParser* parser) {
    }

    void parFileIO::afterLoad() {
    }

    void parFileIO::beforeSave() {
    }

    const char* parFileIO::getDirectory() {
        return "tune";
    }

    const char* parFileIO::getFileType() {
        return "parFileIO";
    }

    void parFileIO::setName(const char* name) {
        if (m_name) delete [] m_name;
        m_name = duplicate_string(name);
    }
};