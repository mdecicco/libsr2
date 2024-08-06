#include <libsr2/frontend/srfeCareer8Movie.h>

namespace sr2 {
    srfeCareer8Movie::srfeCareer8Movie(const char* name, const WidgetRef<ui2Master>& master) : srfeMovieBase(name, master) {
    }

    srfeCareer8Movie::~srfeCareer8Movie() {
    }

    void srfeCareer8Movie::initScreen() {
        srfeMovieBase::initScreen();
        m_movie->setFilename("cuts08.imf");
        loadSubtitles("cuts08", 64, 0x80c8c8c8);
        set0xb8(3.0f);
        setNextScreenName("MissionSelect");
    }
};