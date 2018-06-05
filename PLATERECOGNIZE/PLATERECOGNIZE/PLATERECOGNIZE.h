// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 PLATERECOGNIZE_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// PLATERECOGNIZE_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。

/*
版本标识
版本号	修改时间	修订内容	修订人
Ver1.0.5	20180408	增加车牌识别失败后反馈	中远海科
Ver1.0.6	20180409	新增要求支持的操作系统	中远海科
Ver1.0.7	20180423	修改关闭时间字符叠加方法名称
增加字符叠加位置的参数说明	中远海科
Ver1.0.8	20180427	根据路网中心要求，大幅修改	中远海科
Ver1.0.10	20180516	接口3，接口13修正车牌识别接口函数名	中远海科

1	车牌识别要求：
1）	车牌识别结果应包含车牌和车牌颜色
2）	车牌必须符合交通部、公安部要求，为汉字、数字，字母组成，所有汉字字符集符合GB2312规范，不得超越该字符集
3）	车牌不得包含乱码，不得包含特殊字符：（“！@#|￥$%……&*（”《》，。、——=-）等均是无效字符）
4）	车牌颜色应支持新能源汽车车牌颜色
5）	车牌识别响应速度要求控制在200ms以内。
说明：200ms指的是触发线圈到车牌识别给到收费系统识别数据的周期，200ms用以描述车牌识别的时间性能。


2	动态链接库说明
1）	动态库名：PLATERECOGNIZE.DLL。
2）	该动态库采用静态调用方式，调用约定为_stdcall调用方式。
3）	动态库能满足主流开发工具的调用要求，包括Visual Basic 6.0、Visual C++ 6.0、C#,JAVA等。
4）	由设备厂家提供符合接口函数定义的PLATERECOGNIZE.DLL文件。
5）	设备厂家应该提供调用PLATERECOGNIZE.dll的Demo软件
6）
7）	PLATERECOGNIZE.dll不得使用配置文件，有需要的配置信息均只能通过初始化函数传输
8）	PLATERECOGNIZE.dll要求覆盖本设备厂家的所有设备型号，同一个厂家不得出现多个硬件版本动态库文件,高版本动态库需向下兼容低版本硬件
9）	本动态链接库要求支持Windows x86和x64操作系统，对常见（XP，Win7,Win10）操作系统支持
*/

/*
一、消息机制说明
硬件触发后通过消息通知给收费软件，收费软件去获取相关的信息

1．	消息定义
消息	Msg			    自定义
2．	消息ID定义
消息ID以WPARAM参数传递(LPARAM=0即可)，通过不同的ID区分不同的事件，消息ID定义如下：
WPARAM                LPARAM
硬触发识别后上报事件	      1				       0
*/
#ifdef __cplusplus
#define COMPILER_TYPE  extern "C"
//#define COMPILER_TYPE 
#endif

#define CALLING_CONVENTION WINAPI

#ifdef PLATERECOGNIZE_EXPORTS
#define PLATERECOGNIZE_API COMPILER_TYPE __declspec(dllexport)
#else
#define PLATERECOGNIZE_API COMPILER_TYPE __declspec(dllimport)
#endif

//************************************
// Method:        Plate_InitDevice
// Describe:
// FullName:      Plate_InitDevice
// Access:          public 
// Returns:        BOOL
// Returns  Describe:                   TRUE为初始化成功，FALSE为初始化失败
// Parameter:    HWND hHandle  接收消息的窗口的句柄
// Parameter:    int Msg             识别数据消息号
// Parameter:    int PorType       端口类型
//                                              1：串口
//                                              2：网口
// Parameter:    char * PortName  要初始化的串口号
//                                              若是串口，则为串口号“Com1”, “Com2”等
//                                              若是网口，则为IP地址，如“192.168.1.11”
// Parameter:    long Baudrate   串口波特率，是网口时，填0
// Parameter:    char * Px          播放视频分辨率，默认格式为704*576，采用*号字符分割
//************************************
PLATERECOGNIZE_API BOOL CALLING_CONVENTION Plate_InitDevice(HWND hHandle, int Msg, int PorType, char *  PortName, long Baudrate, char * Px);


