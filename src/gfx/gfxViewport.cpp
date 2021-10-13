#include <libsr2/gfx/gfxViewport.h>

namespace sr2 {
    namespace gfx {
        gfxViewport* gfxViewport::First = nullptr;

        gfxViewport::gfxViewport() {
            ref_count = 1;

            view.x.x = 1.0f;
            view.x.w = 0.0f;
            view.x.y = 0.0f;
            view.x.z = 0.0f;
            view.y.x = 0.0f;
            view.y.w = 0.0f;
            view.y.y = 1.0f;
            view.y.z = 0.0f;
            view.z.x = 0.0f;
            view.z.w = 0.0f;
            view.z.y = 0.0f;
            view.z.z = 1.0f;
            view.w.x = 0.0f;
            view.w.w = 1.0f;
            view.w.y = 0.0f;
            view.w.z = 0.0f;
            world.x.x = 1.0f;
            world.x.w = 0.0f;
            world.x.y = 0.0f;
            world.x.z = 0.0f;
            world.y.x = 0.0f;
            world.y.w = 0.0f;
            world.y.y = 1.0f;
            world.y.z = 0.0f;
            world.z.x = 0.0f;
            world.z.w = 0.0f;
            world.z.y = 0.0f;
            world.z.z = 1.0f;
            world.w.x = 0.0f;
            world.w.y = 0.0f;
            world.w.z = 0.0f;
            world.w.w = 1.0f;

            field_0x138 = 2;
            aspect_ratio = 0.0f;
            field_0x190 = 0.0f;
            field_of_view = 60.0f;
            near = 1.0f;
            far = 1000.0f;

            /*
                datCallback aiStack80;
    
                uVar3 = SEXT48((int)&world);
                if (!First) {
                    Utils::datCallback::datCallback(&aiStack80,gfxTexture::KillAll);
                    Utils::datCallbackList::Add(&PipeResetList,&aiStack80,uVar3);
                }
            */
            next = First;
            First = this;

            // ResetWindow(this);
        }

        gfxViewport::~gfxViewport() {
        }

        void gfxViewport::DoFlush() {
        }
    };
};