/*
 * greyTest.c
 *
 *  Created on: Apr 30, 2020
 *      Author: VIPIN
 */

#include <stdio.h>
#include "xil_io.h"
#include "xparameters.h"
#include "xaxidma.h"
#include "xscugic.h"
#include "xil_cache.h"
#include "sdCard.h"

#define inputImageWidth 512
#define inputImageHeight 512

#define scale 2

#define outputImageWidth 512/scale
#define outputImageHeight 512/scale

XScuGic IntcInstance;


u32 checkHalted(u32 baseAddress,u32 offset);

char inputImage[inputImageWidth*inputImageHeight*3];
char outputImage[outputImageWidth*outputImageHeight];

int main(){
	XAxiDma myDma;
	u32 status;
	XAxiDma_Config *myDmaConfig;
	myDmaConfig = XAxiDma_LookupConfigBaseAddr(XPAR_AXI_DMA_0_BASEADDR);
	status = XAxiDma_CfgInitialize(&myDma, myDmaConfig);
	if(status != XST_SUCCESS){
		print("DMA initialization failed\n");
		return -1;
	}
	status = SD_Init();
    if (status != XST_SUCCESS) {
  	 print("file system init failed\n\r");
    	 return XST_FAILURE;
    }
	//Configure image size converter
	Xil_Out32(XPAR_IMAGESIZECONVERTER_0_S00_AXI_BASEADDR,inputImageWidth);
	Xil_Out32(XPAR_IMAGESIZECONVERTER_0_S00_AXI_BASEADDR+4,inputImageHeight);
	Xil_Out32(XPAR_IMAGESIZECONVERTER_0_S00_AXI_BASEADDR+8,scale);
	Xil_Out32(XPAR_IMAGESIZECONVERTER_0_S00_AXI_BASEADDR+12,scale);

	xil_printf("Input image Buffer address %0x\n\r",inputImage);
	xil_printf("Output image Buffer address %0x\n\r",outputImage);

	status = ReadFile("lenac.bin",(u32)inputImage);
    if (status != XST_SUCCESS) {
  	 print("file read failed\n\r");
    	 return XST_FAILURE;
    }

	Xil_DCacheInvalidate();
	for(int i=0;i<12;i++)
		xil_printf("%0x\n\r",inputImage[i]);
	status = XAxiDma_SimpleTransfer(&myDma, (u32)outputImage,(outputImageWidth*outputImageHeight),XAXIDMA_DEVICE_TO_DMA);
	if(status != XST_SUCCESS)
		xil_printf("DMA failed\n\r",status);
	status = XAxiDma_SimpleTransfer(&myDma, (u32)inputImage,(inputImageWidth*inputImageHeight*3),XAXIDMA_DMA_TO_DEVICE);
	if(status != XST_SUCCESS)
		xil_printf("DMA failed\n\r",status);
	status = checkHalted(XPAR_AXI_DMA_0_BASEADDR,0x4);
	while(status != 1){
	    status = checkHalted(XPAR_AXI_DMA_0_BASEADDR,0x4);
	}
	status = checkHalted(XPAR_AXI_DMA_0_BASEADDR,0x34);
	while(status != 1){
	    status = checkHalted(XPAR_AXI_DMA_0_BASEADDR,0x34);
	}
	print("DMA transfer success..\n\r");

	status = WriteFile("lenag.bin",(outputImageWidth*outputImageHeight),(u32)outputImage);
	if (status != XST_SUCCESS) {
		print("file write failed\n\r");
	    return XST_FAILURE;
	}
	status=SD_Eject();
    if (status != XST_SUCCESS) {
  	 print("SD card unmount failed\n\r");
    	 return XST_FAILURE;
    }
    xil_printf("done...");
	return 0;
}



u32 checkHalted(u32 baseAddress,u32 offset){
	u32 status;
	status = (XAxiDma_ReadReg(baseAddress,offset))&XAXIDMA_HALTED_MASK;
	return status;
}
