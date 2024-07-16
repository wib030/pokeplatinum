#include <nitro.h>
#include <string.h>

#include "inlines.h"

#include "trainer_info.h"
#include "struct_decls/struct_02026310_decl.h"
#include "struct_decls/struct_0202CD88_decl.h"
#include "struct_decls/struct_0203A790_decl.h"
#include "struct_decls/struct_020507E4_decl.h"
#include "struct_decls/struct_02061AB4_decl.h"
#include "pokemon.h"
#include "struct_decls/struct_party_decl.h"

#include "struct_defs/struct_0203A55C.h"
#include "field/field_system.h"
#include "field/field_system_sub2_t.h"
#include "struct_defs/struct_02049FA8.h"
#include "overlay005/struct_ov5_021D1CAC.h"
#include "overlay005/struct_ov5_021D219C.h"

#include "unk_02005474.h"
#include "unk_02025E08.h"
#include "trainer_info.h"
#include "unk_020261E4.h"
#include "unk_0202CD50.h"
#include "unk_02030EE0.h"
#include "communication_information.h"
#include "communication_system.h"
#include "unk_020366A0.h"
#include "map_header.h"
#include "unk_0203A378.h"
#include "unk_0203A6DC.h"
#include "field_menu.h"
#include "unk_0203C954.h"
#include "unk_0203E880.h"
#include "unk_020507CC.h"
#include "unk_02050A74.h"
#include "unk_020530C8.h"
#include "unk_02054884.h"
#include "unk_02054D00.h"
#include "unk_020562F8.h"
#include "unk_02056B30.h"
#include "comm_player_manager.h"
#include "field_comm_manager.h"
#include "unk_0205A0D8.h"
#include "unk_0205B33C.h"
#include "unk_0205DAC8.h"
#include "player_avatar.h"
#include "unk_0205F180.h"
#include "map_object.h"
#include "unk_02067A84.h"
#include "unk_020683F4.h"
#include "pokeradar.h"
#include "unk_0206A8DC.h"
#include "unk_0206AFE0.h"
#include "unk_02071B10.h"
#include "pokemon.h"
#include "party.h"
#include "overlay005/ov5_021D1C30.h"
#include "overlay005/ov5_021DB888.h"
#include "overlay005/ov5_021DFB54.h"
#include "overlay005/ov5_021E1154.h"
#include "overlay005/ov5_021E622C.h"
#include "overlay005/ov5_021EA714.h"
#include "overlay005/ov5_021EF4BC.h"
#include "overlay005/ov5_021EFB0C.h"
#include "overlay005/ov5_021F8370.h"
#include "overlay006/ov6_02240C9C.h"
#include "overlay006/ov6_02246BF4.h"
#include "overlay008/ov8_02249960.h"
#include "overlay009/ov9_02249960.h"
#include "overlay023/ov23_02241F74.h"

static BOOL ov5_021D2504(FieldSystem * fieldSystem, const UnkStruct_ov5_021D1CAC * param1);
static u16 ov5_021D271C(FieldSystem * fieldSystem, u8 param1);
static BOOL ov5_021D249C(FieldSystem * fieldSystem);
static BOOL ov5_021D2884(FieldSystem * fieldSystem);
static BOOL ov5_021D29A0(FieldSystem * fieldSystem);
static BOOL ov5_021D29D8(FieldSystem * fieldSystem, const int param1, const int param2, const u8 param3);
static BOOL ov5_021D2ABC(FieldSystem * fieldSystem);
static BOOL ov5_021D2B94(FieldSystem * fieldSystem);
static BOOL ov5_021D2C14(FieldSystem * fieldSystem);
static BOOL ov5_021D2B08(FieldSystem * fieldSystem);
static BOOL ov5_021D2B14(FieldSystem * fieldSystem);
static BOOL ov5_021D20DC(FieldSystem * fieldSystem);
static BOOL ov5_021D2B20(FieldSystem * fieldSystem);
static BOOL ov5_021D2B2C(FieldSystem * fieldSystem);
static void ov5_021D2B54(FieldSystem * fieldSystem);
static void ov5_021D2C7C(const FieldSystem * fieldSystem, int * param1, int * param2);
static void ov5_021D2C98(const FieldSystem * fieldSystem, int * param1, int * param2);
static void ov5_021D2CB4(const FieldSystem * fieldSystem, int param1, int * param2, int * param3);
static u8 ov5_021D2CFC(const FieldSystem * fieldSystem);
static u8 ov5_021D2D18(const FieldSystem * fieldSystem);
static BOOL ov5_021D2D34(const FieldSystem * fieldSystem, int param1, int param2, Location * param3);
static void ov5_021D2E14(FieldSystem * fieldSystem);
static BOOL ov5_021D2EA4(FieldSystem * fieldSystem, MapObject ** param1);
static int ov5_021D2274(void);

