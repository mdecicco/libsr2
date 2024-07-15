#include <libsr2/frontend/srfeMovieBase.h>
#include <libsr2/ui/ui2Base.h>

namespace sr2 {
    srfeMovieBase::srfeMovieBase(const char* name, const WidgetRef<ui2Master>& master) : srui2SRScreen(name, master) {
    }

    srfeMovieBase::~srfeMovieBase() {
    }

    bool srfeMovieBase::isWidgetAMovie(const char* someWidgetName) {
        ui::BaseRef w = ui2Base::getGlobalMaster()->findWidget(someWidgetName, "srfeMovieBase");
        if (!w) return false;

        return true;
    }
};