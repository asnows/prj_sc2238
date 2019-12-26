/*
 * sc030_fpga.h
 *
 *  Created on: 2018年7月31日
 *      Author: Administrator
 */

#ifndef SC030_FPGA_H_
#define SC030_FPGA_H_
void SC30_XCK(void);
void SC30_PWDN(void);//PWDN上电脉冲信号
void SC30_RESETB(void);//复位信号
void SC30_Set_HHVVCC(unsigned short HH,unsigned short VV,unsigned char CC);
void SC30_Set_AutoRate(float Rate);
void SC30_Set_SoftExposure(void);
void vdma1_enable(void);
void vdma0_enable(void);
void vdma0_disable(void);
int vdma0_rd_status(void);
void hist_ctl(unsigned char  val);
void dpc_thread(unsigned int val);
void dpc_ctrl(unsigned char val);
void debug_cmd(unsigned int cmd);
void E_OUT1(unsigned int flg);
void E_OUT2(unsigned int flg);
void GAUS_EN(void);
void Sobel_model(unsigned int model);
void Sobel_en(unsigned int cmd);




#endif /* SC030_FPGA_H_ */
