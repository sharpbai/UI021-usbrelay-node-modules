#include <unistd.h>
#include "usb2io.h"

/*******************打开关闭函数****************************/
//打开端口获取句柄;
//Nbr是打开的端口号，从1开始，依次为2/3/4...，最大126
//如果电脑上只插上一个双头模块，则打开端口号为1
libusb_device_handle *USB2IO_Open(int Nbr)
{
    libusb_device_handle *hDev;

    if ((Nbr < 1) || (Nbr > 126)) {
        return NULL;
    }
    hDev = HID_Open(VID, PID);
    return hDev;
}

int USB2IO_Close(libusb_device_handle *USB2IO_hdl)
{
    return(HID_Close(USB2IO_hdl));
}

int USB2IO_SetIoCfg(libusb_device_handle *USB2IO_hdl, int IoNbr, int IoCfg)
{
    unsigned char buf[256];
    unsigned long dwp_LenResponse;
    unsigned char ucp_Response[32];

    buf[0] = 0;
    buf[1] = IoNbr;
    buf[2] = 0;
    buf[3] = IoCfg;
    if (HID_Ctrl_Comm(USB2IO_hdl,
            4,
            buf,
            &dwp_LenResponse,
            ucp_Response,
            3) != 0x0001) {
        return -1;
    }
    return 0;
}

int USB2IO_SetIoOut(libusb_device_handle *USB2IO_hdl, int IoNbr, int IoOut)
{
    unsigned char buf[256];
    unsigned long dwp_LenResponse;
    unsigned char ucp_Response[32];

    buf[0] = 0;
    buf[1] = IoNbr;
    buf[2] = 1;
    buf[3] = IoOut;
    if (HID_Ctrl_Comm(USB2IO_hdl,
            4,
            buf,
            &dwp_LenResponse,
            ucp_Response,
            3) != 0x0001) {
        return -1;
    }
    return 0;
}

int USB2IO_RdIoCfg(libusb_device_handle *USB2IO_hdl, int IoNbr, int *IoCfg)
{
    unsigned char buf[256];
    unsigned long dwp_LenResponse;
    unsigned char ucp_Response[32];

    buf[0] = 0;
    buf[1] = IoNbr;
    buf[2] = 0x80;
    if (HID_Ctrl_Comm(USB2IO_hdl,
            3,
            buf,
            &dwp_LenResponse,
            ucp_Response,
            3) != 0x0001) {
        return -1;
    }
    *IoCfg = ucp_Response[0];
    return 0;
}

int USB2IO_RdIoOut(libusb_device_handle *USB2IO_hdl, int IoNbr, int *IoOut)
{
    unsigned char buf[256];
    unsigned long dwp_LenResponse;
    unsigned char ucp_Response[32];

    buf[0] = 0;
    buf[1] = IoNbr;
    buf[2] = 0x81;
    if (HID_Ctrl_Comm(USB2IO_hdl,
            4,
            buf,
            &dwp_LenResponse,
            ucp_Response,
            3) != 0x0001) {
        return -1;
    }
    *IoOut = ucp_Response[0];
    return 0;
}

int USB2IO_RdPin(libusb_device_handle *USB2IO_hdl, int IoNbr, int *PinValue)
{
    unsigned char buf[256];
    unsigned long dwp_LenResponse;
    unsigned char ucp_Response[32];

    buf[0] = 0;
    buf[1] = IoNbr;
    buf[2] = 0x82;
    if (HID_Ctrl_Comm(USB2IO_hdl,
            3,
            buf,
            &dwp_LenResponse,
            ucp_Response,
            3) != 0x0001) {
        return -1;
    }
    *PinValue = ucp_Response[0];
    return 0;
}

