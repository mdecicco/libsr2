#include <libsr2/frontend/srfeUIIntroMovie.h>

namespace sr2 {
    srfeUIIntroMovie::srfeUIIntroMovie(const char* name, const WidgetRef<ui2Master>& master) : srfeMovieBase(name, master) {
    }

    srfeUIIntroMovie::~srfeUIIntroMovie() {
    }

    void srfeUIIntroMovie::initScreen() {
        srfeMovieBase::initScreen();
        m_movie->setFilename("uiintro.imf");
        setNextScreenName("MainMenu");
    }
};