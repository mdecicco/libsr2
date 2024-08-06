#include <libsr2/frontend/srfeCareer3Movie.h>

namespace sr2 {
    srfeCareer3Movie::srfeCareer3Movie(const char* name, const WidgetRef<ui2Master>& master) : srfeMovieBase(name, master) {
    }

    srfeCareer3Movie::~srfeCareer3Movie() {
    }

    void srfeCareer3Movie::initScreen() {
        srfeMovieBase::initScreen();
        m_movie->setFilename("cuts03.imf");
        loadSubtitles("cuts03", 64, 0x80c8c8c8);
        set0xb8(3.0f);
        setNextScreenName("MissionSelect");
    }
};