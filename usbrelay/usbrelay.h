//------------------------------------------------------------------------------//
//          北京飞易来科技有限公司                                              //
//       USB继电器应用编程接口(API)说明                                         //
//                                                                              //
//    本API适用于                                                               //
//    1. 1/2/4路USB继电器                                                       //
//    2. Linux2.6.16及以上操作系统                                              //
//                                                                              //
//    详细请登陆北京飞易来科技有限公司官方网站http://freeeasy-life.com          //
//------------------------------------------------------------------------------//

#ifndef  _USBRELAY_H
#define  _USBRELAY_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "usb2io.h"

/***********设备操作函数;以下函数中的USBRELAY_hdl是指USBRELAY_Open返回的句柄*******/
//打开端口获取句柄;  //Nbr是端口号，从1开始，依次为2/3/4...，最大126
libusb_device_handle *USBRELAY_Open(int Nbr);
//关闭端口；在程序退出前再关闭端口; 返回 0: 成功；!0: 失败
int USBRELAY_Close(libusb_device_handle *USBRELAY_hdl);

/***********继电器操作函数;以下函数中的USBRELAY_hdl是指USBRELAY_Open返回的句柄*******/
//设置继电器
//RelayNbr:  继电器位置，从1开始
//OpCode: 0是关闭；1是打开
//返回： -1=err; -2=操作没有成功; 0=OK
int USBRELAY_SetRelay(libusb_device_handle *USBRELAY_hdl, int RelayNbr, int OpCode);
//读取继电器状态
//RelayStatus: 0关闭； 1打开
//返回： -1=err;  0=OK
int USBRELAY_GetRelay(libusb_device_handle *USBRELAY_hdl, int RelayNbr, int *RelayStatus);

#ifdef __cplusplus
}
#endif

#endif