int USB2IO_SetClkOut(libusb_device_handle *USB2IO_hdl, int ClkOutCfg)
{
    unsigned char buf[256];
    unsigned long dwp_LenResponse;
    unsigned char ucp_Response[32];

    buf[0] = 1;
    buf[1] = ClkOutCfg;
    if (HID_Ctrl_Comm(USB2IO_hdl,
            2,
            buf,
            &dwp_LenResponse,
            ucp_Response,
            3) != 0x0001) {
        return -1;
    }
    return 0;
}

int USB2IO_RdClkOutCfg(libusb_device_handle *USB2IO_hdl, int *ClkOutCfg)
{
    unsigned char buf[256];
    unsigned long dwp_LenResponse;
    unsigned char ucp_Response[32];

    buf[0] = 1;
    buf[1] = 0x80;
    if (HID_Ctrl_Comm(USB2IO_hdl,
            2,
            buf,
            &dwp_LenResponse,
            ucp_Response,
            3) != 0x0001) {
        return -1;
    }
    *ClkOutCfg = ucp_Response[0];
    return 0;
}

//#define PIN_SPI_SS     2  ; app control
#define PIN_SPI_SCK    8
#define PIN_SPI_MOSI   9
#define PIN_SPI_MISO   3
int USB2IO_EnableSpi(libusb_device_handle *USB2IO_hdl)
{
    if (USB2IO_SetIoOut(USB2IO_hdl, PIN_SPI_MOSI, 1) != 0) {
        return -1;
    }
    usleep(1000); //1ms
    if (USB2IO_SetIoOut(USB2IO_hdl, PIN_SPI_MISO, 1) != 0) {
        return -1;
    }
    usleep(1000); //1ms
    if (USB2IO_SetIoCfg(USB2IO_hdl, PIN_SPI_SCK, 3) != 0) {
        return -1;
    }
    usleep(1000); //1ms
    if (USB2IO_SetIoCfg(USB2IO_hdl, PIN_SPI_MOSI, 3) != 0) {
        return -1;
    }
    usleep(1000); //1ms
    if (USB2IO_SetIoCfg(USB2IO_hdl, PIN_SPI_MISO, 0)) {//in
        return -1;
    }
    return 0;
}

int USB2IO_SpiWrite(libusb_device_handle *USB2IO_hdl, int DataLength, char *DataBuf)
{
    unsigned char buf[256];
    unsigned long dwp_LenResponse;
    unsigned char ucp_Response[32];

    buf[0] = 2;
    buf[1] = DataLength;
    buf[2] = 0;
    memcpy((char *)(&buf[3]), DataBuf, DataLength);
    if (HID_Ctrl_Comm(USB2IO_hdl,
            DataLength + 3,
            buf,
            &dwp_LenResponse,
            ucp_Response,
            3) != 0x0001) {
        return -1;
    }
    return 0;
}

int USB2IO_SpiRead(libusb_device_handle *USB2IO_hdl, int DataLength, char *DataBuf)
{
    unsigned char buf[256];
    unsigned long dwp_LenResponse;

    buf[0] = 2;
    buf[1] = DataLength;
    buf[2] = 1;
    if (HID_Ctrl_Comm(USB2IO_hdl,
            3,
            buf,
            &dwp_LenResponse,
            (unsigned char *)DataBuf,
            3) != 0x0001) {
        return -1;
    }
    return 0;
}

