#include <nitro.h>
#include <string.h>

#include "core_sys.h"

#include "overlay099/struct_ov99_021D2CB0.h"
#include "overlay099/struct_ov99_021D3A40.h"
#include "overlay099/struct_ov99_021D3DE0.h"

#include "unk_0200F174.h"
#include "overlay099/ov99_021D3DE0.h"

BOOL ov99_021D3DE0 (UnkStruct_ov99_021D2CB0 * param0, UnkStruct_ov99_021D3A40 * param1)
{
    UnkStruct_ov99_021D3DE0 * v0 = &param1->unk_08_val6;

    switch (param1->unk_00) {
    case 0:
        sub_0200F174(0, 1, 1, 0x0, 30, 1, 75);
        param1->unk_00++;
        break;
    case 1:
        if (ScreenWipe_Done() == 1) {
            if ((param0->unk_10FC > 10080) || (gCoreSys.pressedKeys & PAD_BUTTON_A) || (gCoreSys.pressedKeys & PAD_BUTTON_START)) {
                sub_0200F174(0, 0, 0, 0x0, 45, 1, 75);
                param1->unk_00++;
            }
        }
        break;
    case 2:
        if (ScreenWipe_Done() == 1) {
            return 1;
        }
        break;
    }

    return 0;
}
