#ifndef CAMERRESULT
#define CAMERRESULT

/* 识别结果大图类型定义 */
#define RECORD_BIGIMG_BEST_SNAPSHOT			0x0001	/**< 最清晰识别图 */
#define RECORD_BIGIMG_LAST_SNAPSHOT			0x0002	/**< 最后识别图 */
#define RECORD_BIGIMG_BEGIN_CAPTURE			0x0003	/**< 开始抓拍图 */
#define RECORD_BIGIMG_BEST_CAPTURE			0x0004	/**< 最清晰抓拍图 */
#define RECORD_BIGIMG_LAST_CAPTURE			0x0005	/**<  最后抓拍图 */

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

    float fVehLenth;			//车长
    float fDistanceBetweenAxles;		//轴距
    float fVehHeight;		//车高

	int iDeviceID;
	int iPlateColor;
	int iPlateTypeNo;	
	int iSpeed;
	int iResultNo;
	int iVehTypeNo;		//车型代码: 客1--1 。。。客4--4， 货1--5  。。。货4--8
	int iVehBodyColorNo;	
	int iVehBodyDeepNo;	
	int iAreaNo;
	int iRoadNo;
	int iLaneNo;
	int iDirection;
	int iWheelCount;		//轮数
	int iAxletreeCount;		//轴数
	int iAxletreeGroupCount;//轴组数
	int iAxletreeType;		//轴型
    int iReliability;       //可信度
	bool bBackUpVeh;		//是否倒车

	CameraIMG CIMG_BestSnapshot;	/**< 最清晰识别图 */
	CameraIMG CIMG_LastSnapshot;	/**< 最后识别图 */
	CameraIMG CIMG_BeginCapture;	/**< 开始抓拍图 */
	CameraIMG CIMG_BestCapture;		/**< 最清晰抓拍图 */
	CameraIMG CIMG_LastCapture;		/**< 最后抓拍图 */
	CameraIMG CIMG_PlateImage;		/**< 车牌小图 */
	CameraIMG CIMG_BinImage;			/**< 车牌二值图 */

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