static void ov5_021D1C30 (UnkStruct_ov5_021D1CAC * param0)
{
    param0->unk_00_0 = 0;
    param0->unk_00_1 = 0;
    param0->unk_00_2 = 0;
    param0->unk_00_3 = 0;
    param0->unk_00_4 = 0;
    param0->unk_00_5 = 0;
    param0->unk_00_6 = 0;
    param0->unk_00_7 = 0;
    param0->unk_00_8 = 0;
    param0->unk_00_9 = 0;
    param0->unk_00_10 = 0;
    param0->unk_00_11 = 0;
    param0->unk_02 = -1;
    param0->unk_03 = -1;
}

void ov5_021D1CAC (UnkStruct_ov5_021D1CAC * param0, FieldSystem * fieldSystem, u16 param2, u16 param3)
{
    int v0;
    int v1;
    int v2;

    ov5_021D1C30(param0);

    v0 = Player_MoveState(fieldSystem->playerAvatar);
    v1 = PlayerAvatar_MoveState(fieldSystem->playerAvatar);
    v2 = PlayerAvatar_GetDir(fieldSystem->playerAvatar);

    param0->unk_04 = param2;
    param0->unk_06 = param3;

    if ((v0 == 3) || (v0 == 0)) {
        if (param2 & PAD_BUTTON_X) {
            param0->unk_00_2 = 1;
        }

        if (param2 & PAD_BUTTON_Y) {
            param0->unk_00_3 = 1;
        }

        if (param2 & PAD_BUTTON_A) {
            param0->unk_00_0 = 1;
        }

        if (param2 & PAD_BUTTON_B) {
            param0->unk_00_7 = 1;
        }

        if (param3 & (PAD_KEY_UP | PAD_KEY_DOWN | PAD_KEY_LEFT | PAD_KEY_RIGHT)) {
            param0->unk_00_4 = 1;
        }

        if (param3 & (PAD_KEY_UP | PAD_KEY_DOWN | PAD_KEY_LEFT | PAD_KEY_RIGHT)) {
            param0->unk_00_5 = 1;
        }
    }

    if ((v0 == 3) && (v1 == 1)) {
        param0->unk_00_6 = 1;
    }

    if (v0 == 3) {
        param0->unk_00_1 = 1;
    }

    if (((v2 == 0) && (param3 & PAD_KEY_UP)) || ((v2 == 1) && (param3 & PAD_KEY_DOWN)) || ((v2 == 2) && (param3 & PAD_KEY_LEFT)) || ((v2 == 3) && (param3 & PAD_KEY_RIGHT))) {
        param0->unk_03 = v2;
    } else {
        param0->unk_03 = -1;
    }

    param0->unk_02 = sub_02061308(fieldSystem->playerAvatar, param2, param3);
}

