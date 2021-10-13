#pragma once
#include <libsr2/types.h>

namespace sr2 {
    namespace gfx {
        class gfxViewport;
        extern gfxViewport* g_SomeViewport;

        namespace pipeline {
            extern gfxViewport* Viewport;

            // 0x002e3f20
            void ForceSetViewport(gfxViewport* viewport);
        };

        namespace state {
            extern mat4x4f Camera;
            extern mat4x4f World;
            extern mat4x4f View;
            extern mat4x4f ScaleZ;
            extern mat4x4f Composite;
            extern u32 Touched;

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