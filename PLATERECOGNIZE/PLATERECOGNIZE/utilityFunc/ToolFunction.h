#include "tinyxml/tinyxml.h"
#include "tinyxml/tinystr.h"
#include <string>

//#define MY_SPRINTF sprintf_s

typedef struct _ImgDataStruct{
    UCHAR* srcImgData;
    long srcImgDataLengh;
    _ImgDataStruct() :
        srcImgData(NULL),
        srcImgDataLengh(0)
    {
    }

    ~_ImgDataStruct()
    {
        //if (srcImgData)
        //{
        //    delete[] srcImgData;
        //    srcImgData = NULL;
        //}
        srcImgDataLengh = 0;
    }
}ImgDataStruct;

typedef struct _OverlayInfo
{
    const WCHAR* szOverlayString;
    int itextLength;
    int iFontSize;
    int iColorR;
    int iColorG;
    int iColorB;

    _OverlayInfo() :
        szOverlayString(NULL),
        itextLength(0),
        iFontSize(12),
        iColorR(0),
        iColorG(0),
        iColorB(0)
    {
    }
    ~_OverlayInfo()
    {
        szOverlayString = NULL;
    }

}OverlayInfo;


//ͨ���ڵ������Ҳ�������Ӧ�ڵ�
//ע��XMLTYPE Ϊ1ʱ��InputInfoΪXML·������Ϊ2ʱ,InputInfoΪ�������ļ�����
TiXmlElement SelectElementByName(const char* InputInfo, char* pName, int iXMLType);

TiXmlElement* ReadElememt(TiXmlElement* InputElement, char* pName);

void Tool_ReadKeyValueFromConfigFile(const char* FileName, const char* nodeName, const char* keyName, char* keyValue, int bufferSize);

void g_WriteKeyValueFromConfigFile(const char* FileName, const char* nodeName, const char* keyName, char* keyValue, int bufferSize);

//���IP����Ч��
int g_checkIP(const char* p);

bool Tool_IsFileExist(const char* FilePath);

bool Tool_MakeDir(const char* chImgPath);

long Tool_GetFileSize(const char *FileName);

bool PingIPaddress(const char* IpAddress);

bool Tool_Img_ScaleJpg(PBYTE pbSrc, int iSrcLen, PBYTE pbDst, DWORD *iDstLen, int iDstWidth, int iDstHeight, int compressQuality);

int Tool_GetEncoderClsid(const WCHAR* format, CLSID* pClsid);

void Tool_ExcuteShellCMD(char* pChCommand);

bool Tool_OverlayStringToImg(unsigned char** pImgsrc, long srcSize,
    unsigned char** pImgDest, long& DestSize,
    const wchar_t* DestString, int FontSize,
    int x, int y, int colorR, int colorG, int colorB,
    int compressQuality);

bool GetDataFromAppenedInfo(char *pszAppendInfo, std::string strItemName, char *pszRstBuf, int *piRstBufLen);

void ExcuteCMD(char* pChCommand);

std::wstring Img_string2wstring(std::string strSrc);

#ifdef USE_MFC
//ɾ��ָ���ļ���
bool DeleteDirectory(char* strDirName);

//ɾ��ָ��Ŀ¼���� 'yyyy-mm-dd' ��ʽ�������ļ��У������޶�������Ϊʱ������ָ������
int CirclelaryDelete(char* folderPath, int iBackUpDays);

#endif // USE_MFC


bool Tool_SaveFileToDisk(char* chImgPath, void* pImgData, DWORD dwImgSize);

#ifdef WIN32
std::string GetSoftVersion(const char* exepath);
#endif

int DrawStringToImg(const ImgDataStruct dataStruct, const OverlayInfo overlayInfo, UCHAR* destImgBuffer, long& destBufferSize);

//************************************
// Method:        Tool_RgbBin2ByteBin
// Describe:       ��PCC200�豸����Ķ�ֵͼ���ݷ�ת
// FullName:      Tool_RgbBin2ByteBin
// Access:          public 
// Returns:        void
// Returns Describe:
// Parameter:    PBYTE m_rgbBinImage ԭ��ֵͼ����
// Parameter:    PBYTE pbByteBinImage :��ת��Ķ�ֵͼ����
//************************************
void Tool_RgbBin2ByteBin(PBYTE m_rgbBinImage, PBYTE pbByteBinImage);

//************************************
// Method:        BinImage2BitmapData
// Describe:        ��PCC200�豸����Ķ�ֵͼ����ת���� bmp ����
// FullName:      BinImage2BitmapData
// Access:          public 
// Returns:        BOOL
// Returns Describe:
// Parameter:    int iCX        :��ֵͼ���� ,�豸����Ķ�ֵͼ����Ϊ 112
// Parameter:    int iCY        :��ֵͼ�߶ȣ��豸����Ķ�ֵͼ�߶�Ϊ 20
// Parameter:    BYTE * pbByteBinImage :��ֵͼ����
// Parameter:    char * pbBitmapData      :BMPͼƬ���ݻ���
// Parameter:    int * piBitmapDataLen    :Bmp�����С
//************************************
BOOL Tool_BinImage2BitmapData(int iCX, int iCY, BYTE* pbByteBinImage, char* pbBitmapData, int* piBitmapDataLen);


//************************************
// Method:        Tool_Bin2BMP
// Describe:        ��PCC200�豸����Ķ�ֵͼ����ת���� bmp ����, �ú����Ƕ� Tool_BinImage2BitmapData ����һ���װ
// FullName:      Tool_Bin2BMP
// Access:          public 
// Returns:        void
// Returns Describe:
// Parameter:    PBYTE pbBinData        :��ֵͼ����
// Parameter:    PBYTE pbBmpData        :BMPͼƬ���ݻ���
// Parameter:    INT & nBmpLen          :Bmp�����С
//************************************
void Tool_Bin2BMP(PBYTE pbBinData, PBYTE pbBmpData, INT& nBmpLen);