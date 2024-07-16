#include <nitro.h>
#include <string.h>

#include "inlines.h"
#include "core_sys.h"

#include "struct_decls/struct_020507E4_decl.h"
#include "struct_decls/struct_020508D4_decl.h"
#include "struct_decls/struct_0205E884_decl.h"
#include "struct_decls/struct_02061AB4_decl.h"

#include "field/field_system.h"
#include "field/field_system_sub2_t.h"
#include "struct_defs/struct_02049FA8.h"
#include "overlay115/struct_ov115_0226527C.h"

#include "unk_02005474.h"
#include "heap.h"
#include "unk_02020020.h"
#include "unk_02050568.h"
#include "unk_020507CC.h"
#include "unk_020508D4.h"
#include "unk_020530C8.h"
#include "unk_02055808.h"
#include "unk_02056B30.h"
#include "player_avatar.h"
#include "map_object.h"
#include "unk_0206A8DC.h"
#include "unk_02070428.h"

typedef struct {
    FieldSystem * fieldSystem;
    u16 unk_04;
    u16 unk_06;
    u8 unk_08;
    u8 unk_09[3];
} UnkStruct_02050568;

static BOOL sub_020505A0(TaskManager * taskMan);
static void sub_0205074C(PlayerAvatar * playerAvatar, BOOL param1);
static void sub_0205075C(FieldSystem * fieldSystem);

void sub_02050568 (FieldSystem * fieldSystem)
{
    UnkStruct_02050568 * v0 = Heap_AllocFromHeapAtEnd(11, sizeof(UnkStruct_02050568));

    memset(v0, 0, sizeof(UnkStruct_02050568));
    sub_02050944(fieldSystem->unk_10, sub_020505A0, v0);
}

static BOOL sub_020505A0 (TaskManager * taskMan)
{
    FieldSystem * fieldSystem = TaskManager_FieldSystem(taskMan);
    UnkStruct_02050568 * v1 = TaskManager_Environment(taskMan);
    UnkStruct_020507E4 * v2 = SaveData_Events(fieldSystem->saveData);

    switch (v1->unk_08) {
    case 0:
        v1->unk_04 = Player_GetXPos(fieldSystem->playerAvatar);
        v1->unk_06 = Player_GetZPos(fieldSystem->playerAvatar);
        sub_02070428(fieldSystem, 1);
        sub_020558AC(taskMan);
        v1->unk_08++;
        break;
    case 1:
        sub_02055820(taskMan);
        v1->unk_08++;
        break;
    case 2:
        sub_0206AE0C(v2);

        {
            Location v3;

            inline_02049FA8(&v3, 172, -1, 847, 561, 1);
            sub_020539A0(taskMan, &v3);
        }
        v1->unk_08++;
        break;
    case 3:
        sub_02055868(taskMan);
        v1->unk_08++;
        break;
    case 4:
        sub_0205074C(fieldSystem->playerAvatar, 1);
        sub_0205075C(fieldSystem);
        sub_02056B30(taskMan, 3, 17, 0xffff, 0x0, 6, 1, 11);
        Sound_PlayEffect(1657);
        v1->unk_08++;
        break;
    case 5:
        if ((gCoreSys.pressedKeys & (PAD_BUTTON_A | PAD_BUTTON_B))) {
            v1->unk_08++;
        }
        break;
    case 6:
        sub_02056B30(taskMan, 3, 16, 0xffff, 0x0, 6, 1, 11);
        Sound_PlayEffect(1657);
        v1->unk_08++;
        break;
    case 7:
        sub_02055820(taskMan);
        v1->unk_08++;
        break;
    case 8:
        sub_0206AE1C(v2);

        {
            Location v4;

            inline_02049FA8(&v4, 164, -1, v1->unk_04, v1->unk_06, 0);
            sub_020539A0(taskMan, &v4);
        }
        v1->unk_08++;
        break;
    case 9:
        sub_02055868(taskMan);
        v1->unk_08++;
        break;
    case 10:
        sub_0205074C(fieldSystem->playerAvatar, 0);
        sub_020558F0(taskMan);
        v1->unk_08++;
        break;
    case 11:
        Heap_FreeToHeap(v1);
        sub_02070428(fieldSystem, 0);
        return 1;
    }

    return 0;
}

static void sub_0205074C (PlayerAvatar * playerAvatar, BOOL param1)
{
    MapObject * mapObj = Player_MapObject(playerAvatar);
    MapObject_SetHidden(mapObj, param1);
}

static void sub_0205075C (FieldSystem * fieldSystem)
{
    VecFx32 v0;
    UnkStruct_ov115_0226527C v1;

    sub_02020910(0x8c1, fieldSystem->unk_24);
    sub_02020A50(0xf81b8, fieldSystem->unk_24);

    v0.x = 0x350523d;
    v0.y = 0x15edb7;
    v0.z = 0x23da40e;

    sub_02020ACC(&v0, fieldSystem->unk_24);

    v1.unk_00 = 0x823;
    v1.unk_02 = 0x520;
    v1.unk_04 = 0;

    sub_020209D4(&v1, fieldSystem->unk_24);
    sub_020206BC(12 * FX32_ONE, 1564 * FX32_ONE, fieldSystem->unk_24);
}
