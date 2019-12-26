/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include "get_image.h"
#include "Bsp_RGB_Samp.h"
#include "sc030_fpga.h"



unsigned int sno[2];
unsigned short imageSno;
unsigned short NoUseImage;
extern void My_vdma_write_init(unsigned int H,unsigned int V,unsigned int channle,unsigned int *Rx,unsigned char Rxnum);
unsigned short Get_imageSno(void)
{
	return imageSno;
}
//u8 frameBuf[DISPLAY_NUM_FRAMES][DEMO_MAX_FRAME] __attribute__ ((aligned(64)));
u8 *pFrames[MAX_FRAME]; //array of pointers to the frame buffers
/***************************************************************************************
 * ������void Image_init(void)
 * ���ܣ���ͼƬ�������
 *      ����ϵ�� ���밴�� �ڴ�����
 ***************************************************************************************/
void Image_init(void)
{
		//unsigned short i;
		unsigned short HH = 640;
		unsigned short VV = 480;
		unsigned short channl;
		int stats;
		unsigned int addrs =0x17000000;
		//-----------------------------------����ڴ������--------------------------------
		//Queue_init2();            //ENT�����ڴ������ʼ��

		Bsp_RGBBuff_Init();       //ͼƬ����ڴ�����FPGA VDMA���
		//Get_ImagePixel(&HH,&VV,&channl,0);
		SC30_Set_HHVVCC(HH,VV,channl);
		SC30_Set_AutoRate(30);
		Bsp_RGB_GetpFrames((unsigned int *)&pFrames[0], HH, VV, MAX_FRAME);//�˺�����pFrames��RGBSamp.Framesp[i]ָ�붼ָ��ͼƬ����������8��
	    //Bsp_VDMAframe_init();


//		SC30_PWDN();
//		SC030GS_init();//��ʼ��

		//My_vdma_write_init(HH, VV, channl,(unsigned char *)pFrames,4);
		//vdma_write_init(XPAR_AXI_VDMA_0_DEVICE_ID, HH*channl+52, VV, HH*channl+52, (unsigned char *)pFrames);
		//vdma1_enable();
		//vdma1_write_init(XPAR_AXI_VDMA_VIDEOSTREAM_DEVICE_ID, 0x280, VV, 0x280, (unsigned char *)pFrames);
		//vdma1_write_init(XPAR_AXI_VDMA_VIDEOSTREAM_DEVICE_ID, 0xf00, 0x50, 0xf00, (unsigned char *)pFrames);
		SC30_XCK();
		SC30_PWDN();
		SC30_RESETB();
		SC030GS_init();//��ʼ��
		//stats=sensor_init(SENSORADDR);
		vdma1_enable();
		//vdma_videoStr_write_init();
		vdma_ImgCap_write_init();
		//vdma_imgcopy_init();


//		stats = 0;
//		while(!stats)
//		{
//			stats = vdma0_rd_status();
//
//		};
		unsigned char  hist_en = 0x00;

		 dpc_thread(200);
		 dpc_ctrl(1);
		 debug_cmd(16);
		 Sobel_model(0);
		 Sobel_en(1);



		while(1)
		{
			vdma0_enable();
			xil_printf("R0_reg3 = %x \r\n",vdma0_rd_status());
			vdma0_disable();
			xil_printf("R0_reg3 = %x \r\n",vdma0_rd_status());

			E_OUT1(1);
			E_OUT2(1);

			E_OUT1(0);
			E_OUT2(0);



			if(hist_en ==0)
			{
				hist_en =0x02;
				debug_cmd(0);
				GAUS_EN();

//				dpc_ctrl(0);
			}
			else
			{
				hist_en = 0x00;
				debug_cmd(16);
//				dpc_ctrl(1);

			}
//			hist_ctl(hist_en);

		}










}
/**************************************************
 *
 **************************************************/
int Image_Get(unsigned char LOCK)
{
			unsigned char Framesno;
			sno[0]=Bsp_ReadfreamSno();//��ȡ�����Ѿ���ɵ�֡��
			if(sno[0]!=sno[1])
			{//֡�Ŵ�0-7
				sno[1] = sno[0];

				Framesno=Bsp_GetGrayCode(sno[0]&0xf);

				if(LOCK==0)
				{

					Bsp_SetSkipfreamSno(sno[1]);

				}

				//֪ͨcpu0����ǰ���������٣����Բ�Ҫ�˲�
				Bsp_WriteCPU1SNO(USE_PATH,1);
                //return Bsp_RGB_GetFrameAddr(Framesno);//��New ����
			}

    return 0;
}