int ov5_021D1DA4 (const UnkStruct_ov5_021D1CAC * param0, FieldSystem * fieldSystem)
{
    if ((param0->unk_00_11 == 0) && (sub_0203F5C0(fieldSystem, 1) == 1)) {
        return 1;
    }

    {
        if (param0->unk_00_11 == 0) {
            BOOL v0 = sub_02054AB0(Party_GetFromSavedata(fieldSystem->saveData));

            if (sub_0206A984(SaveData_Events(fieldSystem->saveData)) == 1) {
                v0 = 1;
            }

            if ((sub_02067A84(fieldSystem, v0) == 1) || ((sub_02071CB4(fieldSystem, 2) == 1) && (ov8_0224C51C(fieldSystem) == 1))) {
                sub_0205F56C(fieldSystem->playerAvatar);
                MapObjectMan_PauseAllMovement(fieldSystem->mapObjMan);
                return 1;
            }
        }
    }

    if (param0->unk_00_6) {
        sub_0206A9A4(SaveData_Events(fieldSystem->saveData));

        if (ov5_021D2884(fieldSystem) == 1) {
            return 1;
        }
    }

    if (param0->unk_00_11 == 0) {
        int v1 = 0;
        int v2 = sub_02061308(fieldSystem->playerAvatar, param0->unk_04, param0->unk_06);

        if (inline_0204E650_2(SaveData_Events(fieldSystem->saveData))) {
            v1 |= 1 << 0;
        }

        if (sub_020549A0(Party_GetFromSavedata(fieldSystem->saveData), 127) != 0xff) {
            v1 |= 1 << 1;
        }

        if (sub_02071CB4(fieldSystem, 9) == 1) {
            if (ov9_02250F74(fieldSystem) == 1) {
                v1 |= 1 << 2;
            }
        }

        if (ov5_021DFDE0(fieldSystem, fieldSystem->playerAvatar, v2, v1) == 1) {
            return 1;
        }
    }

    if (param0->unk_00_1) {
        if (ov5_021D249C(fieldSystem)) {
            return 1;
        }

        if (ov5_021D20DC(fieldSystem) == 1) {
            return 1;
        }

        if (sub_02071CB4(fieldSystem, 9) == 1) {
            int v3 = (s8)param0->unk_02;

            if (v3 == -1) {
                v3 = PlayerAvatar_GetDir(fieldSystem->playerAvatar);
            }

            if (ov9_0224A67C(fieldSystem, v3) == 1) {
                return 1;
            }
        }
    }

    if (param0->unk_00_4 && (PlayerAvatar_GetDir(fieldSystem->playerAvatar) == param0->unk_02)) {
        if (ov5_021D20DC(fieldSystem) == 1) {
            return 1;
        }
    }

    if (param0->unk_00_0) {
        {
            int v4;
            MapObject * v5;

            if (sub_02071CB4(fieldSystem, 9) == 1) {
                v4 = ov5_021D2EA4(fieldSystem, &v5);
            } else {
                v4 = sub_0203CA40(fieldSystem, &v5);
            }

            if (v4 == 1) {
                UnkStruct_ov5_021D219C * v6;

                if (sub_0205F588(fieldSystem->playerAvatar) == 1) {
                    sub_0205F5E4(
                        fieldSystem->playerAvatar, PlayerAvatar_GetDir(fieldSystem->playerAvatar));
                }

                if (sub_02062950(v5) != 0x9) {
                    sub_0203E880(fieldSystem, sub_02062960(v5), v5);
                } else {
                    sub_0203E880(fieldSystem, 0, v5);
                }

                return 1;
            }
        }

        {
            int v7;

            v7 = sub_0205F108(fieldSystem->playerAvatar);

            if ((v7 == 0) || (v7 == 1)) {
                int v8;

                v8 = sub_0203CA6C(
                    fieldSystem, (void *)sub_0203A440(fieldSystem), sub_0203A448(fieldSystem));

                if (v8 != 0xffff) {
                    sub_0203E880(fieldSystem, v8, NULL);
                    return 1;
                }
            }

            if (v7 == 0) {
                int v9;

                if (ov5_021EFB40(fieldSystem, &v9)) {
                    sub_0203E880(fieldSystem, v9, NULL);
                    return 1;
                }
            }
        }

        {
            int v10 = sub_0205EAA0(fieldSystem->playerAvatar);
            u32 v11 = sub_020616F0(fieldSystem->playerAvatar, v10);
            int v12 = ov5_021D271C(fieldSystem, v11);

            if (v12 != 0xffff) {
                sub_0203E880(fieldSystem, v12, NULL);
                return 1;
            }
        }

        {
            int v13, v14, v15;

            v15 = PlayerAvatar_GetDir(fieldSystem->playerAvatar);
            ov5_021D2C98(fieldSystem, &v13, &v14);

            if (ov5_021F8410(fieldSystem, v13, v14, v15) == 1) {
                return 1;
            }
        }
    }

    if (param0->unk_00_5) {
        if (sub_02071CB4(fieldSystem, 9) == 1) {
            ov9_0224A800(fieldSystem, param0->unk_03);
        } else if (ov5_021D2504(fieldSystem, param0) == 1) {
            ov5_021D2E14(fieldSystem);
            return 1;
        }
    }

    if (param0->unk_00_3) {
        if (sub_02069238(fieldSystem) == 1) {
            return 1;
        }
    }

    if (param0->unk_00_2) {
        if (sub_0203A9C8(fieldSystem) == 1) {
            Sound_PlayEffect(1533);
            FieldMenu_Init(fieldSystem);
            return 1;
        }
    }

    return 0;
}

static BOOL ov5_021D20DC (FieldSystem * fieldSystem)
{
    MapObject * v0;
    int v1;

    if (sub_0203CBE0(fieldSystem, &v0) == 1) {
        sub_0203E880(fieldSystem, sub_02062960(v0), v0);
        return 1;
    }

    v1 = sub_0203CB80(fieldSystem, sub_0203A440(fieldSystem), sub_0203A448(fieldSystem));

    if (v1 != 0xffff) {
        sub_0203E880(fieldSystem, v1, NULL);
        return 1;
    }

    return 0;
}

