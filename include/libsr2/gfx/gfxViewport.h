#pragma once
#include <libsr2/types.h>
#include <libsr2/math/vec4f.h>
#include <libsr2/math/vec2i.h>
#include <libsr2/math/mat4x4f.h>

namespace sr2 {
    namespace gfx {
        class gfxViewport {
            public:
                gfxViewport();
                ~gfxViewport();

                // 0x002ee290
                void DoFlush();

                i32 ref_count;
                gfxViewport* next;
                undefined field_0x8;
                undefined field_0x9;
                undefined field_0xa;
                undefined field_0xb;
                undefined field_0xc;
                undefined field_0xd;
                undefined field_0xe;
                undefined field_0xf;
                mat4x4f projection;
                mat4x4f window;
                mat4x4f view;
                mat4x4f world;
                vec3f clip_volume;
                f32 field_0x11c;
                vec4f window_w;
                u32 field_0x130;
                f32 backup_aspect_ratio;
                u32 field_0x138;
                f32 near;
                f32 far;
                f32 tan_half_fov_times_aspect;
                f32 tan_half_fov;
                vec2f f[4];
                vec2i origin;
                vec2i size;
                f32 ortho_far;
                f32 ortho_near;
                undefined field_0x184;
                undefined field_0x185;
                undefined field_0x186;
                undefined field_0x187;
                u64 field_0x188;
                f32 field_0x190;
                f32 field_of_view; /* degrees */
                f32 aspect_ratio;
                undefined field_0x19c;
                undefined field_0x19d;
                undefined field_0x19e;
                undefined field_0x19f;

                static gfxViewport* First;
        };
    };
};