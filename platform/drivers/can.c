/*
 * File      : can.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2018, RT-Thread Development Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-01-06     sundm75       first version
 */

#include <rtthread.h>
#include "platform.h"

unsigned char  set_reset_mode(CAN_TypeDef* CANx)
{
  unsigned char status;
  int i;

  /*检查复位标志*/
  status = CANx->MOD;

  /* 关闭中断 */
  CANx->IER = 0x00;
  
  for (i = 0; i < 100; i++)
  {
    if((status & CAN_Mode_RM) == CAN_Mode_RM)
        return 1;
  
    /* 设置复位*/
    CANx->MOD |=  ((unsigned char)CAN_Mode_RM);
  
    /*延时*/
//    delay_us(10);
    rt_thread_mdelay(1);

    /*检查复位标志*/
    status = CANx->MOD;
  }
  rt_kprintf("\r\nSetting can into reset mode failed!\r\n");
  return 0;  
}

static unsigned char  set_normal_mode(CAN_TypeDef* CANx)
{
  unsigned char status;
  int i;

  /*检查复位标志*/
  status = CANx->MOD;
  
  for (i = 0; i < 100; i++)
  {
    if((status & CAN_Mode_RM) != CAN_Mode_RM)
    {
      /*开所有中断 （总线错误中断不开）*/
     CANx->IER |= (~(unsigned char)CAN_IR_BEI);	

     return 1;
    }
    /* 设置正常工作模式*/
    CANx->MOD &= (~(unsigned char) CAN_Mode_RM); 
    /*延时*/
//    delay_us(10);
    rt_thread_mdelay(1);
 
    status = CANx->MOD;	
  }
  rt_kprintf("\r\nSetting can into normal mode failed!\r\n");
  return 0;
}

unsigned char  set_start(CAN_TypeDef* CANx)
{
  /*复位TX错误计数器*/
  CANx->TXERR = 0;
  /*复位RX错误计数器*/
  CANx->RXERR = 0;
  /*时钟分频寄存器: PeliCAN模式; CBP=1,中止输入比较器, RX0激活*/
  //CANx->CDR = 0xC0;	
  return  set_normal_mode(CANx);  
}

unsigned char CAN_Init(CAN_TypeDef* CANx, CAN_InitTypeDef* CAN_InitStruct)
{
  unsigned char InitStatus = CAN_InitStatus_Failed;
  unsigned long wait_ack = 0x00000000;
  unsigned char status;

  status = CANx->MOD;
  if( status == 0xFF)
  {
    rt_kprintf("\n Probe can0 failed \r\n");  	   
    return CAN_InitStatus_Failed;
  }
    /* Basic mode change to extend mode */
  CANx->MOD |= CAN_Mode_RM; 	//basic > basic_reset
  CANx->EXMOD |= CAN_EXMOD_B2E; //basic_reset > extend_reset
    /* 进入复位模式 */
  InitStatus = set_reset_mode(CANx);
  
//  if((CAN_InitStruct->CAN_Mode & CAN_Mode_SM) == CAN_Mode_SM) 
//  {
//    /* 睡眠模式 1: 睡眠 0: 唤醒*/
//    CANx->MOD|= (unsigned char)CAN_Mode_SM;    
//  }
//  else
//  {
//     CANx->MOD&=~ (unsigned char)CAN_Mode_SM; 
//  }

 if((CAN_InitStruct->CAN_Mode & CAN_Mode_LOM) == CAN_Mode_LOM) 
  {
    /*只听模式 1:只听  0:正常 */
    CANx->MOD|= (unsigned char)CAN_Mode_LOM;    
  }
  else
  {
     CANx->MOD&=~ (unsigned char)CAN_Mode_LOM; 
  }

  if((CAN_InitStruct->CAN_Mode & CAN_Mode_AFM) == CAN_Mode_AFM) 
  {
    /*单滤波模式 1:单 0: 双*/
    CANx->MOD |= (unsigned char)CAN_Mode_AFM;    
  }
  else
  {
     CANx->MOD&=~ (unsigned char)CAN_Mode_AFM; 
  }

  if((CAN_InitStruct->CAN_Mode & CAN_Mode_STM) == CAN_Mode_STM) 
  {
    /*自检测模式 1:自检测  0:正常  */
    CANx->MOD |= (unsigned char)CAN_Mode_STM;    
  }
  else
  {
     CANx->MOD&=~ (unsigned char)CAN_Mode_STM;
  }
  
  /* 配置时钟频率 */
  CANx->BTR0 = (( unsigned char )( unsigned char )CAN_InitStruct->CAN_Prescaler -1) | \
               (unsigned char)CAN_InitStruct->CAN_SJW << 6;
  
  CANx->BTR1 = ((unsigned char)CAN_InitStruct->CAN_BS1) | \
               ((unsigned char)CAN_InitStruct->CAN_BS2 << 4) | \
               ((unsigned char)CAN_InitStruct->CAN_SJW<<7);
   /* 进入工作模式 */
  set_start(CANx);    
  
  /* 返回初始化结果 */
  return InitStatus;
}