BOOL ov5_021D213C (UnkStruct_ov5_021D1CAC * param0, FieldSystem * fieldSystem)
{
    u8 v0 = 0;

    if ((param0->unk_00_11 == 0) && (sub_0203F5C0(fieldSystem, 1) == 1)) {
        return 1;
    }

    if (sub_020363A0() || (0 != CommPlayer_GetMovementTimer(CommSys_CurNetId()))) {
        return 0;
    }

    ov23_02242814();

    if (param0->unk_00_0) {
        ov23_02242830(v0);
        return 0;
    }

    if (param0->unk_00_2) {
        ov23_022427F8();
        return 0;
    }

    if (ov23_02242458()) {
        return 0;
    }

    return 0;
}

BOOL ov5_021D219C (UnkStruct_ov5_021D1CAC * param0, FieldSystem * fieldSystem)
{
    if (param0->unk_00_5) {
        if ((param0->unk_03 == 1) && sub_0205DB1C(ov5_021D2CFC(fieldSystem))) {
            sub_0203E880(fieldSystem, 9101, NULL);
            return 1;
        }
    }

    if (sub_020363A0() || (0 != CommPlayer_GetMovementTimer(CommSys_CurNetId())) || !sub_02059D2C()) {
        return 0;
    }

    if (param0->unk_00_0) {
        {
            MapObject * v0;

            if (sub_0203CA40(fieldSystem, &v0) == 1) {
                UnkStruct_ov5_021D219C * v1;

                if (0x1 != MapObject_GetMoveCode(v0)) {
                    if (sub_0205F588(fieldSystem->playerAvatar) == 1) {
                        sub_0205F5E4(fieldSystem->playerAvatar, PlayerAvatar_GetDir(fieldSystem->playerAvatar));
                    }

                    sub_0203E880(fieldSystem, sub_02062960(v0), v0);
                    return 1;
                }
            }
        }
    }

    if (param0->unk_00_0) {
        sub_0205B2D4(fieldSystem);
        return 1;
    }

    if (param0->unk_00_2) {
        Sound_PlayEffect(1533);
        sub_0203AABC(fieldSystem);
        return 1;
    }

    return 0;
}

static int ov5_021D2274 (void)
{
    int v0, v1;
    TrainerInfo * v2;

    v1 = 0;

    for (v0 = 1; v0 < 5; v0++) {
        v2 = CommInfo_TrainerInfo(v0);

        if (v2 != NULL) {
            v1++;
        }
    }

    return v1 >= 1;
}

BOOL ov5_021D2298 (const UnkStruct_ov5_021D1CAC * param0, FieldSystem * fieldSystem)
{
    if (sub_020318EC() & 0xfe) {
        if (ov5_021D2274() && (CommSys_ConnectedCount() > 1)) {
            sub_0203E880(fieldSystem, 5, NULL);
            return 1;
        }

        return 0;
    }

    if (param0->unk_00_0) {
        MapObject * v0;

        if (sub_0203CA40(fieldSystem, &v0) == 1) {
            if (sub_0205F588(fieldSystem->playerAvatar) == 1) {
                sub_0205F5E4(fieldSystem->playerAvatar, PlayerAvatar_GetDir(fieldSystem->playerAvatar));
            }

            sub_02036B84();
            sub_0203E880(fieldSystem, sub_02062960(v0), v0);

            return 1;
        }
    }

    if (param0->unk_00_6 && sub_0205DEE4(ov5_021D2CFC(fieldSystem))) {
        sub_020545EC(fieldSystem);
        return 1;
    }

    if (param0->unk_00_2) {
        if (CommSys_ConnectedCount() > 1) {
            (void)0;
        } else {
            Sound_PlayEffect(1533);
            sub_0203AA78(fieldSystem);
            sub_0205BEA8(4);
            sub_02036BA0();

            return 1;
        }
    }

    return 0;
}

int ov5_021D2368 (const UnkStruct_ov5_021D1CAC * param0, FieldSystem * fieldSystem)
{
    if ((param0->unk_00_11 == 0) && (sub_0203F5C0(fieldSystem, 1) == 1)) {
        return 1;
    }

    if (param0->unk_00_0) {
        {
            MapObject * v0;

            if (sub_0203CA40(fieldSystem, &v0) == 1) {
                UnkStruct_ov5_021D219C * v1;

                if (sub_0205F588(fieldSystem->playerAvatar) == 1) {
                    sub_0205F5E4(fieldSystem->playerAvatar, PlayerAvatar_GetDir(fieldSystem->playerAvatar));
                }

                if (sub_02062950(v0) != 0x9) {
                    sub_0203E880(fieldSystem, sub_02062960(v0), v0);
                } else {
                    sub_0203E880(fieldSystem, 0, v0);
                }

                return 1;
            }
        }

        {
            int v2;

            v2 = sub_0203CA6C(
                fieldSystem, (void *)sub_0203A440(fieldSystem), sub_0203A448(fieldSystem));

            if (v2 != 0xffff) {
                sub_0203E880(fieldSystem, v2, NULL);
                return 1;
            }
        }

        {
            int v3;

            v3 = ov5_021D271C(fieldSystem, ov5_021D2D18(fieldSystem));

            if (v3 != 0xffff) {
                sub_0203E880(fieldSystem, v3, NULL);
                return 1;
            }
        }
    }

    if (param0->unk_00_5) {
        if (ov5_021D2504(fieldSystem, param0) == 1) {
            return 1;
        }
    }

    if (param0->unk_00_3) {
        if (sub_02069238(fieldSystem) == 1) {
            return 1;
        }
    }

    if (param0->unk_00_2) {
        Sound_PlayEffect(1533);
        FieldMenu_Init(fieldSystem);
        return 1;
    }

    return 0;
}

