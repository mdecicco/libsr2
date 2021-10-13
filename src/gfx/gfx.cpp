#include <libsr2/gfx/gfx.h>
#include <libsr2/gfx/gfxViewport.h>
#include <libsr2/math/mat4x4f.h>

namespace sr2 {
    namespace gfx {
        gfxViewport* g_SomeViewport = nullptr;

        namespace pipeline {
            gfxViewport* Viewport;

            void ForceSetViewport(gfxViewport* vp) {
                if (Viewport) {
                    Viewport->view.x.x = state::Camera.x.x;
                    Viewport->view.x.y = state::Camera.x.y;
                    Viewport->view.x.z = state::Camera.x.z;
                    Viewport->view.x.w = state::Camera.x.w;
                    Viewport->view.y.x = state::Camera.y.x;
                    Viewport->view.y.y = state::Camera.y.y;
                    Viewport->view.y.z = state::Camera.y.z;
                    Viewport->view.y.w = state::Camera.y.w;
                    Viewport->view.z.x = state::Camera.z.x;
                    Viewport->view.z.y = state::Camera.z.y;
                    Viewport->view.z.z = state::Camera.z.z;
                    Viewport->view.z.w = state::Camera.z.w;
                    Viewport->view.w.x = state::Camera.w.x;
                    Viewport->view.w.y = state::Camera.w.y;
                    Viewport->view.w.z = state::Camera.w.z;
                    Viewport->view.w.w = state::Camera.w.w;

                    (Viewport->world).x.x = state::World.x.x;
                    (Viewport->world).x.y = state::World.x.y;
                    (Viewport->world).x.z = state::World.x.z;
                    (Viewport->world).x.w = state::World.x.w;
                    (Viewport->world).y.x = state::World.y.x;
                    (Viewport->world).y.y = state::World.y.y;
                    (Viewport->world).y.z = state::World.y.z;
                    (Viewport->world).y.w = state::World.y.w;
                    (Viewport->world).z.x = state::World.z.x;
                    (Viewport->world).z.y = state::World.z.y;
                    (Viewport->world).z.z = state::World.z.z;
                    (Viewport->world).z.w = state::World.z.w;
                    (Viewport->world).w.x = state::World.w.x;
                    (Viewport->world).w.y = state::World.w.y;
                    (Viewport->world).w.z = state::World.w.z;
                    (Viewport->world).w.w = state::World.w.w;
                }
                Viewport = vp;
                vp->field_0x130 = 0xffffffff;
                vp->DoFlush();
                state::SetCamera(Viewport->view);
                state::World.x.x = (Viewport->world).x.x;
                state::World.x.y = (Viewport->world).x.y;
                state::World.x.z = (Viewport->world).x.z;
                state::World.x.w = (Viewport->world).x.w;
                state::World.y.x = (Viewport->world).y.x;
                state::World.y.y = (Viewport->world).y.y;
                state::World.y.z = (Viewport->world).y.z;
                state::World.y.w = (Viewport->world).y.w;
                state::World.z.x = (Viewport->world).z.x;
                state::World.z.y = (Viewport->world).z.y;
                state::World.z.z = (Viewport->world).z.z;
                state::World.z.w = (Viewport->world).z.w;
                state::World.w.x = (Viewport->world).w.x;
                state::World.w.y = (Viewport->world).w.y;
                state::World.w.z = (Viewport->world).w.z;
                state::World.w.w = (Viewport->world).w.w;
                state::Touched |= 0x90;
                lowPsxGfx::UpdateMatrix();
            }
        };

        namespace state {
            mat4x4f Camera;
            mat4x4f World;
            mat4x4f View;
            mat4x4f ScaleZ;
            mat4x4f Composite;
            u32 Touched;

            void SetCamera(const mat4x4f& cam) {
                Camera.x.x = cam.x.x;
                Camera.x.y = cam.y.y;
                Camera.x.z = cam.x.z;
                Camera.x.w = cam.x.w;

                Camera.y.x = cam.y.x;
                Camera.y.y = cam.y.y;
                Camera.y.z = cam.y.z;
                Camera.y.w = cam.y.w;

                Camera.z.x = cam.z.x;
                Camera.z.y = cam.z.y;
                Camera.z.z = cam.z.z;
                Camera.z.w = cam.z.w;

                Camera.w.x = cam.w.x;
                Camera.w.y = cam.w.y;
                Camera.w.z = cam.w.z;
                Camera.w.w = cam.w.w;

                math::FastInverse(View, Camera);
                math::Dot(View, ScaleZ);
                math::Dot(Composite, pipeline::Viewport->projection, View);

                lowPsxGfx::AddScreenMatrix(Composite);
                Touched = Touched | 0x80;
            }
        };

        namespace lowPsxGfx {
            void UpdateMatrix() {
            }

            void AddScreenMatrix(const mat4x4f& mat) {
            }
        };
    };
};