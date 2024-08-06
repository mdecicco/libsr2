#include <libsr2/frontend/srfeCareer6Movie.h>

namespace sr2 {
    srfeCareer6Movie::srfeCareer6Movie(const char* name, const WidgetRef<ui2Master>& master) : srfeMovieBase(name, master) {
    }

    srfeCareer6Movie::~srfeCareer6Movie() {
    }

    void srfeCareer6Movie::initScreen() {
        srfeMovieBase::initScreen();
        m_movie->setFilename("cuts06.imf");
        loadSubtitles("cuts06", 64, 0x80c8c8c8);
        set0xb8(3.0f);
        setNextScreenName("MissionSelect");
    }
};