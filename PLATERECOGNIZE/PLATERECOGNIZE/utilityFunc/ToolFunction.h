#include "tinyxml/tinyxml.h"
#include "tinyxml/tinystr.h"
#include <string>
#include <list>

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

typedef struct tag_COLOR_INFO
{
    int iColorAlpha;        //透明度  0-255 其中0为全透明
    int iColorR;               //红色分量 0-255
    int iColorG;               //绿色分量 0-255
    int iColorB;               //蓝色分量 0-255

    tag_COLOR_INFO() :
        iColorAlpha(255),
        iColorR(255),
        iColorG(255),
        iColorB(255)
    {  }
}COLOR_INFO;

typedef struct tag_PositionInfo
{
    int iPosX;          //字符叠加的左顶点 x 坐标， 范围是大于等于0，
    int iPosY;          //字符叠加的左顶点 y 坐标 ， 当值为-1时， 在图片的上方之外的区域叠加 ; 值为-2时， 在图片下方之外的区域叠加， 其他负数的值按-2处理, 默认为-2                            

    tag_PositionInfo() :
        iPosX(0),
        iPosY(-2)
    {};

}PositionInfo;

typedef struct _OverlayInfo
{
    const WCHAR* szOverlayString;     //需要叠加的字符串
    const WCHAR* szFontFamily;
    int itextLength;                    //字符串长度
    int iFontSize;                        //叠加字符的大小
    int iStyle;                             //叠加风格: 1时,只在字符背后叠加；2时整行叠加; 默认为2
    COLOR_INFO st_fontColor;    //字体颜色
    COLOR_INFO st_backgroundColor;  //背景颜色
    PositionInfo st_FontPosition;    //叠加字符的位置

    _OverlayInfo() :
        szOverlayString(NULL),
        szFontFamily(L"Times New Roman"),
        itextLength(0),
        iFontSize(32),
        iStyle(2)
    {
    }
    ~_OverlayInfo()
    {
        szOverlayString = NULL;
    }

}OverlayInfo;

typedef struct tag_RectF
{
    float X;
    float Y;
    float Width;
    float Height;

    tag_RectF() :
        X(0.0),
        Y(0.0),
        Width(0.0),
        Height(0.0)
    {}
}MyRectf;

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

bool Tool_Img_ScaleJpg(PBYTE pbSrc, int iSrcLen, PBYTE pbDst, size_t *iDstLen, int iDstWidth, int iDstHeight, int compressQuality);

bool Tool_Img_compress(PBYTE pbSrc, int iSrcLen, PBYTE pbDst, size_t *iDstLen, int iDstWidth, int iDstHeight, int wishSize);

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

int DrawStringToImg(const ImgDataStruct dataStruct, const OverlayInfo overlayInfo, void* destImgBuffer, size_t& destBufferSize);

int DrawStringToImgEx(const ImgDataStruct dataStruct, const OverlayInfo* overlayInfo, int overlayCount, void* destImgBuffer, size_t& destBufferSize);

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


int DrawHeadStyleString(void* srcImgData, size_t srcLength, void* destImgData, size_t& destLength, const char* overlayString, int posX, int posY);


//************************************
// Method:        DrawEnd1String
// Describe:        该函数为定制函数，在图片底部叠加白色文字，背景填充颜色为灰色
// FullName:      DrawEnd1String
// Access:          public 
// Returns:        int
// Returns Describe:
// Parameter:    void * srcImgData
// Parameter:    size_t srcLength
// Parameter:    void * destImgData
// Parameter:    size_t & destLength
// Parameter:    char * overlayString
// Parameter:    int posX
// Parameter:    int posY
//************************************
int DrawEnd1String(void* srcImgData, size_t srcLength, void* destImgData, size_t& destLength, const char* overlayString, int posX, int posY);

//************************************
// Method:        DrawEnd2String
// Describe:        该函数为定制函数，在图片底部叠加黄色文字，背景填充颜色为蓝色
// FullName:      DrawEnd2String
// Access:          public 
// Returns:        int
// Returns Describe:
// Parameter:    void * srcImgData
// Parameter:    size_t srcLength
// Parameter:    void * destImgData
// Parameter:    size_t & destLength
// Parameter:    char * overlayString
// Parameter:    int posX
// Parameter:    int posY
//************************************
int DrawEnd2String(void* srcImgData, size_t srcLength, void* destImgData, size_t& destLength, const char* overlayString, int posX, int posY);

bool Tool_CalculateStringWithAndHeight(const char* overlayString, const int imageWidth, const int imageHeight,
    const int fontSize, const WCHAR* szFontFamily,
    MyRectf& rectfOut);