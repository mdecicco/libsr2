#include <libsr2/frontend/srfeCareer7Movie.h>

namespace sr2 {
    srfeCareer7Movie::srfeCareer7Movie(const char* name, const WidgetRef<ui2Master>& master) : srfeMovieBase(name, master) {
    }

    srfeCareer7Movie::~srfeCareer7Movie() {
    }

    void srfeCareer7Movie::initScreen() {
        srfeMovieBase::initScreen();
        m_movie->setFilename("cuts07.imf");
        loadSubtitles("cuts07", 64, 0x80c8c8c8);
        set0xb8(3.0f);
        setNextScreenName("MissionSelect");
    }
};