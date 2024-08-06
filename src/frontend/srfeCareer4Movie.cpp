#include <libsr2/frontend/srfeCareer4Movie.h>

namespace sr2 {
    srfeCareer4Movie::srfeCareer4Movie(const char* name, const WidgetRef<ui2Master>& master) : srfeMovieBase(name, master) {
    }

    srfeCareer4Movie::~srfeCareer4Movie() {
    }

    void srfeCareer4Movie::initScreen() {
        srfeMovieBase::initScreen();
        m_movie->setFilename("cuts04.imf");
        loadSubtitles("cuts04", 64, 0x80c8c8c8);
        set0xb8(3.0f);
        setNextScreenName("MissionSelect");
    }
};