//************************************
// Method:        Plate_CloseDevice
// Describe:        关闭已经打开的车牌识别链接
// FullName:      Plate_CloseDevice
// Access:          public 
// Returns:        BOOL
// Returns  Describe:    TRUE为关闭成功，FALSE为关闭失败
//************************************
PLATERECOGNIZE_API BOOL CALLING_CONVENTION Plate_CloseDevice();

//************************************
// Method:        Plate_GetInfo
// Describe:        获取牌照识别结果
// FullName:      Plate_GetInfo
// Access:          public 
// Returns:        BOOL
// Returns  Describe:  TRUE为识别成功，FALSE为识别失败
// Parameter:    char * PlateInfor  牌照识别结果
// Parameter:    int * ColorInfo   车牌颜色识别结果
//                                               车牌颜色为交通部国标定义：
//                                                1位数字
//                                                0 - 蓝色，1 - 黄色，
//                                                2 - 黑色，3 - 白色，
//                                                4 - 渐变绿色，5 - 黄绿双拼色
//                                                6 - 蓝白渐变色
//                                                9 - 不确定
// Parameter:    char * PicId         上位机传入序列号，作为本次车牌识别结果和图片一一对应关系，当上位机用Plate_GetImage取车辆图片时再次传输参数
//************************************
PLATERECOGNIZE_API BOOL CALLING_CONVENTION Plate_GetInfo(char * PlateInfor, int * ColorInfo, char * PicId);

//************************************
// Method:        Plate_GetImage
// Describe:        获取牌照识别二值化图片信息、车辆抓拍图片信息。一般情况下，调用Plate_GetInfo后，调用该函数获取车牌识别结果对应的图片信息；
//                      否则，则为调用最新的图片信息。图片需持续缓存，只有新的抓拍触发才能将本次图片清除.
// FullName:      Plate_GetImage
// Access:          public 
// Returns:        BOOL
// Returns Describe: TRUE为获取图片成功，FALSE为获取图片失败
// Parameter:    int ImageType   
//                                              0：二值化牌照图片
//                                              1：车牌彩色小图
//                                              2：车头彩色图片
//                                              3: 完整的大照片（压缩到150kb左右）
//
// Parameter:    char * ImageInfo    
//                                              二值化图片的路径和文件名（JPG类型）
//
// Parameter:    char * PicId       
//                                              位机传入序列号，作为本次车牌识别结果和图片一一对应关系，动态库返回的图片需与Plate_Getinfo形成对应关系，
//                                              当找不到本PicId时，车牌识别需重新抓拍一张图片并进行字符叠加
//
// Parameter:    char * Overlay   
//                                              字符叠加信息，当Overlay超过一行所能叠加长度时，或者带有回车换行符时，进行换行处理。
//                                              字符叠加为固定的额外区域，在照片的最下面，不覆盖真实照片区域，当出现同一个车多次调用本函数时，需清空并覆盖上次的字符叠加区域。
//                                              Overlay为空字符串时，为清除字符叠加区域
//************************************
PLATERECOGNIZE_API BOOL CALLING_CONVENTION Plate_GetImage(int ImageType, char * ImageInfo, char * PicId, char * Overlay);

//************************************
// Method:        Plate_GetState
// Describe:        获取牌照识别器状态
// FullName:      Plate_GetState
// Access:          public 
// Returns:        BOOL
// Returns Describe:    TRUE为获取状态成功，FALSE为获取状态失败
// Parameter:    int * State    0：正常  ; 1：故障
//************************************
PLATERECOGNIZE_API BOOL CALLING_CONVENTION Plate_GetState(int* State);

//************************************
// Method:        Plate_Capture
// Describe:        软件触发抓拍识别，调用该函数即可抓拍识别一次
// FullName:      Plate_Capture
// Access:          public 
// Returns:        BOOL
// Returns Describe:        TRUE为抓拍成功，软件等待收到动态库上报事件识别结果消息号就可以取识别结果了；FALSE为抓拍失败
// Notice:   注意，由于识别单元2秒内只能响应一次触发识别或是模拟抓拍识别，因此不要让两次识别间隔太紧。
//************************************
PLATERECOGNIZE_API BOOL CALLING_CONVENTION Plate_Capture();