void CAN_FilterInit(CAN_TypeDef* CANx,    CAN_FilterInitTypeDef * CAN_FilterInitStruct)
{
    unsigned long rtr;
    unsigned long fcase;
    unsigned long ide;
    unsigned long thisid, thisid1, thisid2;
    unsigned long thismask, thismask1, thismask2;
    unsigned long firstdata;
    unsigned long datamask;
    unsigned char CAN_FilterId0, CAN_FilterId1, CAN_FilterId2, CAN_FilterId3 ;
    unsigned char CAN_FilterMaskId0, CAN_FilterMaskId1, CAN_FilterMaskId2, CAN_FilterMaskId3;

    thisid = CAN_FilterInitStruct->ID;
    thismask = CAN_FilterInitStruct->IDMASK;
    thisid1 = (CAN_FilterInitStruct->ID & 0xFFFF0000 )>>16;
    thismask1 = (CAN_FilterInitStruct->IDMASK & 0xFFFF0000 )>>16;
    thisid2 = (CAN_FilterInitStruct->ID & 0x0000FFFF );
    thismask2 = ( CAN_FilterInitStruct->IDMASK& 0x0000FFFF  );
    rtr = CAN_FilterInitStruct->RTR; 
    ide = CAN_FilterInitStruct->IDE;
    firstdata = CAN_FilterInitStruct->First_Data;
    datamask = CAN_FilterInitStruct->Data_Mask;
    fcase = CAN_FilterInitStruct->MODE; 

    if(ide == 0)//标准帧
    {
	
        if(fcase == 0)// 0- 双滤波器模式
        {
            CAN_FilterId0  = thisid1>>3;
            CAN_FilterMaskId0 = thismask1>>3;
            CAN_FilterId1  = thisid1<<5 | firstdata>>4| rtr<<4;
            CAN_FilterMaskId1 = thismask1<<4 | datamask>>4 ;
            CAN_FilterId2  = thisid2 >> 3;
            CAN_FilterMaskId2 = thismask2 >>3;
            CAN_FilterId3  = firstdata & 0x0F | thisid2 <<5 | rtr<<4;
            CAN_FilterMaskId3 = datamask <<4  ;
        }
	
        if(fcase == 1)// 1-单滤波器模式
        {
            CAN_FilterId0  = thisid>>3;
            CAN_FilterMaskId0 = thismask>>3;
            CAN_FilterId1  = thisid<<5 | rtr<<4;
            CAN_FilterMaskId1 = thismask<<5  ;
            CAN_FilterMaskId1 |= 0x0F ;
            CAN_FilterId2  = 0x00;
            CAN_FilterMaskId2 = 0xFF;
            CAN_FilterId3  = 0x00;
            CAN_FilterMaskId3 = 0xFF  ;
        }
    }
    else if(ide == 1)//扩展帧
    {
        if(fcase == 0)// 0- 双滤波器模式
        {
            CAN_FilterId0  = thisid1>>8;
            CAN_FilterMaskId0 = thismask1>>8;
            CAN_FilterId1  = thisid1 ;
            CAN_FilterMaskId1 = thismask1 ;
            CAN_FilterId2  = thisid2>>8;
            CAN_FilterMaskId2 = thismask2>>8;
            CAN_FilterId3  = thisid2 ;
            CAN_FilterMaskId3 = thismask2 ;
        }
        else if(fcase == 1)// 1-单滤波器模式
        {
            CAN_FilterId0  = thisid>>21;
            CAN_FilterMaskId0 = thismask>>21;
            CAN_FilterId1  = thisid>>13 ;
            CAN_FilterMaskId1 = thismask>>13 ;
            CAN_FilterId2  = thisid>>5;
            CAN_FilterMaskId2 = thismask>>5;
            CAN_FilterId3  = thisid<<3 | rtr<<2;
            CAN_FilterMaskId3 = thismask<<3;
            CAN_FilterMaskId3 |= 0x03;
        }
    }

    /* 进入复位模式 */
    set_reset_mode(CANx);
  
    if(fcase == 1)// 1-单滤波器模式
    {
        /*单滤波模式 */
        CANx->MOD |= (unsigned char)CAN_Mode_AFM;
    }
    else if(fcase == 1)// 0- 双滤波器模式
    {
        /*双滤波模式 */
        CANx->MOD &=(~ (unsigned char) CAN_Mode_AFM);
    }

    CANx->BUF[0] = CAN_FilterId0;
    CANx->BUF[1] =  CAN_FilterId1;
    CANx->BUF[2] =  CAN_FilterId2;
    CANx->BUF[3] =  CAN_FilterId3;
    CANx->BUF[4] =  CAN_FilterMaskId0;
    CANx->BUF[5] = CAN_FilterMaskId1;
    CANx->BUF[6] = CAN_FilterMaskId2;
    CANx->BUF[7] = CAN_FilterMaskId3;
   /* 进入工作模式 */
    set_start(CANx);
}

