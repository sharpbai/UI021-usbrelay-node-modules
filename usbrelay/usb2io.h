//------------------------------------------------------------------------------//
//          北京飞易来科技有限公司                                              //
//         UI051 应用编程接口(API)说明                                          //
//                                                                              //
//    本API适用于                                                               //
//    1. UI051 USB2IO芯片                                                       //
//    2. Linux2.6.16操作系统                                                    //
//                                                                              //
//    详细请登陆北京飞易来科技有限公司官方网站http://freeeasy-life.com          //
//------------------------------------------------------------------------------//
#ifndef  _usb2io_H
#define  _usb2io_H

#include "hidcomm.h"

#define VID 0xC216
#define PID 0x0105
/***********设备操作函数;以下函数中的USB2IO_hdl是指USB2IO_Open返回的句柄*******/
//打开端口获取句柄;  //Nbr是端口号，从1开始，依次为2/3/4...，最大126
libusb_device_handle *USB2IO_Open(int Nbr);
//关闭端口；在程序退出前再关闭端口; 返回 0: 成功；!0: 失败
int USB2IO_Close(libusb_device_handle *USB2IO_hdl);

/***********IO操作函数;以下函数中的USB2IO_hdl是指USB2IO_Open返回的句柄*******/
/** 以下所有接口函数                                                       **/
/**  IoNbr: IO口(1~10)                                                     **/
/**  返回值 0: 成功；!0: 失败                                              **/
//设置IO输出方式：IoCfg:0=输入; IoCfg:2=OD+上拉, 3=PushPull推挽输出;
int USB2IO_SetIoCfg(libusb_device_handle *USB2IO_hdl, int IoNbr, int IoCfg);
//设置IO输出值：  IoOut:1=输出高, 0=输出低
int USB2IO_SetIoOut(libusb_device_handle *USB2IO_hdl, int IoNbr, int IoOut);
//读取IO输出配置, 读取值存放在IoCfg中
int USB2IO_RdIoCfg(libusb_device_handle *USB2IO_hdl, int IoNbr, int *IoCfg);
//读取IO输出值(这里读取到的是上位机用USB2IO_SetIoOut的设置值), 读取值存放在IoOut中
int USB2IO_RdIoOut(libusb_device_handle *USB2IO_hdl, int IoNbr, int *IoOut);
//读取IO引脚值(这里读取到的是实际引脚的电平值), 读取值存放在PinValue中
//注意：IO输出值是设置IO输出寄存器；IO引脚值是芯片引脚上的实际电平值。
//      两者一般情况下是相等的，但如果该IO受到外围MCU或其他外设影响，两者有可能不相等，此时需要仔细查看外围电路是否有问题
int USB2IO_RdPin(libusb_device_handle *USB2IO_hdl, int IoNbr, int *PinValue);
//读取ClkOut输出： ClkOutCfg: 1=输出Clk, 0=关闭Clk
int USB2IO_SetClkOut(libusb_device_handle *USB2IO_hdl, int ClkOutCfg);
//读取ClkOut的输出配置, 读取值存放在ClkOutCfg中
int USB2IO_RdClkOutCfg(libusb_device_handle *USB2IO_hdl, int *ClkOutCfg);

/***********SPI操作函数;以下函数中的USB2IO_hdl是指USB2IO_Open返回的句柄******/
/** 以下所有接口函数                                                       **/
/**  返回值 0: 成功；!0: 失败                                              **/
/*********标准的SPI MODE 定义如下********************************************/
/**    MODE0 CPOL=0 CPHA=0 SPI_CLK空闲时是低电平; 上升沿采样               **/ 
/**    MODE1 CPOL=0 CPHA=1 SPI_CLK空闲时是低电平; 下降沿采样               **/
/**    MODE2 CPOL=1 CPHA=0 SPI_CLK空闲时是高电平; 下降沿采样               **/
/**    MODE3 CPOL=1 CPHA=1 SPI_CLK空闲时是高电平; 上升沿采样               **/
/****************************************************************************/
//使能SPI接口，SPI接口的相关IO配置将自动改变
int USB2IO_EnableSpi(libusb_device_handle *USB2IO_hdl);
//SPI写: DataLength:数据长度(1~136字节); DataBuf:数据buf
int USB2IO_SpiWrite(libusb_device_handle *USB2IO_hdl, int DataLength, char *DataBuf);
//SPI读: DataLength:期望读的数据长度(1~136字节); ; DataBuf:数据buf(大小至少为256字节)
int USB2IO_SpiRead(libusb_device_handle *USB2IO_hdl, int DataLength, char *DataBuf);
//设置SPI MODE: Mode0~3
int USB2IO_SetSpiMode(libusb_device_handle *USB2IO_hdl, int SpiMode);
//读取SPI MODE: Mode0~3
int USB2IO_GetSpiMode(libusb_device_handle *USB2IO_hdl, int *SpiMode);