static BOOL ov5_021D249C (FieldSystem * fieldSystem)
{
    int v0, v1;

    ov5_021D2C7C(fieldSystem, &v0, &v1);

    if (sub_0206AE8C(SaveData_Events(fieldSystem->saveData)) == 1) {
        if (sub_02056374(fieldSystem, v0, v1) == 1) {
            sub_02051450(fieldSystem, sub_0205639C(fieldSystem));
            return 1;
        } else {
            return 0;
        }
    }

    if (MapHeader_HasWildEncounters(fieldSystem->unk_1C->unk_00) && (ov6_02240D5C(fieldSystem) == 1)) {
        return 1;
    }

    return 0;
}

static BOOL ov5_021D2504 (FieldSystem * fieldSystem, const UnkStruct_ov5_021D1CAC * param1)
{
    int v0;
    int v1;
    int v2;
    int v3;
    u8 v4;
    Location v5;

    if (param1->unk_03 == -1) {
        return 0;
    }

    if ((sub_02071CB4(fieldSystem, 4) == 1) && (ov8_0224BF4C(fieldSystem) == 1)) {
        return 1;
    }

    ov5_021D2C98(fieldSystem, &v1, &v2);

    if (FieldSystem_CheckCollision(fieldSystem, v1, v2) == 0) {
        return 0;
    }

    if (ov5_021D2D34(fieldSystem, v1, v2, &v5) && (param1->unk_03 != -1)) {
        v4 = sub_02054F94(fieldSystem, v1, v2);

        if (sub_0205DAEC(v4)) {
            int v6 = param1->unk_03;

            if (sub_02071CB4(fieldSystem, 2) == 1) {
                ov8_0224C62C(fieldSystem, v1, v2, &v6);
            }

            sub_02056BDC(fieldSystem, v5.unk_00, v5.unk_04, 0, 0, v6, 1);

            return 1;
        }
    }

    ov5_021D2C7C(fieldSystem, &v1, &v2);

    v4 = sub_02054F94(fieldSystem, v1, v2);

    if (sub_0205DAF8(v4) || sub_0205DB28(v4)) {
        if (param1->unk_03 != 3) {
            return 0;
        }
    } else if (sub_0205DB04(v4) || sub_0205DB34(v4)) {
        if (param1->unk_03 != 2) {
            return 0;
        }
    } else if (sub_0205DB1C(v4) || sub_0205DB4C(v4)) {
        if (param1->unk_03 != 1) {
            return 0;
        }
    } else if (sub_0205DC44(v4)) {
        if (param1->unk_03 != 3) {
            return 0;
        }
    } else if (sub_0205DC50(v4)) {
        if (param1->unk_03 != 2) {
            return 0;
        }
    }

    if (ov5_021D2D34(fieldSystem, v1, v2, &v5) == 0) {
        return 0;
    }

    {
        int v7;

        if (sub_0205DAEC(v4)) {
            v7 = 1;
        } else if (sub_0205DC44(v4)) {
            v7 = 3;
        } else if (sub_0205DC50(v4)) {
            v7 = 3;
        } else if (sub_0205DAF8(v4) || sub_0205DB28(v4)
                   || sub_0205DB04(v4) || sub_0205DB34(v4)
                   || sub_0205DB1C(v4) || sub_0205DB4C(v4)) {
            sub_02056C18(fieldSystem, v5.unk_00, v5.unk_04, 0, 0, param1->unk_03);
            return 1;
        } else {
            return 0;
        }

        sub_02056BDC(fieldSystem, v5.unk_00, v5.unk_04, 0, 0, param1->unk_03, v7);
    }

    return 1;
}

