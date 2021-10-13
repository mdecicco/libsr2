namespace sr2 {
    namespace gfx {
        class gfxViewport;
        extern gfxViewport* g_SomeViewport;

        namespace pipeline {
            extern gfxViewport* Viewport;

            // 0x002e3f20
            void ForceSetViewport(gfxViewport* viewport);
        };
    };
};