/***********I2C操作函数;以下函数中的USB2IO_hdl是指USB2IO_Open返回的句柄******/
/** 以下所有接口函数                                                       **/
/**  返回值 0: 成功；!0: 失败                                              **/
//使能I2C接口，I2C接口的相关IO配置将自动改变
int USB2IO_EnableI2c(libusb_device_handle *USB2IO_hdl);
//I2C写: 
//I2cAddr:I2C设备地址; 
//CmdRegAddr: 命令或寄存器地址, 只有当CmdRegAddrExistFlag!=0时有效; 
//CmdRegAddrByteCnt: CmdRegAddr的长度(0~4)(单位:字节)  0: 不存在CmdRegAddr
//DataLength:数据长度(1~136字节); 
//DataBuf:数据buf
int USB2IO_I2cWrite(libusb_device_handle *USB2IO_hdl, int I2cAddr, int CmdRegAddr, int CmdRegAddrByteCnt, int DataLength, char *DataBuf);
//I2C读: 
//I2cAddr:I2C设备地址; 
//CmdRegAddr: 命令或寄存器地址, 只有当CmdRegAddrByteCnt!=0时有效; 
//CmdRegAddrByteCnt: CmdRegAddr的长度(0~4)(单位:字节)  0: 不存在CmdRegAddr
//DataLength:期望读的数据长度(1~136字节); 
//DataBuf:返回数据的数据buf(大小至少为256字节)
int USB2IO_I2cRead(libusb_device_handle *USB2IO_hdl, int I2cAddr, int CmdRegAddr, int CmdRegAddrByteCnt, int DataLength, char *DataBuf);
//设置I2C速度: 0:80k 1:160k
int USB2IO_SetI2cSpeed(libusb_device_handle *USB2IO_hdl, int I2cSpeed);
//读取I2C速度: 0:80k 1:160k
int USB2IO_GetI2cSpeed(libusb_device_handle *USB2IO_hdl, int *I2cSpeed);

/***********UART操作函数;以下函数中的USB2IO_hdl是指USB2IO_Open返回的句柄******/
/**  以下所有接口函数                                                       **/
/**  返回值 0: 成功；!0: 失败                                               **/
//使能UART接口，UART接口的相关IO配置将自动改变
int USB2IO_EnableUart(libusb_device_handle *USB2IO_hdl);
//UART发送接收
//返回值：0:成功; 1:接收超时，接收到的实际数据长度为ActualReadDataLength; -1:失败
//WrLen: 待发送的数据长度(取值范围0~136;0表示无数据发送)
//WrBuf: 待发送的数据buf
//ExpRdLen: 期望接收的数据长度(取值范围0~136;0表示无需接收数据)
//ActRdLen: 实际接收到的数据长度(取值范围0~136;0表示无需接收数据)
//RdBuf: 返回数据的数据buf(大小至少为256字节)
//TimeOut:接收数据才需要；等待数据的时间(单位: s)。取值范围: 3~600s(也就是10分钟)
int USB2IO_UartWrRd(libusb_device_handle *USB2IO_hdl, int WrLen, char *WrBuf, int ExpRdLen, int *ActRdLen, char *RdBuf, int TimeOut);
//设置波特率: UartBr=115200或9600
int USB2IO_SetUartBr(libusb_device_handle *USB2IO_hdl, int UartBr);
//读取波特率 115200或9600
int USB2IO_GetUartBr(libusb_device_handle *USB2IO_hdl, int *UartBr);

#endif