#include <libsr2/audio/audSource.h>
#include <libsr2/utilities/utils.h>

namespace sr2 {
    audSource::audSource(const char* name, i32 maxAudible, i32 priority, i32 index) {
        m_name = duplicateString(name);
        m_maxAudible = maxAudible;
        m_priority = priority;
        m_index = index;
    }

    audSource::~audSource() {
        if (m_name) delete [] m_name;
    }
};