#include <nitro.h>
#include <string.h>

#include "inlines.h"

#include "struct_decls/struct_020507E4_decl.h"

#include "struct_defs/struct_0202D7B0.h"
#include "field/field_system.h"
#include "struct_defs/struct_02049FA8.h"
#include "struct_defs/struct_020556C4.h"
#include "struct_defs/struct_0205EC34.h"

#include "unk_02025E08.h"
#include "unk_0202B604.h"
#include "unk_0202D7A8.h"
#include "map_header.h"
#include "unk_0203A6DC.h"
#include "unk_0203A7D8.h"
#include "unk_020507CC.h"
#include "unk_020556C4.h"
#include "unk_0206A8DC.h"
#include "unk_0206AFE0.h"
#include "roaming_pokemon.h"
#include "unk_02070428.h"

static BOOL sub_020705DC(FieldSystem * fieldSystem);
static BOOL sub_02070610(FieldSystem * fieldSystem);

void sub_02070428 (FieldSystem * fieldSystem, BOOL param1)
{
    fieldSystem->unk_B8 = param1;
}

void sub_02070430 (FieldSystem * fieldSystem)
{
    sub_020705DC(fieldSystem);
    sub_0206AF1C(SaveData_Events(fieldSystem->saveData), 0);

    inline_0204E650_1(SaveData_Events(fieldSystem->saveData));

    sub_0203A8E8(fieldSystem, fieldSystem->unk_1C->unk_00);
    sub_0202D9EC(sub_0202D834(fieldSystem->saveData), 0);

    fieldSystem->unk_78.unk_00 = 0;

    if (!sub_0206AE5C(SaveData_Events(fieldSystem->saveData))) {
        UnkStruct_0202D7B0 * v0;

        v0 = sub_0202D834(fieldSystem->saveData);
        sub_0206C404(v0, fieldSystem->unk_1C->unk_00);
        sub_0206C37C(v0);
    }
}

void sub_02070494 (FieldSystem * fieldSystem)
{
    if (fieldSystem->unk_B8 == 1) {
        return;
    }

    if (sub_020705DC(fieldSystem) == 0) {
        sub_02070610(fieldSystem);
    }

    sub_0206AF1C(SaveData_Events(fieldSystem->saveData), 0);
    sub_0206AE9C(SaveData_Events(fieldSystem->saveData));

    inline_0204E650_1(SaveData_Events(fieldSystem->saveData));

    sub_0203A8E8(fieldSystem, fieldSystem->unk_1C->unk_00);
    sub_0202D9EC(sub_0202D834(fieldSystem->saveData), 0);

    fieldSystem->unk_78.unk_00 = 0;

    {
        UnkStruct_0202D7B0 * v0;

        v0 = sub_0202D834(fieldSystem->saveData);
        sub_0206C404(v0, fieldSystem->unk_1C->unk_00);
    }

    if (!MapHeader_IsCave(fieldSystem->unk_1C->unk_00)) {
        UnkStruct_020507E4 * v1 = SaveData_Events(fieldSystem->saveData);

        sub_0206AECC(v1);
        sub_0206AEFC(v1);
    }

    {
        PlayerData * v2 = sub_0203A780(sub_0203A790(fieldSystem->saveData));

        if ((v2->unk_04 == 0x1) && (MapHeader_IsBikeAllowed(fieldSystem->unk_1C->unk_00) == 0)) {
            v2->unk_04 = 0x0;
        } else if (v2->unk_04 == 0x2) {
            v2->unk_04 = 0x0;
        }
    }

    if (MapHeader_IsOnMainMatrix(fieldSystem->unk_1C->unk_00)) {
        UnkStruct_020556C4 * v3;

        v3 = sub_0203A76C(sub_0203A790(fieldSystem->saveData));
        sub_020556E8(v3, fieldSystem->unk_1C->unk_08, fieldSystem->unk_1C->unk_0C);
    }
}

void sub_0207056C (FieldSystem * fieldSystem)
{
    sub_0206AE4C(SaveData_Events(fieldSystem->saveData));
    sub_0206C354(sub_0202D834(fieldSystem->saveData));
}

void sub_02070588 (FieldSystem * fieldSystem)
{
    sub_0206AE4C(SaveData_Events(fieldSystem->saveData));
    sub_0206C354(sub_0202D834(fieldSystem->saveData));
}

void sub_020705A4 (FieldSystem * fieldSystem)
{
    sub_0206AE4C(SaveData_Events(fieldSystem->saveData));
}

void sub_020705B4 (FieldSystem * fieldSystem)
{
    UnkStruct_020507E4 * v0 = SaveData_Events(fieldSystem->saveData);

    sub_0206A974(v0);
    sub_0206B024(v0, 0);
}

void sub_020705CC (FieldSystem * fieldSystem)
{
    sub_0206C354(sub_0202D834(fieldSystem->saveData));
}

static BOOL sub_020705DC (FieldSystem * fieldSystem)
{
    int v0 = sub_0203A87C(fieldSystem->unk_1C->unk_00);

    if ((v0 != 0) && (sub_0203A920(fieldSystem, v0) == 0)) {
        sub_0202C704(fieldSystem->unk_9C, fieldSystem->unk_1C->unk_00, 32);
        return 1;
    }

    return 0;
}

static BOOL sub_02070610 (FieldSystem * fieldSystem)
{
    Location * v0 = sub_0203A728(sub_0203A790(fieldSystem->saveData));

    if (v0->unk_00 != fieldSystem->unk_1C->unk_00) {
        sub_0202C5C4(
            SaveData_GetTrainerInfo(fieldSystem->saveData), fieldSystem->unk_9C, fieldSystem->unk_1C->unk_00, v0->unk_00, 32);
        return 1;
    }

    return 0;
}
