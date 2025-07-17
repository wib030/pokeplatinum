    .include "macros/btlcmd.inc"

    .data

_000:
    // Neutralizing gas fills the area!
    PrintMessage pl_msg_00000368_01365, TAG_NONE
    Wait 
    WaitButtonABTime 30
    End 