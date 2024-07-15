#pragma once
#include <libsr2/types.h>

namespace sr2 {
    struct UnkButtonStruct0 {
        Button btn;
        u32 padUnkIdx;
    };

    constexpr float debug_ui_font_size = 14.0f;
    constexpr float debug_ui_scale = 1.8f;

    static undefined4 DAT_00362d7c;
    static f32 FLOAT_00362d84;
    static f32 FLOAT_00362d88;
    static f32 FLOAT_00362d90;
    static f32 FLOAT_00362d8c;
    static undefined4 DAT_00362d14;
    static undefined4 DAT_00362d78;
    static undefined4 DAT_00362d80;
    static f32 FLOAT_00362dba;
    static f32 FLOAT_00362dbe;

    static undefined4 DAT_003b11a0;
    static undefined4 DAT_003b11b4;
    static undefined4 DAT_003b11c8;

    static UnkButtonStruct0 DAT_008d5380[12];
    static KEY_CODE uiKeyboardActionMap[8];
    static Button uiControllerActionMap[8];
}