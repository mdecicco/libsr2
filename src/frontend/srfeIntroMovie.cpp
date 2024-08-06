#include <libsr2/frontend/srfeIntroMovie.h>

namespace sr2 {
    srfeIntroMovie::srfeIntroMovie(const char* name, const WidgetRef<ui2Master>& master) : srfeMovieBase(name, master) {
    }

    srfeIntroMovie::~srfeIntroMovie() {
    }

    void srfeIntroMovie::initScreen() {
        srfeMovieBase::initScreen();
        m_movie->setFilename("intro.imf");
        loadSubtitles("intro", 400, 0x8032c832);
        setNextScreenName("Titlescreen");
    }
};