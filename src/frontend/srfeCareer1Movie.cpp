#include <libsr2/frontend/srfeCareer1Movie.h>

namespace sr2 {
    srfeCareer1Movie::srfeCareer1Movie(const char* name, const WidgetRef<ui2Master>& master) : srfeMovieBase(name, master) {
    }

    srfeCareer1Movie::~srfeCareer1Movie() {
    }

    void srfeCareer1Movie::initScreen() {
        srfeMovieBase::initScreen();
        m_movie->setFilename("cuts01.imf");
        loadSubtitles("cuts01", 64, 0x80c8c8c8);
        set0xb8(3.0f);
        setNextScreenName("");
    }
};