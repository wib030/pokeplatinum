#include <nitro.h>
#include <string.h>
#include <dwc.h>

#include "heap.h"
#include "unk_02033200.h"
#include "overlay018/ov18_0221F800.h"

void ov18_0221F800 (int param0)
{
    OSIntrMode v0;
    void * v1;

    sub_02033478();

    OS_InitTick();
    OS_InitAlarm();

    v0 = OS_DisableInterrupts();
    DWC_SetAuthServer((DWC_CONNECTINET_AUTH_RELEASE));
    v1 = Heap_AllocFromHeap(param0, DWC_UTILITY_WORK_SIZE);

    (void)DWC_StartUtility(v1, DWC_LANGUAGE_ENGLISH, DWC_UTILITY_TOP_MENU_COMMON);

    Heap_FreeToHeap(v1);

    OS_RestoreInterrupts(v0);
    OS_EnableIrq( );
}
