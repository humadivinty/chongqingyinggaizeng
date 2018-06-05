// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� PLATERECOGNIZE_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// PLATERECOGNIZE_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�

/*
�汾��ʶ
�汾��	�޸�ʱ��	�޶�����	�޶���
Ver1.0.5	20180408	���ӳ���ʶ��ʧ�ܺ���	��Զ����
Ver1.0.6	20180409	����Ҫ��֧�ֵĲ���ϵͳ	��Զ����
Ver1.0.7	20180423	�޸Ĺر�ʱ���ַ����ӷ�������
�����ַ�����λ�õĲ���˵��	��Զ����
Ver1.0.8	20180427	����·������Ҫ�󣬴���޸�	��Զ����
Ver1.0.10	20180516	�ӿ�3���ӿ�13��������ʶ��ӿں�����	��Զ����

1	����ʶ��Ҫ��
1��	����ʶ����Ӧ�������ƺͳ�����ɫ
2��	���Ʊ�����Ͻ�ͨ����������Ҫ��Ϊ���֡����֣���ĸ��ɣ����к����ַ�������GB2312�淶�����ó�Խ���ַ���
3��	���Ʋ��ð������룬���ð��������ַ���������@#|��$%����&*������������������=-���Ⱦ�����Ч�ַ���
4��	������ɫӦ֧������Դ����������ɫ
5��	����ʶ����Ӧ�ٶ�Ҫ�������200ms���ڡ�
˵����200msָ���Ǵ�����Ȧ������ʶ������շ�ϵͳʶ�����ݵ����ڣ�200ms������������ʶ���ʱ�����ܡ�


2	��̬���ӿ�˵��
1��	��̬������PLATERECOGNIZE.DLL��
2��	�ö�̬����þ�̬���÷�ʽ������Լ��Ϊ_stdcall���÷�ʽ��
3��	��̬�������������������ߵĵ���Ҫ�󣬰���Visual Basic 6.0��Visual C++ 6.0��C#,JAVA�ȡ�
4��	���豸�����ṩ���Ͻӿں��������PLATERECOGNIZE.DLL�ļ���
5��	�豸����Ӧ���ṩ����PLATERECOGNIZE.dll��Demo���
6��
7��	PLATERECOGNIZE.dll����ʹ�������ļ�������Ҫ��������Ϣ��ֻ��ͨ����ʼ����������
8��	PLATERECOGNIZE.dllҪ�󸲸Ǳ��豸���ҵ������豸�ͺţ�ͬһ�����Ҳ��ó��ֶ��Ӳ���汾��̬���ļ�,�߰汾��̬�������¼��ݵͰ汾Ӳ��
9��	����̬���ӿ�Ҫ��֧��Windows x86��x64����ϵͳ���Գ�����XP��Win7,Win10������ϵͳ֧��
*/

