#include <nitro.h>
#include <string.h>

#include "struct_decls/sys_task.h"

#include "functypes/sys_task_func.h"

#include "unk_0200679C.h"
#include "unk_0200D9E8.h"
#include "heap.h"
#include "unk_0201CCF0.h"

SysTask * sub_0200679C (SysTaskFunc param0, int param1, u32 param2, const u32 param3)
{
    void * v0;

    if (param1) {
        v0 = Heap_AllocFromHeap(param3, param1);

        if (v0 == NULL) {
            return NULL;
        }

        memset(v0, 0, param1);
    } else {
        v0 = NULL;
    }

    return SysTask_Start(param0, v0, param2);
}

void sub_020067D0 (SysTask * param0)
{
    void * v0;

    v0 = sub_0201CED0(param0);

    if (v0 != NULL) {
        Heap_FreeToHeap(v0);
    }

    SysTask_Done(param0);
}
