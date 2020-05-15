#include "sdCard.h"


static FATFS  fatfs;

int SD_Init()
{
	FRESULT rc;
	TCHAR *Path = "0:/";
	rc = f_mount(&fatfs,Path,0);
	if (rc) {
		xil_printf(" ERROR : f_mount returned %d\r\n", rc);
		return XST_FAILURE;
	}
	return XST_SUCCESS;
}

int SD_Eject()
{
	FRESULT rc;
	TCHAR *Path = "0:/";
	rc = f_mount(0,Path,1);
	if (rc) {
		xil_printf(" ERROR : f_mount returned %d\r\n", rc);
		return XST_FAILURE;
	}
	return XST_SUCCESS;
}



int ReadFile(char *FileName, u32 DestinationAddress)
{
	FIL fil;
	FRESULT rc;
	UINT br;
	u32 file_size;
	rc = f_open(&fil, FileName, FA_READ);
	if (rc) {
		xil_printf(" ERROR : f_open returned %d\r\n", rc);
		return XST_FAILURE;
	}
	file_size = fil.fsize;
	rc = f_lseek(&fil, 0);
	if (rc) {
		xil_printf(" ERROR : f_lseek returned %d\r\n", rc);
		return XST_FAILURE;
	}
	rc = f_read(&fil, (void*) DestinationAddress, file_size, &br);
	if (rc) {
		xil_printf(" ERROR : f_read returned %d\r\n", rc);
		return XST_FAILURE;
	}
	rc = f_close(&fil);
	if (rc) {
		xil_printf(" ERROR : f_close returned %d\r\n", rc);
		return XST_FAILURE;
	}
	Xil_DCacheFlush();
	return XST_SUCCESS;
}

int WriteFile(char *FileName, u32 size, u32 SourceAddress){
	UINT btw;
	static FIL fil; // File instance
	FRESULT rc; // FRESULT variable
	rc = f_open(&fil, (char *)FileName, FA_OPEN_ALWAYS | FA_WRITE); //f_open
	if (rc) {
		xil_printf(" ERROR : f_open returned %d\r\n", rc);
		return XST_FAILURE;
	}
	rc = f_write(&fil,(const void*)SourceAddress,size,&btw);
	if (rc) {
		xil_printf(" ERROR : f_write returned %d\r\n", rc);
		return XST_FAILURE;
	}
	rc = f_close(&fil);
	if (rc) {
		xil_printf(" ERROR : f_write returned %d\r\n", rc);
		return XST_FAILURE;
	}
	return XST_SUCCESS;
}