u16 ov5_021D271C (FieldSystem * fieldSystem, u8 param1)
{
    int v0 = PlayerAvatar_GetDir(fieldSystem->playerAvatar);

    if (sub_0205DBE4(param1) && (v0 == 0)) {
        return 2018;
    } else if (sub_0205DC80(param1)) {
        return 2500;
    } else if (sub_0205DC8C(param1)) {
        return 2501;
    } else if (sub_0205DC98(param1)) {
        return 2502;
    } else if (sub_0205DCA4(param1)) {
        return 2503;
    } else if (sub_0205DCB0(param1)) {
        return 2504;
    } else if (sub_0205DCBC(param1)) {
        return 2505;
    } else if (sub_0205DCC8(param1)) {
        return 2506;
    } else if (sub_0205DCD4(param1)) {
        return 2507;
    } else if (sub_0205DDB4(param1)) {
        return 10006;
    } else if (sub_0205DBF0(param1)) {
        return 2508;
    } else if (sub_0205DDC0(param1)) {
        return 2030;
    } else if (sub_0205DE84(param1) && (v0 == 0)) {
        return 10100;
    }

    if (ov5_021E0760(param1, v0)) {
        return 10003;
    }

    if (PlayerAvatar_GetPlayerState(fieldSystem->playerAvatar) != 0x2) {
        TrainerInfo * v1 = SaveData_GetTrainerInfo(fieldSystem->saveData);
        u32 v2 = sub_02061760(fieldSystem->playerAvatar);

        if (ov5_021E0118(fieldSystem->playerAvatar, v2, param1)

            && TrainerInfo_HasBadge(v1, 3)) {
            if (sub_020549A0(Party_GetFromSavedata(fieldSystem->saveData), 57) != 0xff) {
                return 10004;
            }
        }
    }

    return 0xffff;
}

static BOOL ov5_021D2884 (FieldSystem * fieldSystem)
{
    u8 v0;
    int v1, v2;
    int v3;

    if ((sub_02071CB4(fieldSystem, 3) == 1) && ov8_0224AAA8(fieldSystem)) {
        return 1;
    }

    if (sub_02071CB4(fieldSystem, 9) == 1) {
        if (ov9_0224A71C(fieldSystem) == 1) {
            return 1;
        }
    }

    v3 = PlayerAvatar_GetDir(fieldSystem->playerAvatar);

    if (ov5_021E1154(fieldSystem, fieldSystem->playerAvatar, v3) == 1) {
        return 1;
    }

    v1 = Player_GetXPos(fieldSystem->playerAvatar);
    v2 = Player_GetZPos(fieldSystem->playerAvatar);
    v0 = sub_02054F94(fieldSystem, v1, v2);

    if (ov5_021D29A0(fieldSystem) == 1) {
        return 1;
    }

    if (ov5_021D29D8(fieldSystem, v1, v2, v0) == 1) {
        ov5_021D2E14(fieldSystem);
        return 1;
    }

    if (sub_0205EF58(fieldSystem->playerAvatar)) {
        return 0;
    }

    ov5_021EA714(fieldSystem, 5, 1);

    if (ov5_021D2B94(fieldSystem) == 1) {
        return 1;
    }

    if (ov5_021D2C14(fieldSystem) == 1) {
        return 1;
    }

    if (ov5_021D2ABC(fieldSystem) == 1) {
        return 1;
    }

    if (ov5_021D2B08(fieldSystem) == 1) {
        return 1;
    }

    if (ov5_021D2B14(fieldSystem) == 1) {
        return 1;
    }

    if (ov5_021D2B20(fieldSystem) == 1) {
        return 1;
    }

    if (ov5_021D2B2C(fieldSystem)) {
        ov5_021D2B54(fieldSystem);
    }

    sub_0206B238(SaveData_Events(fieldSystem->saveData));
    return 0;
}

static BOOL ov5_021D29A0 (FieldSystem * fieldSystem)
{
    u16 v0;

    v0 = sub_0203CC14(fieldSystem, (void *)sub_0203A4AC(fieldSystem), sub_0203A4A4(fieldSystem));

    if (v0 != 0xffff) {
        sub_0203E880(fieldSystem, v0, NULL);
        return 1;
    }

    return 0;
}

static BOOL ov5_021D29D8 (FieldSystem * fieldSystem, const int param1, const int param2, const u8 param3)
{
    Location v0;

    if (ov5_021D2D34(fieldSystem, param1, param2, &v0) == 0) {
        return 0;
    }

    if (sub_0205DC2C(param3) == 1) {
        int v1 = PlayerAvatar_GetDir(fieldSystem->playerAvatar);

        if (v1 == 2) {
            v1 = 3;
        } else if (v1 == 3) {
            v1 = 2;
        } else {
            GF_ASSERT(FALSE);
            return 0;
        }

        sub_02056BDC(fieldSystem, v0.unk_00, v0.unk_04, 0, 0, v1, 2);
        return 1;
    } else if (sub_0205DC38(param3) == 1) {
        int v1 = PlayerAvatar_GetDir(fieldSystem->playerAvatar);

        if ((v1 != 2) && (v1 != 3)) {
            GF_ASSERT(FALSE);
            return 0;
        }

        sub_02056BDC(fieldSystem, v0.unk_00, v0.unk_04, 0, 0, v1, 2);
        return 1;
    }

    if (sub_0205DB10(param3) || sub_0205DB40(param3)) {
        sub_02056C18(fieldSystem, v0.unk_00, v0.unk_04, 0, 0, 0);
        return 1;
    }

    if (sub_0205DEE4(param3)) {
        sub_02053F58(fieldSystem, v0.unk_00, v0.unk_04);
        return 1;
    }

    return 0;
}

