    .include "macros/scrcmd.inc"

    .data

    .byte 2
    .short 1, 0
    .byte 3
    .short 2, 0
    .byte 1
    .long _0010-.-4
    .byte 0

_0010:
    .short 0x40DE, 1, 3
    .short 0x40DE, 2, 4
    .short 0

    .balign 4, 0
