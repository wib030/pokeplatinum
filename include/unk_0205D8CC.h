#ifndef POKEPLATINUM_UNK_0205D8CC_H
#define POKEPLATINUM_UNK_0205D8CC_H

#include "struct_decls/struct_02018340_decl.h"
#include "strbuf.h"
#include "game_options.h"
#include "struct_defs/struct_0205AA50.h"

void sub_0205D8CC(u32 param0, u32 param1);
void FieldMessage_AddWindow(BGL * param0, Window * param1, u32 param2);
void FieldMessage_DrawWindow(Window * param0, const Options * param1);
void sub_0205D988(Window * param0);
u8 FieldMessage_Print(Window * param0, Strbuf *param1, const Options * param2, u8 param3);
u8 sub_0205D9CC(Window * param0, Strbuf *param1, int param2, int param3, u8 param4, int param5);
u8 FieldMessage_FinishedPrinting(u8 param0);
void sub_0205DA1C(BGL * param0, Window * param1, u16 param2, u16 param3);
void sub_0205DA80(Window * param0, u16 param1, u16 param2);

#endif // POKEPLATINUM_UNK_0205D8CC_H
