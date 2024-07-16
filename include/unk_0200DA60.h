#ifndef POKEPLATINUM_UNK_0200DA60_H
#define POKEPLATINUM_UNK_0200DA60_H

#include "struct_decls/struct_02018340_decl.h"
#include "struct_defs/struct_0205AA50.h"
#include "pokemon.h"

#define TEXT_WINDOW_SIZE    30

void Window_SetFrame(BGL * param0, u8 param1, u16 param2, u8 param3, u32 param4);
u32 Window_FramePalette(void);
void sub_0200DAA4(BGL * param0, u8 param1, u16 param2, u8 param3, u8 param4, u32 param5);
void Window_Show(Window * param0, u8 param1, u16 param2, u8 param3);
void Window_Clear(Window * param0, u8 param1);
u32 sub_0200DD04(u32 param0);
u32 sub_0200DD08(u32 param0);
void sub_0200DD0C(BGL * param0, u8 param1, u16 param2, u8 param3, u8 param4, u32 param5);
void sub_0200E010(Window * param0, u32 param1, u32 param2);
void sub_0200E060(Window * param0, u8 param1, u16 param2, u8 param3);
void sub_0200E084(Window * param0, u8 param1);
void sub_0200E218(BGL * param0, u8 param1, u16 param2, u8 param3, u8 param4, u32 param5);
void sub_0200E2A4(BGL * param0, u8 param1, u16 param2, u8 param3, u8 param4, u16 param5, u32 param6);
void sub_0200E69C(Window * param0, u8 param1, u16 param2, u8 param3, u8 param4);
void sub_0200E744(Window * param0, u8 param1, u8 param2);
void * sub_0200E7FC(Window * param0, u32 param1);
void DeleteWaitDial(void * param0);
void sub_0200EBC8(void * param0);
u8 * sub_0200EBF0(BGL * param0, u8 param1, u8 param2, u8 param3, u8 param4, u16 param5, u16 param6, u8 param7, int param8);
u8 * sub_0200EC48(BGL * param0, u8 param1, u8 param2, u8 param3, u8 param4, u16 param5, Pokemon * param6, int param7);

#endif // POKEPLATINUM_UNK_0200DA60_H
