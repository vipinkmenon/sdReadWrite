#include <xil_types.h>
#include "ff.h"
#include "xil_printf.h"
#include <xstatus.h>
#include "xil_cache.h"

int SD_Init();
int SD_Eject();
int ReadFile(char *FileName, u32 DestinationAddress);
int WriteFile(char *FileName, u32 size, u32 SourceAddress);