static BOOL ov5_021D2ABC (FieldSystem * fieldSystem)
{
    Party * v0 = Party_GetFromSavedata(fieldSystem->saveData);
    UnkStruct_02026310 * v1 = sub_02026310(fieldSystem->saveData);

    if (ov5_021E7154(v1, v0, fieldSystem) == 1) {
        UnkStruct_0202CD88 * v2 = sub_0202CD88(fieldSystem->saveData);

        sub_0202CF28(v2, (1 + 10));
        sub_0202CFEC(v2, 15);
        sub_0203E880(fieldSystem, 2031, NULL);

        return 1;
    } else {
        return 0;
    }
}

static BOOL ov5_021D2B08 (FieldSystem * fieldSystem)
{
    ov5_021DBB94(fieldSystem);
    return 0;
}

static BOOL ov5_021D2B14 (FieldSystem * fieldSystem)
{
    RadarChargeStep(fieldSystem);
    return 0;
}

static BOOL ov5_021D2B20 (FieldSystem * fieldSystem)
{
    return ov6_02246BF4(fieldSystem->saveData, fieldSystem);
}

static BOOL ov5_021D2B2C (FieldSystem * fieldSystem)
{
    u16 v0;
    UnkStruct_020507E4 * v1;
    BOOL v2 = 0;

    v1 = SaveData_Events(fieldSystem->saveData);
    v0 = sub_0206B44C(v1);

    v0++;

    if (v0 >= 128) {
        v0 = 0;
        v2 = 1;
    }

    sub_0206B45C(v1, v0);

    return v2;
}

static void ov5_021D2B54 (FieldSystem * fieldSystem)
{
    int v0, v1;
    Pokemon * v2;
    Party * v3 = Party_GetFromSavedata(fieldSystem->saveData);
    u16 v4 = MapHeader_GetMapLabelTextID(fieldSystem->unk_1C->unk_00);

    v1 = Party_GetCurrentCount(v3);

    for (v0 = 0; v0 < v1; v0++) {
        v2 = Party_GetPokemonBySlotIndex(v3, v0);
        Pokemon_UpdateFriendship(v2, 5, v4);
    }
}

static BOOL ov5_021D2B94 (FieldSystem * fieldSystem)
{
    Party * v0 = Party_GetFromSavedata(fieldSystem->saveData);
    u16 * v1 = sub_0203A78C(sub_0203A790(fieldSystem->saveData));

    (*v1)++;
    (*v1) %= 4;

    if (*v1 != 0) {
        return 0;
    }

    switch (sub_02054B04(v0, MapHeader_GetMapLabelTextID(fieldSystem->unk_1C->unk_00))) {
    case 0:
        return 0;
    case 1:
        ov5_021EF518(fieldSystem->unk_04->unk_20);
        return 0;
    case 2:
        ov5_021EF518(fieldSystem->unk_04->unk_20);
        sub_0203E880(fieldSystem, 2003, NULL);
        return 1;
    }

    return 0;
}

static BOOL ov5_021D2C14 (FieldSystem * fieldSystem)
{
    u16 * v0;
    u16 * v1;

    if (sub_0206AE5C(SaveData_Events(fieldSystem->saveData)) == 0) {
        return 0;
    }

    v0 = sub_0203A784(sub_0203A790(fieldSystem->saveData));

    if (*v0 == 0) {
        sub_0203E880(fieldSystem, 8802, NULL);
        return 1;
    }

    v1 = sub_0203A788(sub_0203A790(fieldSystem->saveData));
    (*v1)++;

    if (*v1 >= 500) {
        sub_0203E880(fieldSystem, 8801, NULL);
        return 1;
    }

    return 0;
}

static void ov5_021D2C7C (const FieldSystem * fieldSystem, int * param1, int * param2)
{
    *param1 = Player_GetXPos(fieldSystem->playerAvatar);
    *param2 = Player_GetZPos(fieldSystem->playerAvatar);
}