/*
һ����Ϣ����˵��
Ӳ��������ͨ����Ϣ֪ͨ���շ�������շ����ȥ��ȡ��ص���Ϣ

1��	��Ϣ����
��Ϣ	Msg			    �Զ���
2��	��ϢID����
��ϢID��WPARAM��������(LPARAM=0����)��ͨ����ͬ��ID���ֲ�ͬ���¼�����ϢID�������£�
WPARAM                LPARAM
Ӳ����ʶ����ϱ��¼�	      1				       0
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
// Returns  Describe:                   TRUEΪ��ʼ���ɹ���FALSEΪ��ʼ��ʧ��
// Parameter:    HWND hHandle  ������Ϣ�Ĵ��ڵľ��
// Parameter:    int Msg             ʶ��������Ϣ��
// Parameter:    int PorType       �˿�����
//                                              1������
//                                              2������
// Parameter:    char * PortName  Ҫ��ʼ���Ĵ��ں�
//                                              ���Ǵ��ڣ���Ϊ���ںš�Com1��, ��Com2����
//                                              �������ڣ���ΪIP��ַ���硰192.168.1.11��
// Parameter:    long Baudrate   ���ڲ����ʣ�������ʱ����0
// Parameter:    char * Px          ������Ƶ�ֱ��ʣ�Ĭ�ϸ�ʽΪ704*576������*���ַ��ָ�
//************************************
PLATERECOGNIZE_API BOOL CALLING_CONVENTION Plate_InitDevice(HWND hHandle, int Msg, int PorType, char *  PortName, long Baudrate, char * Px);


//************************************
// Method:        Plate_CloseDevice
// Describe:        �ر��Ѿ��򿪵ĳ���ʶ������
// FullName:      Plate_CloseDevice
// Access:          public 
// Returns:        BOOL
// Returns  Describe:    TRUEΪ�رճɹ���FALSEΪ�ر�ʧ��
//************************************
PLATERECOGNIZE_API BOOL CALLING_CONVENTION Plate_CloseDevice();

//************************************
// Method:        Plate_GetInfo
// Describe:        ��ȡ����ʶ����
// FullName:      Plate_GetInfo
// Access:          public 
// Returns:        BOOL
// Returns  Describe:  TRUEΪʶ��ɹ���FALSEΪʶ��ʧ��
// Parameter:    char * PlateInfor  ����ʶ����
// Parameter:    int * ColorInfo   ������ɫʶ����
//                                               ������ɫΪ��ͨ�����궨�壺
//                                                1λ����
//                                                0 - ��ɫ��1 - ��ɫ��
//                                                2 - ��ɫ��3 - ��ɫ��
//                                                4 - ������ɫ��5 - ����˫ƴɫ
//                                                6 - ���׽���ɫ
//                                                9 - ��ȷ��
// Parameter:    char * PicId         ��λ���������кţ���Ϊ���γ���ʶ������ͼƬһһ��Ӧ��ϵ������λ����Plate_GetImageȡ����ͼƬʱ�ٴδ������
//************************************
PLATERECOGNIZE_API BOOL CALLING_CONVENTION Plate_GetInfo(char * PlateInfor, int * ColorInfo, char * PicId);

//************************************
// Method:        Plate_GetImage
// Describe:        ��ȡ����ʶ���ֵ��ͼƬ��Ϣ������ץ��ͼƬ��Ϣ��һ������£�����Plate_GetInfo�󣬵��øú�����ȡ����ʶ������Ӧ��ͼƬ��Ϣ��
//                      ������Ϊ�������µ�ͼƬ��Ϣ��ͼƬ��������棬ֻ���µ�ץ�Ĵ������ܽ�����ͼƬ���.
// FullName:      Plate_GetImage
// Access:          public 
// Returns:        BOOL
// Returns Describe: TRUEΪ��ȡͼƬ�ɹ���FALSEΪ��ȡͼƬʧ��
// Parameter:    int ImageType   
//                                              0����ֵ������ͼƬ
//                                              1�����Ʋ�ɫСͼ
//                                              2����ͷ��ɫͼƬ
//                                              3: �����Ĵ���Ƭ��ѹ����150kb���ң�
//
// Parameter:    char * ImageInfo    
//                                              ��ֵ��ͼƬ��·�����ļ�����JPG���ͣ�
//
// Parameter:    char * PicId       
//                                              λ���������кţ���Ϊ���γ���ʶ������ͼƬһһ��Ӧ��ϵ����̬�ⷵ�ص�ͼƬ����Plate_Getinfo�γɶ�Ӧ��ϵ��
//                                              ���Ҳ�����PicIdʱ������ʶ��������ץ��һ��ͼƬ�������ַ�����
//
// Parameter:    char * Overlay   
//                                              �ַ�������Ϣ����Overlay����һ�����ܵ��ӳ���ʱ�����ߴ��лس����з�ʱ�����л��д���
//                                              �ַ�����Ϊ�̶��Ķ�����������Ƭ�������棬��������ʵ��Ƭ���򣬵�����ͬһ������ε��ñ�����ʱ������ղ������ϴε��ַ���������
//                                              OverlayΪ���ַ���ʱ��Ϊ����ַ���������
//************************************
PLATERECOGNIZE_API BOOL CALLING_CONVENTION Plate_GetImage(int ImageType, char * ImageInfo, char * PicId, char * Overlay);

//************************************
// Method:        Plate_GetState
// Describe:        ��ȡ����ʶ����״̬
// FullName:      Plate_GetState
// Access:          public 
// Returns:        BOOL
// Returns Describe:    TRUEΪ��ȡ״̬�ɹ���FALSEΪ��ȡ״̬ʧ��
// Parameter:    int * State    0������  ; 1������
//************************************
PLATERECOGNIZE_API BOOL CALLING_CONVENTION Plate_GetState(int* State);

//************************************
// Method:        Plate_Capture
// Describe:        �������ץ��ʶ�𣬵��øú�������ץ��ʶ��һ��
// FullName:      Plate_Capture
// Access:          public 
// Returns:        BOOL
// Returns Describe:        TRUEΪץ�ĳɹ�������ȴ��յ���̬���ϱ��¼�ʶ������Ϣ�žͿ���ȡʶ�����ˣ�FALSEΪץ��ʧ��
// Notice:   ע�⣬����ʶ��Ԫ2����ֻ����Ӧһ�δ���ʶ�����ģ��ץ��ʶ����˲�Ҫ������ʶ����̫����
//************************************
PLATERECOGNIZE_API BOOL CALLING_CONVENTION Plate_Capture();

//************************************
// Method:        Plate_InitVideo
// Describe:        ������ʶ���豸��Ƶ��ʾ��ָ���ؼ��ϡ����ظ����øýӿڣ���hHandle����ͬʱ����ʾ�ڶ���ؼ�����ʾ������ʶ���豸�Ͽ�����֮��֮ǰ����������Ȼ��Ч
// FullName:      Plate_InitVideo
// Access:          public 
// Returns:        BOOL
// Returns Describe:    TRUEΪ��Ƶ��ʾ�ɹ���FALSEΪ��Ƶ��ʾʧ��
// Parameter:    HWND hHandle    Ҫ��ʾ��Ƶ�Ŀؼ����
// Parameter:    char * ErrInfo   ����˵����Ϣ
//************************************
PLATERECOGNIZE_API BOOL CALLING_CONVENTION Plate_InitVideo(HWND  hHandle, char *  ErrInfo);

//************************************
// Method:        Plate_CloseVideo
// Describe:    �رճ���ʶ���豸��ָ���ؼ��ϵ���Ƶ��ʾ
// FullName:      Plate_CloseVideo
// Access:          public 
// Returns:        BOOL
// Returns Describe:    TRUEΪ��Ƶ�رճɹ���FALSEΪ��Ƶ�ر�ʧ��
// Parameter:    HWND hHandle   Ҫ�ر���Ƶ�Ŀؼ��ľ������Ϊ0����ʾ�ر����пؼ��ϵ���Ƶ
//************************************
PLATERECOGNIZE_API BOOL CALLING_CONVENTION  Plate_CloseVideo(HWND  hHandle);

//************************************
// Method:        Plate_Screenshot
// Describe:        ץ��һ�������Ĵ���Ƭ��ѹ����150kb���ң������浽ָ����·��������Ҫ����ʶ�𣬲���Ҫ�ϱ���������ʱ���ַ����ӣ���������ַ�
// FullName:      Plate_Screenshot
// Access:          public 
// Returns:        BOOL
// Returns Describe:    TRUEΪ��ȡͼƬ�ɹ���FALSEΪ��ȡͼƬʧ��
// Parameter:    char * ImageInfo    ͼƬ��·�����ļ�����JPG���ͣ�
//************************************
PLATERECOGNIZE_API BOOL CALLING_CONVENTION  Plate_Screenshot(char * ImageInfo);

//************************************
// Method:        Plate_Timing
// Describe:        ���ݲ�������Уʱ��ͬʱ����Уʱ���ʱ��
// FullName:      Plate_Timing
// Access:          public 
// Returns:        BOOL
// Returns Describe:                                 TRUEΪ��ʾ�ɹ���FALSEΪ��ʾʧ�ܣ�
// Parameter:    char * StandardTime        �����׼ʱ,��ʽ��yyyy-MM-dd HH:mm:ss
// Parameter:    char * ReturnTime            Уʱ���豸�豸ʱ,��ʽ��yyyy-MM-dd HH:mm:ss
//************************************
PLATERECOGNIZE_API BOOL CALLING_CONVENTION  Plate_Timing(char* StandardTime, char * ReturnTime);

//************************************
// Method:        Plate_ShowTime
// Describe:        ����Ƶ���½���ʾʱ��
// FullName:      Plate_ShowTime
// Access:          public 
// Returns:        BOOL
// Returns Describe:        TRUEΪ��ʾ�ɹ���FALSEΪ��ʾʧ��
//************************************
PLATERECOGNIZE_API BOOL CALLING_CONVENTION  Plate_ShowTime();

//************************************
// Method:        Plate_CloseTime
// Describe:        ����Ƶ���½����ʱ��
// FullName:      Plate_CloseTime
// Access:          public 
// Returns:        BOOL
// Returns Describe:        TRUEΪ����ɹ���FALSEΪ���ʧ��
//************************************
PLATERECOGNIZE_API BOOL CALLING_CONVENTION  Plate_CloseTime();

//************************************
// Method:        Plate_GetDeviceInfo
// Describe:        ��ȡ����ʶ�������Ϣ˵�� ����ȡ����ʶ��Ļ�����Ϣ
// FullName:      Plate_GetDeviceInfo
// Access:          public 
// Returns:        BOOL
// Returns Describe:    TRUEΪ��ȡ�ɹ���FALSEΪ��ȡʧ��
// Parameter:    char * PlateInfo
//                                    ��ϲ�����ʽ�� Brand | Model | FirmWareVer | DLLVer
//                                    Brand����ʶ��Ʒ��
//                                    1����·��   2����������  3�������� 4������
//                                    Model������ʶ���ͺ�
//                                    FirmWareVer������ʶ��̼��汾
//                                    DLLVer������ʶ��̬���ͺ�
//                                    ���磺2 | HKWS - 1 - ds | 2.0.1 | 3.2.1.1
//                                    ��ʾΪ�������ӵ��ͺ�ΪHKWS - 1 - ds�ĳ���ʶ����̼��汾��Ϊ2.0.1����̬��汾��Ϊ3.2.1.1
//************************************
PLATERECOGNIZE_API BOOL CALLING_CONVENTION  Plate_GetDeviceInfo(char * PlateInfo);