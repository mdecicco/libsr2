#include <libsr2/frontend/srfeCareer5Movie.h>

namespace sr2 {
    srfeCareer5Movie::srfeCareer5Movie(const char* name, const WidgetRef<ui2Master>& master) : srfeMovieBase(name, master) {
    }

    srfeCareer5Movie::~srfeCareer5Movie() {
    }

    void srfeCareer5Movie::initScreen() {
        srfeMovieBase::initScreen();
        m_movie->setFilename("cuts05.imf");
        loadSubtitles("cuts05", 64, 0x80c8c8c8);
        set0xb8(3.0f);
        setNextScreenName("MissionSelect");
    }
};