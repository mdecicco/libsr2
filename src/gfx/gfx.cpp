#include <libsr2/gfx/gfx.h>

namespace sr2 {
    namespace gfx {
        gfxViewport* g_SomeViewport = nullptr;

        namespace pipeline {
            gfxViewport* Viewport;

            void ForceSetViewport(gfxViewport* vp) {
                // todo
                Viewport = vp;
            }
        };
    };
};