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


//通过节点名查找并返回相应节点
//注：XMLTYPE 为1时，InputInfo为XML路径，当为2时,InputInfo为二进制文件内容
TiXmlElement SelectElementByName(const char* InputInfo, char* pName, int iXMLType);

TiXmlElement* ReadElememt(TiXmlElement* InputElement, char* pName);

void Tool_ReadKeyValueFromConfigFile(const char* FileName, const char* nodeName, const char* keyName, char* keyValue, int bufferSize);

void g_WriteKeyValueFromConfigFile(const char* FileName, const char* nodeName, const char* keyName, char* keyValue, int bufferSize);

//检查IP的有效性
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
//删除指定文件夹
bool DeleteDirectory(char* strDirName);

//删除指定目录中以 'yyyy-mm-dd' 方式命名的文件夹，其中限定的条件为时间早于指定天数
int CirclelaryDelete(char* folderPath, int iBackUpDays);

#endif // USE_MFC


bool Tool_SaveFileToDisk(char* chImgPath, void* pImgData, DWORD dwImgSize);

#ifdef WIN32
std::string GetSoftVersion(const char* exepath);
#endif

int DrawStringToImg(const ImgDataStruct dataStruct, const OverlayInfo overlayInfo, UCHAR* destImgBuffer, long& destBufferSize);

//************************************
// Method:        Tool_RgbBin2ByteBin
// Describe:       将PCC200设备输出的二值图数据翻转
// FullName:      Tool_RgbBin2ByteBin
// Access:          public 
// Returns:        void
// Returns Describe:
// Parameter:    PBYTE m_rgbBinImage 原二值图数据
// Parameter:    PBYTE pbByteBinImage :翻转后的二值图数据
//************************************
void Tool_RgbBin2ByteBin(PBYTE m_rgbBinImage, PBYTE pbByteBinImage);

//************************************
// Method:        BinImage2BitmapData
// Describe:        将PCC200设备输出的二值图数据转换成 bmp 数据
// FullName:      BinImage2BitmapData
// Access:          public 
// Returns:        BOOL
// Returns Describe:
// Parameter:    int iCX        :二值图宽度 ,设备输出的二值图宽度为 112
// Parameter:    int iCY        :二值图高度，设备输出的二值图高度为 20
// Parameter:    BYTE * pbByteBinImage :二值图数据
// Parameter:    char * pbBitmapData      :BMP图片数据缓冲
// Parameter:    int * piBitmapDataLen    :Bmp缓冲大小
//************************************
BOOL Tool_BinImage2BitmapData(int iCX, int iCY, BYTE* pbByteBinImage, char* pbBitmapData, int* piBitmapDataLen);


//************************************
// Method:        Tool_Bin2BMP
// Describe:        将PCC200设备输出的二值图数据转换成 bmp 数据, 该函数是对 Tool_BinImage2BitmapData 的又一层封装
// FullName:      Tool_Bin2BMP
// Access:          public 
// Returns:        void
// Returns Describe:
// Parameter:    PBYTE pbBinData        :二值图数据
// Parameter:    PBYTE pbBmpData        :BMP图片数据缓冲
// Parameter:    INT & nBmpLen          :Bmp缓冲大小
//************************************
void Tool_Bin2BMP(PBYTE pbBinData, PBYTE pbBmpData, INT& nBmpLen);