#ifndef CAMERRESULT
#define CAMERRESULT

/* ʶ������ͼ���Ͷ��� */
#define RECORD_BIGIMG_BEST_SNAPSHOT			0x0001	/**< ������ʶ��ͼ */
#define RECORD_BIGIMG_LAST_SNAPSHOT			0x0002	/**< ���ʶ��ͼ */
#define RECORD_BIGIMG_BEGIN_CAPTURE			0x0003	/**< ��ʼץ��ͼ */
#define RECORD_BIGIMG_BEST_CAPTURE			0x0004	/**< ������ץ��ͼ */
#define RECORD_BIGIMG_LAST_CAPTURE			0x0005	/**<  ���ץ��ͼ */

#define  MAX_IMG_SIZE (20*1024*1024)
#define COMPRESS_IMG_SIZE (150 * 1024)

#define COMPRESS_300W_WIDTH 1600
#define COMPRESS_300W_HEIGHT (1200+80)

#define COMPRESS_200W_WIDTH 1600
#define COMPRESS_200W_HEIGHT (1080+80)


class CameraIMG
{
public:
	CameraIMG();
	CameraIMG(const CameraIMG& CaIMG);
	~CameraIMG();
    char chSavePath[256];
	unsigned char* pbImgData;
    unsigned long wImgWidth;
    unsigned long wImgHeight;
    unsigned long dwImgSize;
    unsigned long  wImgType;

	CameraIMG& operator = (const CameraIMG& CaIMG);
};

class CameraResult
{
public:

	CameraResult();
	CameraResult(const CameraResult& CaRESULT);
	~CameraResult();
public:
    char pcAppendInfo[2048];
    char chSignStationName[256];
    char chSignStationDirection[256];
    char chListNo[256];
    char chPlateTime[256];
    char chDeviceIp[64];
    char chPlateNO[64];
    char chPlateColor[64];
    char chSignStationID[64];
    char chDeviceID[64];
    char chLaneID[64];

    DWORD64 dw64TimeMS;
    unsigned long dwCarID;

    float fVehLenth;			//����
    float fDistanceBetweenAxles;		//���
    float fVehHeight;		//����

	int iDeviceID;
	int iPlateColor;
	int iPlateTypeNo;	
	int iSpeed;
	int iResultNo;
	int iVehTypeNo;		//���ʹ���: ��1--1 ��������4--4�� ��1--5  ��������4--8
	int iVehBodyColorNo;	
	int iVehBodyDeepNo;	
	int iAreaNo;
	int iRoadNo;
	int iLaneNo;
	int iDirection;
	int iWheelCount;		//����
	int iAxletreeCount;		//����
	int iAxletreeGroupCount;//������
	int iAxletreeType;		//����
    int iReliability;       //���Ŷ�
	bool bBackUpVeh;		//�Ƿ񵹳�

	CameraIMG CIMG_BestSnapshot;	/**< ������ʶ��ͼ */
	CameraIMG CIMG_LastSnapshot;	/**< ���ʶ��ͼ */
	CameraIMG CIMG_BeginCapture;	/**< ��ʼץ��ͼ */
	CameraIMG CIMG_BestCapture;		/**< ������ץ��ͼ */
	CameraIMG CIMG_LastCapture;		/**< ���ץ��ͼ */
	CameraIMG CIMG_PlateImage;		/**< ����Сͼ */
	CameraIMG CIMG_BinImage;			/**< ���ƶ�ֵͼ */

public:
	CameraResult& operator = (const CameraResult& CaRESULT);

	friend bool SerializationResultToDisk(const char* chFilePath, const CameraResult& CaResult);
	friend bool SerializationFromDisk(const char* chFilePath, CameraResult& CaResult);

	friend bool SerializationAsConfigToDisk(const char* chFilePath, const CameraResult& CaResult);
	friend bool SerializationAsConfigFromDisk(const char* chFilePath, CameraResult& CaResult);
};

typedef struct _tagSafeModeInfo
{	
	char chBeginTime[256];
	char chEndTime[256];
    int iEableSafeMode;
	int index;
	int DataInfo;
	_tagSafeModeInfo()
	{		
		memset(chBeginTime, 0, sizeof(chBeginTime));
		memset(chEndTime, 0, sizeof(chEndTime));
        iEableSafeMode = 0;
		index = 0;
		DataInfo = 0;
	}
}_tagSafeModeInfo;

#endif