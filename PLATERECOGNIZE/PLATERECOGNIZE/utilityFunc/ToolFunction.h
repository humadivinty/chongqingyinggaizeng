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
    int iColorAlpha;        //͸����  0-255 ����0Ϊȫ͸��
    int iColorR;               //��ɫ���� 0-255
    int iColorG;               //��ɫ���� 0-255
    int iColorB;               //��ɫ���� 0-255

    tag_COLOR_INFO() :
        iColorAlpha(255),
        iColorR(255),
        iColorG(255),
        iColorB(255)
    {  }
}COLOR_INFO;

typedef struct tag_PositionInfo
{
    int iPosX;          //�ַ����ӵ��󶥵� x ���꣬ ��Χ�Ǵ��ڵ���0��
    int iPosY;          //�ַ����ӵ��󶥵� y ���� �� ��ֵΪ-1ʱ�� ��ͼƬ���Ϸ�֮���������� ; ֵΪ-2ʱ�� ��ͼƬ�·�֮���������ӣ� ����������ֵ��-2����, Ĭ��Ϊ-2                            

    tag_PositionInfo() :
        iPosX(0),
        iPosY(-2)
    {};

}PositionInfo;

typedef struct _OverlayInfo
{
    const WCHAR* szOverlayString;     //��Ҫ���ӵ��ַ���
    const WCHAR* szFontFamily;
    int itextLength;                    //�ַ�������
    int iFontSize;                        //�����ַ��Ĵ�С
    int iStyle;                             //���ӷ��: 1ʱ,ֻ���ַ�������ӣ�2ʱ���е���; Ĭ��Ϊ2
    COLOR_INFO st_fontColor;    //������ɫ
    COLOR_INFO st_backgroundColor;  //������ɫ
    PositionInfo st_FontPosition;    //�����ַ���λ��

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
//ɾ��ָ���ļ���
bool DeleteDirectory(char* strDirName);

//ɾ��ָ��Ŀ¼���� 'yyyy-mm-dd' ��ʽ�������ļ��У������޶�������Ϊʱ������ָ������
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
// Parameter:    int iCX        :��ֵͼ��� ,�豸����Ķ�ֵͼ���Ϊ 112
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


int DrawHeadStyleString(void* srcImgData, size_t srcLength, void* destImgData, size_t& destLength, const char* overlayString, int posX, int posY);


//************************************
// Method:        DrawEnd1String
// Describe:        �ú���Ϊ���ƺ�������ͼƬ�ײ����Ӱ�ɫ���֣����������ɫΪ��ɫ
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
// Describe:        �ú���Ϊ���ƺ�������ͼƬ�ײ����ӻ�ɫ���֣����������ɫΪ��ɫ
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