//设置SPI MODE: Mode0~3
int USB2IO_SetSpiMode(libusb_device_handle *USB2IO_hdl, int SpiMode)
{
    unsigned char buf[256];
    unsigned long dwp_LenResponse;
    unsigned char ucp_Response[32];

    if ((SpiMode > 3) || (SpiMode < 0)) {
        return -1;
    }
    else if (SpiMode == 0){ //转成051支持的模式
        SpiMode = 1;
    }
    else if (SpiMode == 1){ //转成051支持的模式
        SpiMode = 0;
    }
    buf[0] = 2;
    buf[1] = SpiMode; //length
    buf[2] = 0x80;
    if (HID_Ctrl_Comm(USB2IO_hdl,
            3,
            buf,
            &dwp_LenResponse,
            ucp_Response,
            3) != 0x0001) {
        return -1;
    }
    return 0;
}
//读取SPI MODE: Mode0~3
int USB2IO_GetSpiMode(libusb_device_handle *USB2IO_hdl, int *SpiMode)
{
    unsigned char buf[256];
    unsigned long dwp_LenResponse;
    unsigned char ucp_Response[32];

    buf[0] = 2;
    buf[1] = 1; //length
    buf[2] = 0x81;
    if (HID_Ctrl_Comm(USB2IO_hdl,
            3,
            buf,
            &dwp_LenResponse,
            ucp_Response,
            3) != 0x0001) {
        return -1;
    }
    *SpiMode = ucp_Response[0];
    if ((*SpiMode > 3) || (*SpiMode < 0)) {
        return -1;
    }
    else if (*SpiMode == 0){ //转成app需要的模式
        *SpiMode = 1;
    }
    else if (*SpiMode == 1){ //转成app需要的模式
        *SpiMode = 0;
    }
    return 0;
}

#define PIN_I2C_SCL    6
#define PIN_I2C_SDA    7
int USB2IO_EnableI2c(libusb_device_handle *USB2IO_hdl)
{
    if (USB2IO_SetIoOut(USB2IO_hdl, PIN_I2C_SCL, 0) != 0) {//先输出低，避免发出Start或stop
        return -1;
    }
    usleep(1000); //1ms
    if (USB2IO_SetIoOut(USB2IO_hdl, PIN_I2C_SDA, 1) != 0) {
        return -1;
    }
    usleep(1000); //1ms
    if (USB2IO_SetIoCfg(USB2IO_hdl, PIN_I2C_SCL, 2) != 0) {//od
        return -1;
    }
    usleep(1000); //1ms
    if (USB2IO_SetIoCfg(USB2IO_hdl, PIN_I2C_SDA, 2) != 0) {
        return -1;
    }
    usleep(1000); //1ms
    if (USB2IO_SetIoOut(USB2IO_hdl, PIN_I2C_SCL, 1) != 0) {//输出高
        return -1;
    }
    return 0;
}

//StopFlag: 0=不发STOP  1=发STOP;
int USB2IO_I2cWriteNoCs(libusb_device_handle *USB2IO_hdl, int I2cAddr, int DataLength, char *DataBuf, char StopFlag)
{
    unsigned char buf[256];
    unsigned long dwp_LenResponse;
    unsigned char ucp_Response[32];

    int AddrCnt = 1; //默认
    int CmdLen = 4; //默认
    if ((DataLength < 1) || (DataLength > 141)) { //这里的长度 需要考虑已经加上CmdRegAddrByteCnt
        return -1;
    }
    if (I2cAddr > 0x3FF) {
        return -1;
    }
    else if (I2cAddr > 0x7F) {
        AddrCnt = 2;
        buf[3] = (0xF0 | ((char)(I2cAddr >> 8) << 1)); //0x11110XX0
        buf[4] = (char)I2cAddr;
        CmdLen = 5;
    }
    else {
        buf[3] = (char)(I2cAddr << 1);
    }
    buf[0] = 3;
    buf[1] = DataLength;
    buf[2] = 0x00 | AddrCnt;
    memcpy((char *)(&buf[CmdLen]), DataBuf, DataLength);
    buf[DataLength + CmdLen] = StopFlag;
    if (HID_Ctrl_Comm(USB2IO_hdl,
            DataLength + CmdLen + 1,  //+1 StopFlag
            buf,
            &dwp_LenResponse,
            ucp_Response,
            3) != 0x0001) {
        return -1;
    }
    return 0;
}

