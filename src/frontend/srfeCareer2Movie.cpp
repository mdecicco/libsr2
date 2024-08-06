#include <libsr2/frontend/srfeCareer2Movie.h>

namespace sr2 {
    srfeCareer2Movie::srfeCareer2Movie(const char* name, const WidgetRef<ui2Master>& master) : srfeMovieBase(name, master) {
    }

    srfeCareer2Movie::~srfeCareer2Movie() {
    }

    void srfeCareer2Movie::initScreen() {
        srfeMovieBase::initScreen();
        m_movie->setFilename("cuts02.imf");
        loadSubtitles("cuts02", 64, 0x80c8c8c8);
        set0xb8(3.0f);
        setNextScreenName("MissionSelect");
    }
};