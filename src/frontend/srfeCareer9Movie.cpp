#include <libsr2/frontend/srfeCareer9Movie.h>
#include <libsr2/frontend/srfeCredits.h>
#include <libsr2/ui/ui2Base.h>
#include <libsr2/ui/ui2Master.h>

namespace sr2 {
    srfeCareer9Movie::srfeCareer9Movie(const char* name, const WidgetRef<ui2Master>& master) : srfeMovieBase(name, master) {
    }

    srfeCareer9Movie::~srfeCareer9Movie() {
    }

    void srfeCareer9Movie::initScreen() {
        srfeMovieBase::initScreen();
        m_movie->setFilename("cuts09.imf");
        loadSubtitles("cuts09", 64, 0x80c8c8c8);
        set0xb8(3.0f);
        setNextScreenName("Credits");

        WidgetRef<srfeCredits> credits = ui2Base::getGlobalMaster()->findWidget("Credits", "srfeCredits").cast<srfeCredits>();
        if (!credits) return;

        credits->FUN_0011c260();
    }
};