int USB2IO_I2cWrite(libusb_device_handle *USB2IO_hdl, int I2cAddr, int CmdRegAddr, int CmdRegAddrByteCnt, int DataLength, char *DataBuf)
{
    unsigned char buf[256];
    int i;
    
    if ((DataLength < 1) || (DataLength > 136) || (CmdRegAddrByteCnt > 4) || (CmdRegAddrByteCnt < 0)) {
        return -1;
    }
    if (I2cAddr > 0x3FF) {
        return -1;
    }
    for (i = 0; i < CmdRegAddrByteCnt; i++) {
        buf[CmdRegAddrByteCnt - i - 1] = (char)CmdRegAddr;
        CmdRegAddr >>= 8;
    }
    memcpy((char *)(&buf[CmdRegAddrByteCnt]), DataBuf, DataLength);
    if (USB2IO_I2cWriteNoCs(USB2IO_hdl, I2cAddr, DataLength + CmdRegAddrByteCnt, (char *)buf, 1) != 0) {
        return -1;
    }
    return 0;
}

int USB2IO_I2cRead(libusb_device_handle *USB2IO_hdl, int I2cAddr, int CmdRegAddr, int CmdRegAddrByteCnt, int DataLength, char *DataBuf)
{
    unsigned char buf[256];
    unsigned long dwp_LenResponse;
    int i;
    int AddrCnt;
    int CmdLen;
    
    if ((DataLength < 1) || (DataLength > 136) || (CmdRegAddrByteCnt > 4) || (CmdRegAddrByteCnt < 0)) {
        return -1;
    }
    if (CmdRegAddrByteCnt > 0) {//需要先发W，再发Read
        for (i = 0; i < CmdRegAddrByteCnt; i++) {
            buf[CmdRegAddrByteCnt - i - 1] = (char)CmdRegAddr;
            CmdRegAddr >>= 8;
        }
        if (USB2IO_I2cWriteNoCs(USB2IO_hdl, I2cAddr, CmdRegAddrByteCnt, (char *)buf, 0) != 0) {//Write 不发STOP
            return -1;
        }
    }
    AddrCnt = 1; //默认
    CmdLen = 4; //默认
    if (I2cAddr > 0x3FF) {
        return -1;
    }
    else if (I2cAddr > 0x7F) {
        AddrCnt = 2;
        buf[3] = (0xF1 | ((char)(I2cAddr >> 8) << 1)); //0x11110XX1
        buf[4] = (char)I2cAddr;
        CmdLen = 5;
    }
    else {
        buf[3] = 0x01 | ((char)(I2cAddr << 1));
    }
    buf[0] = 3;
    buf[1] = DataLength;
    buf[2] = 0x80 | AddrCnt;
    if (HID_Ctrl_Comm(USB2IO_hdl,
            CmdLen,
            buf,
            &dwp_LenResponse,
            (unsigned char *)DataBuf,
            3) != 0x0001) {
        return -1;
    }
    return 0;
}

int USB2IO_SetI2cSpeed(libusb_device_handle *USB2IO_hdl, int I2cSpeed)
{
    unsigned char buf[256];
    unsigned long dwp_LenResponse;
    unsigned char ucp_Response[32];

    if ((I2cSpeed > 1) || (I2cSpeed < 0)) {
        return -1;
    }
    buf[0] = 3;
    buf[1] = I2cSpeed;
    buf[2] = 0x40;
    if (HID_Ctrl_Comm(USB2IO_hdl,
            3,
            buf,
            &dwp_LenResponse,
            ucp_Response,
            3) != 0x0001) {
        return -1;
    }
    return 0;
}

int USB2IO_GetI2cSpeed(libusb_device_handle *USB2IO_hdl, int *I2cSpeed)
{
    unsigned char buf[256];
    unsigned long dwp_LenResponse;
    unsigned char ucp_Response[32];

    buf[0] = 3;
    buf[1] = 1; //length
    buf[2] = 0xC0;
    if (HID_Ctrl_Comm(USB2IO_hdl,
            3,
            buf,
            &dwp_LenResponse,
            ucp_Response,
            3) != 0x0001) {
        return -1;
    }
    *I2cSpeed = ucp_Response[0];
    if ((*I2cSpeed > 1) || (*I2cSpeed < 0)) {
        return -1;
    }
    return 0;
}