//************************************
// Method:        Plate_InitVideo
// Describe:        将车牌识别设备视频显示到指定控件上。若重复调用该接口，且hHandle不相同时，表示在多个控件上显示。车牌识别设备断开重连之后，之前所传入句柄依然有效
// FullName:      Plate_InitVideo
// Access:          public 
// Returns:        BOOL
// Returns Describe:    TRUE为视频显示成功，FALSE为视频显示失败
// Parameter:    HWND hHandle    要显示视频的控件句柄
// Parameter:    char * ErrInfo   错误说明信息
//************************************
PLATERECOGNIZE_API BOOL CALLING_CONVENTION Plate_InitVideo(HWND  hHandle, char *  ErrInfo);

//************************************
// Method:        Plate_CloseVideo
// Describe:    关闭车牌识别设备到指定控件上的视频显示
// FullName:      Plate_CloseVideo
// Access:          public 
// Returns:        BOOL
// Returns Describe:    TRUE为视频关闭成功，FALSE为视频关闭失败
// Parameter:    HWND hHandle   要关闭视频的控件的句柄。若为0，表示关闭所有控件上的视频
//************************************
PLATERECOGNIZE_API BOOL CALLING_CONVENTION  Plate_CloseVideo(HWND  hHandle);

//************************************
// Method:        Plate_Screenshot
// Describe:        抓拍一张完整的大照片（压缩到150kb左右），并存到指定的路径（不需要车牌识别，不需要上报），若此时有字符叠加，则叠加上字符
// FullName:      Plate_Screenshot
// Access:          public 
// Returns:        BOOL
// Returns Describe:    TRUE为获取图片成功，FALSE为获取图片失败
// Parameter:    char * ImageInfo    图片的路径和文件名（JPG类型）
//************************************
PLATERECOGNIZE_API BOOL CALLING_CONVENTION  Plate_Screenshot(char * ImageInfo);

//************************************
// Method:        Plate_Timing
// Describe:        根据参数传入校时，同时反馈校时后的时间
// FullName:      Plate_Timing
// Access:          public 
// Returns:        BOOL
// Returns Describe:                                 TRUE为显示成功，FALSE为显示失败；
// Parameter:    char * StandardTime        传入标准时,格式：yyyy-MM-dd HH:mm:ss
// Parameter:    char * ReturnTime            校时后设备设备时,格式：yyyy-MM-dd HH:mm:ss
//************************************
PLATERECOGNIZE_API BOOL CALLING_CONVENTION  Plate_Timing(char* StandardTime, char * ReturnTime);

//************************************
// Method:        Plate_ShowTime
// Describe:        在视频右下角显示时间
// FullName:      Plate_ShowTime
// Access:          public 
// Returns:        BOOL
// Returns Describe:        TRUE为显示成功，FALSE为显示失败
//************************************
PLATERECOGNIZE_API BOOL CALLING_CONVENTION  Plate_ShowTime();

//************************************
// Method:        Plate_CloseTime
// Describe:        在视频右下角清除时间
// FullName:      Plate_CloseTime
// Access:          public 
// Returns:        BOOL
// Returns Describe:        TRUE为清除成功，FALSE为清除失败
//************************************
PLATERECOGNIZE_API BOOL CALLING_CONVENTION  Plate_CloseTime();

//************************************
// Method:        Plate_GetDeviceInfo
// Describe:        获取车牌识别基础信息说明 ：获取车牌识别的基本信息
// FullName:      Plate_GetDeviceInfo
// Access:          public 
// Returns:        BOOL
// Returns Describe:    TRUE为获取成功，FALSE为获取失败
// Parameter:    char * PlateInfo
//                                    混合参数格式： Brand | Model | FirmWareVer | DLLVer
//                                    Brand车牌识别品牌
//                                    1：信路威   2：海康威视  3：金三立 4：……
//                                    Model：车牌识别型号
//                                    FirmWareVer：车牌识别固件版本
//                                    DLLVer：车牌识别动态库型号
//                                    例如：2 | HKWS - 1 - ds | 2.0.1 | 3.2.1.1
//                                    表示为海康威视的型号为HKWS - 1 - ds的车牌识别，其固件版本号为2.0.1，动态库版本号为3.2.1.1
//************************************
PLATERECOGNIZE_API BOOL CALLING_CONVENTION  Plate_GetDeviceInfo(char * PlateInfo);