static void ov5_021D2C98 (const FieldSystem * fieldSystem, int * param1, int * param2)
{
    int v0;

    v0 = PlayerAvatar_GetDir(fieldSystem->playerAvatar);
    ov5_021D2CB4(fieldSystem, v0, param1, param2);
}

static void ov5_021D2CB4 (const FieldSystem * fieldSystem, int param1, int * param2, int * param3)
{
    ov5_021D2C7C(fieldSystem, param2, param3);

    switch (param1) {
    case 0:
        *param3 -= 1;
        break;
    case 1:
        *param3 += 1;
        break;
    case 2:
        *param2 -= 1;
        break;
    case 3:
        *param2 += 1;
        break;
    default:
        GF_ASSERT("FALSE");
    }
}

static u8 ov5_021D2CFC (const FieldSystem * fieldSystem)
{
    int v0, v1;

    ov5_021D2C7C(fieldSystem, &v0, &v1);
    return sub_02054F94(fieldSystem, v0, v1);
}

static u8 ov5_021D2D18 (const FieldSystem * fieldSystem)
{
    int v0, v1;

    ov5_021D2C98(fieldSystem, &v0, &v1);
    return sub_02054F94(fieldSystem, v0, v1);
}

static BOOL ov5_021D2D34 (const FieldSystem * fieldSystem, int param1, int param2, Location * param3)
{
    const UnkStruct_0203A55C * v0;
    int v1;

    v1 = sub_0203A468(fieldSystem, param1, param2);

    if (v1 == -1) {
        return 0;
    }

    v0 = sub_0203A450(fieldSystem, v1);

    if (v0 == NULL) {
        return 0;
    }

    if (v0->unk_06 == 0x100) {
        GF_ASSERT(v0->unk_04 == 0xfff);
        *param3 = *(sub_0203A730(sub_0203A790(fieldSystem->saveData)));
    } else {
        inline_02049FA8(param3, v0->unk_04, v0->unk_06, v0->unk_00, v0->unk_02, 1);
    }

    {
        Location * v2 = sub_0203A724(sub_0203A790(fieldSystem->saveData));
        inline_02049FA8(v2, fieldSystem->unk_1C->unk_00, v1, param1, param2, PlayerAvatar_GetDir(fieldSystem->playerAvatar));
    }

    return 1;
}

static void ov5_021D2DCC (FieldSystem * fieldSystem, const int param1, const int param2, const int param3)
{
    UnkStruct_0203A790 * v0 = sub_0203A790(fieldSystem->saveData);
    Location * v1 = sub_0203A72C(v0);

    (*v1) = *(fieldSystem->unk_1C);
    v1->unk_10 = param3;
    v1->unk_08 = param1;
    v1->unk_0C = param2;

    if (param3 == 0) {
        (v1->unk_0C)++;
    }

    v1->unk_00 = fieldSystem->unk_1C->unk_00;
    v1->unk_04 = -1;
}

static void ov5_021D2E14 (FieldSystem * fieldSystem)
{
    int v0, v1;
    Location v2;

    ov5_021D2C7C(fieldSystem, &v0, &v1);

    if (ov5_021D2D34(fieldSystem, v0, v1, &v2)) {
        if ((MapHeader_IsOnMainMatrix(fieldSystem->unk_1C->unk_00) == 1) && (MapHeader_IsOnMainMatrix(v2.unk_00) == 0)) {
            ov5_021D2DCC(fieldSystem, v0, v1, PlayerAvatar_GetDir(fieldSystem->playerAvatar));
        }
    } else {
        ov5_021D2C98(fieldSystem, &v0, &v1);

        if (ov5_021D2D34(fieldSystem, v0, v1, &v2)) {
            if ((MapHeader_IsOnMainMatrix(fieldSystem->unk_1C->unk_00) == 1) && (MapHeader_IsOnMainMatrix(v2.unk_00) == 0)) {
                ov5_021D2DCC(fieldSystem, v0, v1, PlayerAvatar_GetDir(fieldSystem->playerAvatar));
            }
        }
    }
}

static BOOL ov5_021D2EA4 (FieldSystem * fieldSystem, MapObject ** param1)
{
    int v0 = 0;
    int v1, v2, v3, v4, v5, v6;

    sub_020617BC(fieldSystem->playerAvatar, &v1, &v2, &v3);

    while (sub_020625B0(fieldSystem->mapObjMan, param1, &v0, (1 << 0))) {
        v4 = MapObject_GetXPos(*param1);
        v5 = ((MapObject_GetYPos(*param1) / 2));
        v6 = MapObject_GetZPos(*param1);

        if ((v2 == v5) && (v1 == v4) && (v3 == v6)) {
            return 1;
        }
    }

    return 0;
}
