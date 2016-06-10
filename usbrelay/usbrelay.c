#include <unistd.h>
#include "usbrelay.h"

#define Level2OpenRelay  0
/*******************打开关闭函数****************************/
//打开端口获取句柄;
//Nbr是打开的端口号，从1开始，依次为2/3/4...，最大126
libusb_device_handle *USBRELAY_Open(int Nbr)
{
    return(USB2IO_Open(Nbr));
}

int USBRELAY_Close(libusb_device_handle *USBRELAY_hdl)
{
    return(USB2IO_Close(USBRELAY_hdl));
}

//RelayNbr:  继电器位置，从1开始
//OpCode: 0是关闭；1是打开
//返回： -1=err; -2=操作没有成功; 0=OK
const int IO_RELAY_MAP[] = {1, 2, 4, 5};
const int LEDIO_RELAY_MAP[] = {7, 8, 11, 12};
int USBRELAY_SetRelay(libusb_device_handle *USBRELAY_hdl, int RelayNbr, int OpCode)
{
    int IoNbr;
	int Level2Set;
	int IoCfg;
    int Delay = 20; //delay 1s
	int RdVal;

    if ((RelayNbr < 0) || (RelayNbr > 4)) {
        return -1;
    }
    IoNbr = IO_RELAY_MAP[RelayNbr - 1];
    //不管当前PIN电平，直接设置输出电平
    if (OpCode == 0) {
        Level2Set = 1 - Level2OpenRelay;  //关闭
    }
    else if (OpCode == 1) {
        Level2Set = Level2OpenRelay;      //打开
    }
    else {
        return -1;
    }
    if (USB2IO_SetIoOut(USBRELAY_hdl, IoNbr, Level2Set) != 0) {
        return -1;
    }
    //判断当前IO是否是PP OUT，如果不是，则设置为PP OUT
    if (USB2IO_RdIoCfg(USBRELAY_hdl, IoNbr, &IoCfg) != 0) {
        return -1;
    }
    if (IoCfg != 3) {
        if (USB2IO_SetIoCfg(USBRELAY_hdl, IoNbr, 3) != 0) {
            return -1;
        }
    }
    //读取当前PIN是否已经是想要的电平
    Delay = 20; //delay 1s
    while(Delay) {
        usleep(50000); //50ms
        Delay--;
        if (USB2IO_RdPin(USBRELAY_hdl, IoNbr, &RdVal) != 0) {
            return -1;
        }
        if (RdVal == Level2Set) {
            //设置led
            USB2IO_SetIoOut(USBRELAY_hdl, LEDIO_RELAY_MAP[RelayNbr - 1], OpCode);
            return 0;
        }
        else {
            USB2IO_SetIoOut(USBRELAY_hdl, IoNbr, Level2Set);
        }
    }
    return -2;
}

//读取继电器状态
//RelayStatus: 0关闭； 1打开
int USBRELAY_GetRelay(libusb_device_handle *USBRELAY_hdl, int RelayNbr, int *RelayStatus)
{
    int RdVal;
    int IoNbr;
    
    if ((RelayNbr < 0) || (RelayNbr > 4)) {
        return -1;
    }
    IoNbr = IO_RELAY_MAP[RelayNbr - 1];
    if (USB2IO_RdPin(USBRELAY_hdl, IoNbr, &RdVal) != 0) {
        return -1;
    }
    if (RdVal == Level2OpenRelay) {
        *RelayStatus = 1;
    }
    else if (RdVal == (1 - Level2OpenRelay)) {
        *RelayStatus = 0;
    }
    else {
        return -1;
    }
    return 0;
}