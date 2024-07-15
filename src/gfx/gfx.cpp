#include <libsr2/gfx/gfx.h>
#include <libsr2/gfx/gfxViewport.h>
#include <libsr2/math/mat4x4f.h>
#include <libsr2/utilities/timer.h>
#include <libsr2/managers/datTimeManager.h>
#include <libsr2/io/ioPad.h>
#include <libsr2/io/ioKeyboard.h>
#include <libsr2/globals.h>

namespace sr2 {
    namespace gfx {
        gfxViewport* g_SomeViewport = nullptr;
        u32 g_clearColor = 0;
        f32 g_HostTime = 0.0f;
        f32 g_DrawTime = 0.0f;
        f32 g_FrameTime = 0.0f;

        namespace pipeline {
            gfxViewport* Viewport;
            gfxViewport* OrthoVP;
            u32 FrameNumber = 0;
            bool NeedBufferSwap = false;
            bool DoSwapBuffers = false;
            bool CopyToFront = false;
            bool firstTime = false;
            CopyToFrontFuncTp CopyToFrontFunc = nullptr;
            f32 FadeValue = 0.0f;
            f32 FadeStep = 0.0f;
            f32 Fade = 0.0f;
            f32 FadeStart = 0.0f;
            f32 FadeEnd = 0.0f;

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

            void Manage() {
                DAT_003b11c8 = DAT_003b11b4;
                DAT_003b11b4 = DAT_003b11a0;

                ioKeyboard::Update();
                ioPad::UpdateAll();
                // ioFFWheel::UpdateAll();

                // todo: some profiling stuff
            }

            void BeginFrame() {
                if (!Viewport) return;
                NeedBufferSwap = true;

                // lowPsxGfx::BeginDL();

                gfxViewport* prev = Viewport;
                ForceSetViewport(Viewport);
                if (CopyToFront) CopyToFrontFunc(prev);
                // FUN_002ece48(g_NoTexture);

                if (firstTime == 0) {
                    // RenderState::SetBlendSet(&state::Current, 6, 0x80);
                    // RenderState::SetBlendSet(&state::Current, 0, 0x80);
                    firstTime = 1;
                }
            }

            void EndFrame() {
                UpdateFade();

                if (state::Touched & state::TouchedMask) {
                    // state::DoFlush(state::current, state::last);
                }

                // PS2 Graphics nonsense
                // Probably sending some kind of frame end notification packet to the GS

                // SubmitFrame(1);
            }

            void UpdateFade() {
                FadeValue += FadeStep * datTimeManager::Seconds;
                if (FadeValue > 1.0f) FadeValue = 1.0f;
                Fade = FadeStart + (FadeEnd - FadeStart) * FadeValue;
            }

            void Clear(u32 unk0, u32 clear_color, f32 unk1, u32 unk2) {
                /*
                int iVar1;
                ulong uVar2;
                ulong uVar3;

                iVar1 = Current.field_0x18;
                if (param_1 != 0) {
                    uVar2 = Current.field_0x38 >> 1;
                    uVar3 = (long)(Current.field_0x40 << 0x1b) >> 0x20 & 1;
                    if ((Current.field_0x38 & 0xe) != 2) {
                        Current.field_0x38 = Current.field_0x38 & 0xfffffffffffffff1 | 2;
                        RenderState::Touched |= 1;
                    }
                    RenderState::SetBlendSet(&Current,0,0x80);
                    if (Current.field_0x74 != 0) {
                        Current.field_0x74 = 0;
                        RenderState::Touched |= 1;
                    }
                    if ((Current.field_0x40 & 0x20) != 0) {
                        Current.field_0x40 &= 0xffffffffffffffdf;
                        RenderState::Touched |= 1;
                    }
                    if ((Current.field_0x38 & 0x4000) != 0) {
                        Current.field_0x38 &= 0xffffffffffffbfff;
                        RenderState::Touched |= 1;
                    }
                    if (((param_1 & 2U) != 0) && ((Current.field_0x38 & 0x60000) != 0x20000)) {
                        Current.field_0x38 = Current.field_0x38 & 0xfffffffffff9ffff | 0x20000;
                        RenderState::Touched |= 1;
                    }
                    if (Viewport == NULL) {
                        ClearRect(0,0,iWidth,iHeight,color);
                    }
                    else {
                        ClearRect((Viewport->origin).x,(Viewport->origin).y,(Viewport->size).x,(Viewport->size).y,color);
                    }
                    if ((param_1 & 2U) != 0) {
                        if ((Current.field_0x38 & 0x60000) != 0x40000) {
                            RenderState::Touched |= 1;
                            Current.field_0x38 = Current.field_0x38 & 0xfffffffffff9ffff | 0x40000;
                        }
                        if ((RenderState::Touched & RenderState::TouchedMask) != 0) {
                            RenderState::DoFlush(&Current,&Last);
                        }
                    }
                    uVar2 = (long)(int)((uint)uVar2 & 7) << 1;
                    if ((Current.field_0x38 & 0xe) != uVar2) {
                        Current.field_0x38 = Current.field_0x38 & 0xfffffffffffffff1 | uVar2;
                        RenderState::Touched |= 1;
                    }
                    RenderState::SetBlendSet(&Current,iVar1,0x80);
                    if ((long)Current.field_0x74 != uVar3) {
                        Current.field_0x74 = (int)uVar3;
                        RenderState::Touched |= 1;
                    }
                    if ((Current.field_0x40 & 0x20) != uVar3 << 5) {
                        Current.field_0x40 = Current.field_0x40 & 0xffffffffffffffdf | uVar3 << 5;
                        RenderState::Touched |= 1;
                    }
                }
                */
            }
        };

        namespace state {
            mat4x4f Camera;
            mat4x4f World;
            mat4x4f View;
            mat4x4f ScaleZ;
            mat4x4f Composite;
            u32 Touched;
            u32 TouchedMask;

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
                // PS2 Graphics nonsense
                // Appears to send world matrix to GS
            }

            void AddScreenMatrix(const mat4x4f& mat) {
                // PS2 Graphics nonsense
                // Appears to scale mat by (invScaleClipVolume, invScaleClipVolume, 1.0, 1.0) before sending it to GS
            }
        };
    };
};