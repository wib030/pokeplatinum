    .include "macros/scrcmd.inc"

    .data

    .byte 2
    .short 1, 0
    .byte 4
    .short 2, 0
    .byte 1
    .long _0010-.-4
    .byte 0

_0010:
    .short 0x4095, 0, 3
    .short 0

    .balign 4, 0
