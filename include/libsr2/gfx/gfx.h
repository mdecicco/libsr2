#pragma once
#include <libsr2/types.h>

namespace sr2 {
    namespace gfx {
        class gfxViewport;
        extern gfxViewport* g_SomeViewport;
        extern u32 g_clearColor;
        extern f32 g_HostTime; // ms
        extern f32 g_DrawTime; // ms
        extern f32 g_FrameTime; // ms

        namespace pipeline {
            typedef void (*CopyToFrontFuncTp)(gfxViewport*);
            extern gfxViewport* Viewport;
            extern gfxViewport* OrthoVP;
            extern bool NeedBufferSwap;
            extern bool DoSwapBuffers;
            extern bool CopyToFront;
            extern u32 FrameNumber;
            extern CopyToFrontFuncTp CopyToFrontFunc;
            extern f32 FadeValue;
            extern f32 FadeStep;
            extern f32 Fade;
            extern f32 FadeStart;
            extern f32 FadeEnd;
            extern f32 fWidth;
            extern f32 fHeight;
            extern i32 iWidth;
            extern i32 iHeight;

            // 0x002e3f20
            void ForceSetViewport(gfxViewport* viewport);

            // 0x002e6cd8
            void Manage();

            // 0x002e60b8
            void BeginFrame();

            // 0x002e6688
            void EndFrame();

            // 0x002e3d10
            void UpdateFade();

            // 0x002e6f20
            void Clear(u32 unk0, u32 clear_color, f32 unk1, u32 unk2);
        };

        namespace state {
            extern mat4x4f Camera;
            extern mat4x4f World;
            extern mat4x4f View;
            extern mat4x4f ScaleZ;
            extern mat4x4f Composite;
            extern u32 Touched;
            extern u32 TouchedMask;

            // 0x002efdc8
            void SetCamera(const mat4x4f& cam);
        };

        namespace lowPsxGfx {
            // 0x002eb780
            void UpdateMatrix();

            // 0x002eb4e0
            void AddScreenMatrix(const mat4x4f& mat);
        };
    };
};