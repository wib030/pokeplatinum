#ifndef POKEPLATINUM_CONSTANTS_PALETTE_H
#define POKEPLATINUM_CONSTANTS_PALETTE_H

enum PaletteBufferId {
    PLTTBUF_MAIN_BG = 0,
    PLTTBUF_SUB_BG,
    PLTTBUF_MAIN_OBJ,
    PLTTBUF_SUB_OBJ,
    PLTTBUF_EX_BEGIN,

    PLTTBUF_MAIN_EX_BG_0 = PLTTBUF_EX_BEGIN,
    PLTTBUF_MAIN_EX_BG_1,
    PLTTBUF_MAIN_EX_BG_2,
    PLTTBUF_MAIN_EX_BG_3,
    PLTTBUF_SUB_EX_BG_0,
    PLTTBUF_SUB_EX_BG_1,
    PLTTBUF_SUB_EX_BG_2,
    PLTTBUF_SUB_EX_BG_3,
    PLTTBUF_MAIN_EX_OBJ,
    PLTTBUF_SUB_EX_OBJ,

    PLTTBUF_MAX,
};

#define PLTTBUF_MAIN_BG_F                      (1<<PLTTBUF_MAIN_BG)
#define PLTTBUF_SUB_BG_F                       (1<<PLTTBUF_SUB_BG)
#define PLTTBUF_MAIN_OBJ_F                     (1<<PLTTBUF_MAIN_OBJ)
#define PLTTBUF_SUB_OBJ_F                      (1<<PLTTBUF_SUB_OBJ)
#define PLTTBUF_MAIN_EX_BG_0_F                 (1<<PLTTBUF_MAIN_EX_BG_0)
#define PLTTBUF_MAIN_EX_BG_1_F                 (1<<PLTTBUF_MAIN_EX_BG_1)
#define PLTTBUF_MAIN_EX_BG_2_F                 (1<<PLTTBUF_MAIN_EX_BG_2)
#define PLTTBUF_MAIN_EX_BG_3_F                 (1<<PLTTBUF_MAIN_EX_BG_3)
#define PLTTBUF_SUB_EX_BG_0_F                  (1<<PLTTBUF_SUB_EX_BG_0)
#define PLTTBUF_SUB_EX_BG_1_F                  (1<<PLTTBUF_SUB_EX_BG_1)
#define PLTTBUF_SUB_EX_BG_2_F                  (1<<PLTTBUF_SUB_EX_BG_2)
#define PLTTBUF_SUB_EX_BG_3_F                  (1<<PLTTBUF_SUB_EX_BG_3)
#define PLTTBUF_MAIN_EX_OBJ_F                  (1<<PLTTBUF_MAIN_EX_OBJ)
#define PLTTBUF_SUB_EX_OBJ_F                   (1<<PLTTBUF_SUB_EX_OBJ)
#define PLTTBUF_ALL_F                          ((1<<PLTTBUF_MAX)-1)

enum PaletteSelector {
    PLTTSEL_TRANSPARENT,
    PLTTSEL_OPAQUE,
    PLTTSEL_BOTH,
};

#endif // POKEPLATINUM_CONSTANTS_PALETTE_H