#define PIN_UART_TX    4
#define PIN_UART_RX    5
int USB2IO_EnableUart(libusb_device_handle *USB2IO_hdl)
{
    if (USB2IO_SetIoOut(USB2IO_hdl, PIN_UART_TX, 1) != 0) {
        return -1;
    }
    usleep(1000); //1ms
    if (USB2IO_SetIoOut(USB2IO_hdl, PIN_UART_RX, 1) != 0) {
        return -1;
    }
    usleep(1000); //1ms
    if (USB2IO_SetIoCfg(USB2IO_hdl, PIN_UART_TX, 3) != 0) {
        return -1;
    }
    usleep(1000); //1ms
    if (USB2IO_SetIoCfg(USB2IO_hdl, PIN_UART_RX, 2)) {//OD
        return -1;
    }
    return 0;
}

//返回值：0:成功; 1:接收超时，接收到的实际数据长度为ActualReadDataLength; -1:失败
//WrLen: 待发送的数据长度(取值范围0~136;0表示无数据发送)
//WrBuf: 待发送的数据buf
//ExpRdLen: 期望接收的数据长度(取值范围0~136;0表示无需接收数据)
//ActRdLen: 实际接收到的数据长度(取值范围0~136;0表示无需接收数据)
//RdBuf: 返回数据的数据buf(大小至少为256字节)
//TimeOut:接收数据才需要；等待数据的时间(单位: s)。取值范围: 3~600s(也就是10分钟)
int USB2IO_UartWrRd(libusb_device_handle *USB2IO_hdl, int WrLen, char *WrBuf, int ExpRdLen, int *ActRdLen, char *RdBuf, int TimeOut)
{
    unsigned char buf[256];
	int ret;

    if ((WrLen > 136) || (ExpRdLen > 136)) {
        return -1;
    }
    buf[0] = 11;
    buf[1] = WrLen;
    buf[2] = ExpRdLen;
    memcpy((char *)(&buf[3]), WrBuf, WrLen);
    ret = HID_Ctrl_Comm(USB2IO_hdl,
            WrLen+3,
            buf,
            (unsigned long *)ActRdLen,
            (unsigned char *)RdBuf,
            TimeOut);
    if (ret != 0x0001) {
        if (ret == 0xF0FE) {

            return 1;
        }
        return -1;
    }
    return 0;
}

//设置波特率: UartBr=115200;9600
int USB2IO_SetUartBr(libusb_device_handle *USB2IO_hdl, int UartBr)
{
    unsigned char buf[256];
    unsigned long dwp_LenResponse;
    unsigned char ucp_Response[32];

    if ((UartBr != 115200) && (UartBr != 9600)) {
        return -1;
    }
    if (UartBr == 115200) {
        buf[1] = 1;
    }
    else if (UartBr == 9600) {
        buf[1] = 0;
    }
    buf[0] = 10;
    if (HID_Ctrl_Comm(USB2IO_hdl,
            2,
            buf,
            &dwp_LenResponse,
            ucp_Response,
            3) != 0x0001) {
        return -1;
    }
    return 0;
}
//读取波特率:  9600 or 115200
int USB2IO_GetUartBr(libusb_device_handle *USB2IO_hdl, int *UartBr)
{
    unsigned char buf[256];
    unsigned long dwp_LenResponse;
    unsigned char ucp_Response[32];

    buf[0] = 9;
    if (HID_Ctrl_Comm(USB2IO_hdl,
            1,
            buf,
            &dwp_LenResponse,
            ucp_Response,
            3) != 0x0001) {
        return -1;
    }
    *UartBr = ucp_Response[0];
    if (*UartBr == 1) {
        *UartBr = 115200;
    }
    else if (*UartBr == 0) {
        *UartBr = 9600;
    }
    return 0;
}