unsigned char CAN_SetBps(CAN_TypeDef* CANx, Ls1c_CanBPS_t  Bps)
{
    unsigned char InitStatus = CAN_InitStatus_Failed;
    unsigned char  CAN_Prescaler, CAN_BS1, CAN_BS2, CAN_SJW; 
    CAN_SJW = CAN_SJW_1tq;
   /* 进入复位模式 */
  InitStatus = set_reset_mode(CANx);
  if( InitStatus == CAN_InitStatus_Failed)
  	return CAN_InitStatus_Failed;

    /* BaudRate= f(APB)/((1+BS1+BS2)(SJW*2*Prescaler))=126000000/[(1+7+2)*1*2*63]=100000=100K*/
    /* BPS     PRE   BS1   BS2   最低40K
       1M      9       4       2   
       800K    8       7       2   
       500K    9       11      2   
       250K    36      4       2   
       125K    36      11      2   
       100K    63      7       2   
       50K     63      16      3`  
       40K     63      16      8   
    */


    /* BPS     PRE   BS1   BS2   最低40K
       1M      6       5       2   
       800K    6       7       2   
       500K    8       9       2   
       250K    16      9       2   
       125K    24      13      2   
       100K    48      7       2   
       50K     60      13      2`  
       40K     60      15      4   
    */

    switch (Bps)
    {
        case LS1C_CAN1MBaud: 
            CAN_Prescaler = 6;
            CAN_BS1 = CAN_BS1_5tq;
            CAN_BS2 = CAN_BS2_2tq;
        break;
        case LS1C_CAN800kBaud: 
            CAN_Prescaler = 6;
            CAN_BS1 = CAN_BS1_7tq;
            CAN_BS2 = CAN_BS2_2tq;
        break;
        case LS1C_CAN500kBaud: 
            CAN_Prescaler = 8;
            CAN_BS1 = CAN_BS1_9tq;
            CAN_BS2 = CAN_BS2_2tq;
        break;
        case LS1C_CAN250kBaud: 
            CAN_Prescaler = 16;
            CAN_BS1 = CAN_BS1_9tq;
            CAN_BS2 = CAN_BS2_2tq;
        break;
        case LS1C_CAN125kBaud: 
            CAN_Prescaler = 24;
            CAN_BS1 = CAN_BS1_13tq;
            CAN_BS2 = CAN_BS2_2tq;
        break;
        case LS1C_CAN100kBaud: 
            CAN_Prescaler = 48;
            CAN_BS1 = CAN_BS1_7tq;
            CAN_BS2 = CAN_BS2_2tq;
        break;
        case LS1C_CAN50kBaud: 
            CAN_Prescaler = 60;
            CAN_BS1 = CAN_BS1_13tq;
            CAN_BS2 = CAN_BS2_2tq;
        break;
        case LS1C_CAN40kBaud: 
            CAN_Prescaler = 60;
            CAN_BS1 = CAN_BS1_15tq;
            CAN_BS2 = CAN_BS2_4tq;
        break;
        default: //250K
            CAN_Prescaler = 16;
            CAN_BS1 = CAN_BS1_9tq;
            CAN_BS2 = CAN_BS2_2tq;
        break;
    }
  /* 配置时钟频率 */
  CANx->BTR0 = (( unsigned char )CAN_Prescaler -1) | \
               (unsigned char)CAN_SJW << 6;
  
  CANx->BTR1 = ((unsigned char)CAN_BS1) | \
               ((unsigned char)CAN_BS2 << 4) | \
               ((unsigned char)CAN_SJW<<7);

   /* 进入工作模式 */
  set_start(CANx);    
  /* 返回初始化结果 */
  return CAN_InitStatus_Failed;
}


