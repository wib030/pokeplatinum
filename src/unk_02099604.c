#include <nitro.h>
#include <string.h>

#include "struct_decls/struct_0202B628_decl.h"
#include "savedata.h"

#include "field/field_system.h"
#include "overlay082/struct_ov82_0223B164.h"

#include "heap.h"
#include "unk_02025E08.h"
#include "poffin.h"
#include "field_system.h"
#include "unk_0207D3B8.h"
#include "overlay082/ov82_0223B140.h"

FS_EXTERN_OVERLAY(overlay82);

UnkStruct_ov82_0223B164 * sub_02099604(int param0, SaveData * param1, int param2, UnkStruct_0202B628 * param3);
UnkStruct_ov82_0223B164 * sub_02099674(FieldSystem * fieldSystem, int param1, int param2);
UnkStruct_ov82_0223B164 * sub_020996A0(FieldSystem * fieldSystem, int param1, BOOL param2);

UnkStruct_ov82_0223B164 * sub_02099604 (int param0, SaveData * param1, int param2, UnkStruct_0202B628 * param3)
{
    UnkStruct_ov82_0223B164 * v0;

    v0 = Heap_AllocFromHeap(param2, sizeof(UnkStruct_ov82_0223B164));
    MI_CpuClear8(v0, sizeof(UnkStruct_ov82_0223B164));

    v0->unk_0C = param1;
    v0->unk_08 = Poffin_GetSavedataBlock(param1);
    v0->unk_10 = SaveData_GetTrainerInfo(param1);
    v0->unk_14 = sub_0207D990(param1);
    v0->unk_18 = SaveData_Options(param1);
    v0->unk_1C = param3;
    v0->unk_20 = 0;
    v0->unk_04 = param0;

    return v0;
}

static void sub_0209964C (FieldSystem * fieldSystem, UnkStruct_ov82_0223B164 * param1)
{
    FS_EXTERN_OVERLAY(overlay82);

    {
        OverlayManagerTemplate v0 = {
            ov82_0223B164,
            ov82_0223B1D4,
            ov82_0223B24C,
            FS_OVERLAY_ID(overlay82)
        };

        sub_0203CD84(fieldSystem, &v0, param1);
    }
}

UnkStruct_ov82_0223B164 * sub_02099674 (FieldSystem * fieldSystem, int param1, int param2)
{
    UnkStruct_ov82_0223B164 * v0;

    v0 = sub_02099604(param1, FieldSystem_SaveData(fieldSystem), param2, fieldSystem->unk_9C);
    sub_0209964C(fieldSystem, v0);

    return v0;
}

UnkStruct_ov82_0223B164 * sub_020996A0 (FieldSystem * fieldSystem, int param1, BOOL param2)
{
    UnkStruct_ov82_0223B164 * v0;

    v0 = sub_02099604(1, FieldSystem_SaveData(fieldSystem), param1, fieldSystem->unk_9C);
    v0->unk_20 = 1;
    v0->unk_24 = param2;

    sub_0209964C(fieldSystem, v0);

    return v0;
}