unsigned char CAN_SetMode(CAN_TypeDef* CANx, unsigned char  mode)
{
  unsigned char InitStatus = CAN_InitStatus_Failed;
  unsigned long wait_ack = 0x00000000;
  CAN_InitTypeDef        CAN_InitStructure;

   /* 进入复位模式 */
  InitStatus = set_reset_mode(CANx);
  if( InitStatus == CAN_InitStatus_Failed)
  	return CAN_InitStatus_Failed;

  switch( mode )
  {
    case 0://正常
        CANx->MOD &= ~(unsigned char)CAN_Mode_STM;  
        CANx->MOD &= ~(unsigned char)CAN_Mode_LOM;  
      break;
    case 1://只听
        CANx->MOD &= ~(unsigned char)CAN_Mode_STM;  
        CANx->MOD |= (unsigned char)CAN_Mode_LOM;  
      break;
    case 2://回环
        CANx->MOD |= (unsigned char)CAN_Mode_STM;  
        CANx->MOD &= ~(unsigned char)CAN_Mode_LOM;  
      break;
    case 3://只听回环
        CANx->MOD |= (unsigned char)CAN_Mode_STM;  
        CANx->MOD |= (unsigned char)CAN_Mode_LOM;  
      break;
  }
   /* 进入工作模式 */
  set_start(CANx);    
  
  /* 返回初始化结果 */
  return CAN_InitStatus_Failed;
 }

unsigned char  CAN_Transmit(CAN_TypeDef* CANx, CanTxMsg* TxMessage)
{
    int i;
    if (TxMessage->IDE == CAN_Id_Extended)
    {
        CANx->BUF[1]= TxMessage ->ExtId>> 21;
        CANx->BUF[2]= TxMessage ->ExtId>> 13;
        CANx->BUF[3]= TxMessage ->ExtId>> 5;
        CANx->BUF[4]= TxMessage ->ExtId<<3;
        CANx->BUF[0]= (TxMessage ->IDE & 0x01) << 7 |\
            (TxMessage ->RTR & 0x04) << 6 |\
            (TxMessage ->DLC & 0x0F);
        for( i=0;i<TxMessage ->DLC; i++)
        {
           CANx->BUF[i+5]= TxMessage->Data[i];
        }
    }
    else if (TxMessage->IDE ==CAN_Id_Standard)
    {
        CANx->BUF[1]= TxMessage ->StdId>> 3;
        CANx->BUF[2]= TxMessage ->StdId<< 5;
        CANx->BUF[0]= (TxMessage ->IDE & 0x01) << 7 |\
            (TxMessage ->RTR & 0x01) << 6 |\
            (TxMessage ->DLC & 0x0F);
       // CANx->BUF[3]= TxMessage ->Data[0];
       // CANx->BUF[4]= TxMessage ->Data[1];
        for( i=0;i<TxMessage ->DLC; i++)
        {
           CANx->BUF[i+3]= TxMessage->Data[i];
        }
    }
    CANx->CMR = CAN_CMR_TR ;
}

void CAN_Receive(CAN_TypeDef* CANx,  CanRxMsg* RxMessage)
{
  /* 获取 IDE */
    RxMessage->IDE = (CANx->BUF[0]  & 0x80)>>7;
//  RxMessage->IDE = (CANx->IDE_RTR_DLC  & 0x80)>>7;
  /* 获取 RTR */
    RxMessage->RTR = (CANx->BUF[0]  & 0x40)>>6;
  /* 获取 DLC */
    RxMessage->DLC= (CANx->BUF[0]  & 0x0F);
//  RxMessage->DLC= (CANx->IDE_RTR_DLC  & 0x0F);
  if (RxMessage->IDE == CAN_Id_Standard)
  {
    RxMessage->StdId = CANx->BUF[1]<<3 |CANx->BUF[2]>>5 ;
    /* 获取数据 */
    RxMessage->Data[0] = (unsigned char)CANx->BUF[3];
    RxMessage->Data[1] = (unsigned char)CANx->BUF[4];
    RxMessage->Data[2] = (unsigned char)CANx->BUF[5];
    RxMessage->Data[3] = (unsigned char)CANx->BUF[6];
    RxMessage->Data[4] = (unsigned char)CANx->BUF[7];
    RxMessage->Data[5] = (unsigned char)CANx->BUF[8];
    RxMessage->Data[6] = (unsigned char)CANx->BUF[9];
    RxMessage->Data[7] = (unsigned char)CANx->BUF[10];
  }

  else  if (RxMessage->IDE == CAN_Id_Extended)
  {
    RxMessage->ExtId= CANx->BUF[1]<<21 |CANx->BUF[2]<<13|CANx->BUF[3]<<5|CANx->BUF[4]>>3 ;
    /* 获取数据 */
    RxMessage->Data[0] = (unsigned char)CANx->BUF[5];
    RxMessage->Data[1] = (unsigned char)CANx->BUF[6];
    RxMessage->Data[2] = (unsigned char)CANx->BUF[7];
    RxMessage->Data[3] = (unsigned char)CANx->BUF[8];
    RxMessage->Data[4] = (unsigned char)CANx->BUF[9];
    RxMessage->Data[5] = (unsigned char)CANx->BUF[10];
    RxMessage->Data[6] = (unsigned char)CANx->BUF[11];
    RxMessage->Data[7] = (unsigned char)CANx->BUF[12];
  }  
}
