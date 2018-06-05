// PLATERECOGNIZE.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "PLATERECOGNIZE.h"
#include "Camera/Camera6467_plate.h"
#include "Camera/CameraResult.h"
#include "utilityFunc/ToolFunction.h"
#include <memory>

extern void g_WriteLog(const char*);
extern void g_ReadKeyValueFromConfigFile(const char* nodeName, const char* keyName, char* keyValue, int bufferSize);

std::shared_ptr<Camera6467_plate> g_pCamera;
std::shared_ptr<uint8_t> g_pImgData;
std::shared_ptr<uint8_t> g_pImgData2;
std::shared_ptr<CameraResult> g_pLastResult;
std::string g_strLastpicID;
std::string g_strOverlayInfo;

bool CheckCamerIsValid()
{
    if (g_pCamera == nullptr)
    {
        g_WriteLog("The camera is not ready, please init device first.");
        return false;
    }
    return true;
}

uint8_t* GetImgBufferAddress()
{
    uint8_t* pData = NULL;
    if (!g_pImgData)
    {
        g_pImgData = std::shared_ptr<uint8_t>(new uint8_t[MAX_IMG_SIZE], std::default_delete<uint8_t[]>());
    }
    if (g_pImgData)
    {
        pData = g_pImgData.get();
        memset(pData, 0, MAX_IMG_SIZE);
    }
    return pData;
}
uint8_t* GetImgBufferAddress2()
{
    uint8_t* pData = NULL;
    if (!g_pImgData2)
    {
        g_pImgData2 = std::shared_ptr<uint8_t>(new uint8_t[MAX_IMG_SIZE], std::default_delete<uint8_t[]>());
    }
    if (g_pImgData2)
    {
        pData = g_pImgData2.get();
        memset(pData, 0, MAX_IMG_SIZE);
    }
    return pData;
}

std::shared_ptr<CameraResult> g_GetResult(const char* PicId)
{
    char szLog[256] = {0};
    std::string strCurrentPicId(PicId);
    if (g_strLastpicID.empty() || 0 != g_strLastpicID.compare(strCurrentPicId) || g_pLastResult == nullptr)
    {
        g_pLastResult = std::shared_ptr<CameraResult>(g_pCamera->GetOneResult());
        g_strLastpicID = strCurrentPicId;

        sprintf_s(szLog, "current PicId= %d is not the same with last PicId = %s, get the new one result from camera.", strCurrentPicId.c_str(), g_strLastpicID.c_str());        
    }
    else
    {
        sprintf_s(szLog, "current PicId= %d is same with last PicId = %s, get the last one result from buffer.", strCurrentPicId.c_str(), g_strLastpicID.c_str());
    }
    g_WriteLog(szLog);
    return g_pLastResult;
}


#define BIN_IMG 0
#define PLATE_IMG 1
#define COLOR_VEHICLE_HEAD_IMG 2
#define FULL_IMG 3

int g_iVideoWidth = 0, g_iVideoHeight = 0;
int g_iTimePosX = 1500, g_iTimePosY = 1000;
char g_szOverlayInfo[512] = {0};
std::wstring g_overlayText;

PLATERECOGNIZE_API BOOL WINAPI Plate_InitDevice(HWND hHandle, int Msg, int PorType, char * PortName, long Baudrate, char * Px)
{
    char chLog[256] = {0};
    sprintf_s(chLog, sizeof(chLog), "Plate_InitDevice, begin, handle = %p, msg = %d, port type = %d, portName = %s, Baudrate= %ld, Px= %p",
        hHandle,
        Msg,
        PorType,
        PortName,
        Baudrate,
        Px);
    g_WriteLog(chLog);

    if (Px && strstr(Px, "*"))
    {
        g_WriteLog("find *");
        sscanf_s(Px, "%d*%d", &g_iVideoWidth, &g_iVideoHeight);

        memset(chLog, 0, sizeof(chLog));
        sprintf_s(chLog, "Get video high-resolution, width and height %d * %d", g_iVideoWidth, g_iVideoHeight);
        g_WriteLog(chLog);
    }
    else
    {
        g_WriteLog("Can`t not get the video high-resolution");
    }

    //char chValue[256] = { 0 };
    //g_ReadKeyValueFromConfigFile("Video", "TimePosX", chValue, sizeof(chValue));
    //int iValue = atoi(chValue);
    //g_iTimePosX = (iValue > 1500) ? iValue : g_iTimePosX;

    //memset(chValue, 0, sizeof(chValue));
    //g_ReadKeyValueFromConfigFile("Video", "TimePosY", chValue, sizeof(chValue));
    //iValue = atoi(chValue);
    //g_iTimePosY = (iValue > 1000) ? iValue : g_iTimePosY;

    BOOL bRet = FALSE;
    if (g_pCamera == nullptr)
    {
        g_WriteLog("create new camera.");
        g_pCamera = std::make_shared<Camera6467_plate>(PortName, hHandle, Msg);
        if (0 == g_pCamera->ConnectToCamera())
        {
            g_WriteLog("connect to camera success.");
        }
        else
        {
            g_WriteLog("connect to camera failed, but will reconnect auto.");
        }
    }
    else
    {
        if (0 != strcmp(g_pCamera->GetCameraIP(), PortName))
        {
            g_WriteLog("The camera is already exist,but the ip address is not the same, create new one.");
            g_pCamera = std::make_shared<Camera6467_plate>(PortName, hHandle, Msg);
        }
        else
        {
            g_WriteLog("The camera is already exist,but the ip address is same with the old one, use it.");
        }
    }

    if (g_pCamera != nullptr)
    {
        bRet = TRUE;
        g_WriteLog("Plate_InitDevice, finish return true.");
    }
    else
    {
        g_WriteLog("Plate_InitDevice, finish return false.");
    }    
    return  (bRet) ? TRUE : FALSE;
}

PLATERECOGNIZE_API BOOL WINAPI Plate_CloseDevice()
{
    g_WriteLog("Plate_CloseDevice, begin.");
    if (g_pCamera)
    {
        g_pCamera->StopPlayVideo();
    }
    g_pCamera = nullptr;
    g_WriteLog("Plate_CloseDevice, finish.");
    return TRUE;
}

PLATERECOGNIZE_API BOOL CALLING_CONVENTION Plate_GetInfo(char * PlateInfor, int * ColorInfo, char * PicId)
{
    char chLog[256] = { 0 };
    sprintf_s(chLog, sizeof(chLog), "Plate_GetInfo, begin, PlateInfor = %p, ColorInfo = %p, PicId = %s.",    PlateInfor,  ColorInfo,   PicId);
    g_WriteLog(chLog);

    bool bRet = FALSE;
    if (CheckCamerIsValid())
    {
        auto Result = g_GetResult(PicId);
        //auto Result = std::shared_ptr<CameraResult>(g_pCamera->GetOneResult());
        if (Result != nullptr)
        {            
            if (!strstr(Result->chPlateNO, "无"))
            {
                memset(PlateInfor, '\0', strlen(Result->chPlateNO)+1);
                memcpy(PlateInfor, Result->chPlateNO, strlen(Result->chPlateNO));
                PlateInfor[strlen(Result->chPlateNO)] = '\0';
                *ColorInfo = Result->iPlateColor;
                bRet = true;
            }
            else
            {
                g_WriteLog("车牌结果为 '无车牌', 返回FFALSE.");
            }
        }
        else
        {
            g_WriteLog("The result is not ready.");
        }
    }

    memset(chLog, 0, sizeof(chLog));
    sprintf_s(chLog, sizeof(chLog), "Plate_GetInfo, end, PlateInfor = %s, ColorInfo = %d, PicId = %s.", PlateInfor, ColorInfo, PicId);
    g_WriteLog(chLog);
    return  (bRet) ? TRUE : FALSE;
}

PLATERECOGNIZE_API BOOL CALLING_CONVENTION Plate_GetImage(int ImageType, char * ImageInfo, char * PicId, char * Overlay)
{
    char chLog[256] = { 0 };
    sprintf_s(chLog, sizeof(chLog), "Plate_GetImage, begin, ImageType = %d, ImageInfo = %s, PicId = %s, Overlay = %s",
        ImageType, \
        ImageInfo, \
        PicId, \
        Overlay);
    g_WriteLog(chLog);

    g_strOverlayInfo = std::string(Overlay);

    bool bRet = false;
    if (CheckCamerIsValid())
    {
        auto Result = g_GetResult(PicId);
        //auto Result = std::shared_ptr<CameraResult>(g_pCamera->GetOneResult());
        void* pData = NULL;
        DWORD iDataLength = 0;
        int iWidth = 0, iHeight = 0;
        if (Result != nullptr)
        {
            switch (ImageType)
            {
            case BIN_IMG:
                pData = Result->CIMG_BinImage.pbImgData;
                iDataLength = Result->CIMG_BinImage.dwImgSize;
                iWidth = Result->CIMG_BinImage.wImgWidth;
                iHeight = Result->CIMG_BinImage.wImgHeight;
                break;
            case PLATE_IMG:
                pData = Result->CIMG_PlateImage.pbImgData;
                iDataLength = Result->CIMG_PlateImage.dwImgSize;
                iWidth = Result->CIMG_PlateImage.wImgWidth;
                iHeight = Result->CIMG_PlateImage.wImgHeight;
                break;
            case COLOR_VEHICLE_HEAD_IMG:
                pData = Result->CIMG_LastSnapshot.pbImgData;
                iDataLength = Result->CIMG_LastSnapshot.dwImgSize;
                iWidth = Result->CIMG_LastSnapshot.wImgWidth;
                iHeight = Result->CIMG_LastSnapshot.wImgHeight;
                break;
            case FULL_IMG:
                pData = Result->CIMG_LastSnapshot.pbImgData;
                iDataLength = Result->CIMG_LastSnapshot.dwImgSize;
                iWidth = Result->CIMG_LastSnapshot.wImgWidth;
                iHeight = Result->CIMG_LastSnapshot.wImgHeight;
                break;
            default:
                break;
            }

            if (ImageType == COLOR_VEHICLE_HEAD_IMG || ImageType == FULL_IMG)
            {
                std::wstring wstrOverlayInfo = Img_string2wstring(Overlay);
                g_overlayText = wstrOverlayInfo;
                ImgDataStruct dataStruct;
                dataStruct.srcImgData = (UCHAR*)pData;
                dataStruct.srcImgDataLengh = iDataLength;
                OverlayInfo oInfo;
                oInfo.iColorR = 255;
                oInfo.iColorG = 255;
                oInfo.iColorB = 255;
                oInfo.iFontSize = 32;
                oInfo.szOverlayString = wstrOverlayInfo.c_str();
                uint8_t* pBmpBuffer = GetImgBufferAddress();
                long iDestBufSize = MAX_IMG_SIZE;

                int iRet = DrawStringToImg(dataStruct, oInfo, pBmpBuffer, iDestBufSize);
                dataStruct.srcImgData = NULL;
                oInfo.szOverlayString = NULL;
                if (iRet == 0)
                {
                    g_WriteLog("DrawStringToImg success.");
                    uint8_t* pJPGBuffer = GetImgBufferAddress2();
                    DWORD iJpgBufSize = MAX_IMG_SIZE;                    

                    if (Tool_Img_ScaleJpg(pBmpBuffer, iDestBufSize, pJPGBuffer, &iJpgBufSize, iWidth, iHeight, 50))
                    {
                        g_WriteLog("compress image success.");
                        bRet = Tool_SaveFileToDisk(ImageInfo, pJPGBuffer, iJpgBufSize);
                    }
                    else
                    {
                        g_WriteLog("compress image failed, use orig image.");
                        bRet = Tool_SaveFileToDisk(ImageInfo, pData, iDataLength);
                    }
                    pJPGBuffer = NULL;
                }
                else
                {
                    g_WriteLog("DrawStringToImg failed, use default.");
                    bRet = Tool_SaveFileToDisk(ImageInfo, pData, iDataLength);
                }
                pBmpBuffer = NULL;
            }
            else
            {
                if (ImageType == BIN_IMG)
                {
                    if (pData)
                    {
                        uint8_t* pBmpBuffer = GetImgBufferAddress();
                        int ibmpBufLength = MAX_IMG_SIZE;
                        uint8_t* pJpgBuffer = GetImgBufferAddress2();
                        DWORD dwJpgBufLength = MAX_IMG_SIZE;

                        Tool_Bin2BMP((PBYTE)pData, pBmpBuffer, ibmpBufLength);
                        bRet = Tool_Img_ScaleJpg(pBmpBuffer, ibmpBufLength, pJpgBuffer, &dwJpgBufLength, iWidth, iHeight, 80);
                        if (bRet)
                        {
                            g_WriteLog("二值化图片数据转JPEG成功.");
                            bRet = Tool_SaveFileToDisk(ImageInfo, pJpgBuffer, dwJpgBufLength);
                        }
                        else
                        {
                            g_WriteLog("二值化图片数据转JPEG失败.");
                        }
                    }
                    else
                    {
                        g_WriteLog("二值化图片数据不存在.");
                    }
                }
                else if (ImageType == PLATE_IMG)
                {
                    if (pData)
                    {
                        uint8_t* pJpgBuffer = GetImgBufferAddress2();
                        DWORD dwJpgBufLength = MAX_IMG_SIZE;
                        bRet = Tool_Img_ScaleJpg((PBYTE)pData, iDataLength, pJpgBuffer, &dwJpgBufLength, iWidth, iHeight, 80);
                        if (bRet)
                        {
                            g_WriteLog("车牌小图数据 转换 jpeg 成功.");
                            bRet = Tool_SaveFileToDisk(ImageInfo, pJpgBuffer, dwJpgBufLength);
                        }
                        else
                        {
                            g_WriteLog("车牌小图数据 转换 jpeg 失败.");
                        }
                    }
                    else
                    {
                        g_WriteLog("车牌小图数据不存在.");
                    }
                }
                else
                {
                    g_WriteLog("传入图片数据格式不存在.");
                }
            }            
            
            if (bRet)
            {
                g_WriteLog("Tool_SaveFileToDisk succeed.");
            }
            else
            {
                g_WriteLog("Tool_SaveFileToDisk failed.");
            }
        }
        else
        {
            g_WriteLog("The result is not ready.");
        }
    }

    memset(chLog, 0, sizeof(chLog));
    sprintf_s(chLog, sizeof(chLog), "Plate_GetImage, end");
    g_WriteLog(chLog);
    return  (bRet) ? TRUE : FALSE;
}

PLATERECOGNIZE_API BOOL CALLING_CONVENTION Plate_GetState(int* State)
{
    char chLog[256] = { 0 };
    sprintf_s(chLog, sizeof(chLog), "Plate_GetState, begin, State = %p", State);
    g_WriteLog(chLog);

    bool bRet = FALSE;
    if (CheckCamerIsValid())
    {
        *State = g_pCamera->GetCamStatus();
    }
    else
    {
        *State = 1;
    }

    memset(chLog, 0, sizeof(chLog));
    sprintf_s(chLog, sizeof(chLog), "Plate_GetState, end");
    g_WriteLog(chLog);
    return  (bRet) ? TRUE : FALSE;
}

PLATERECOGNIZE_API BOOL CALLING_CONVENTION Plate_Capture()
{
    char chLog[256] = { 0 };
    sprintf_s(chLog, sizeof(chLog), "Plate_Capture, begin");
    g_WriteLog(chLog);

    bool bRet = FALSE;
    if (CheckCamerIsValid())
    {
        bRet = g_pCamera->TakeCapture();
    }

    memset(chLog, 0, sizeof(chLog));
    if (bRet)
    {
        sprintf_s(chLog, sizeof(chLog), "Plate_Capture, end, TakeCapture succeed.");
    }
    else
    {
        sprintf_s(chLog, sizeof(chLog), "Plate_Capture, end, TakeCapture failed.");
    }
    g_WriteLog(chLog);
    return  ( bRet ) ? TRUE : FALSE;
}

PLATERECOGNIZE_API BOOL CALLING_CONVENTION Plate_InitVideo(HWND hHandle, char * ErrInfo)
{
    char chLog[256] = { 0 };
    sprintf_s(chLog, sizeof(chLog), "Plate_InitVideo, begin, hHandle= %d, ErrInfo= %p", hHandle, ErrInfo);
    g_WriteLog(chLog);

    bool bRet = FALSE;
    char szErrInfo[128] = { 0 };
    if (CheckCamerIsValid())
    {
        g_pCamera->SetVideoPlayHandle(hHandle, g_iVideoWidth, g_iVideoHeight);
        bRet = g_pCamera->StartPlayVideo();        
        if (bRet)
        {
            sprintf_s(szErrInfo, sizeof(szErrInfo), "Init video succeed.");
        }
        else
        {
            sprintf_s(szErrInfo, sizeof(szErrInfo), "Init video failed, please check Network connection status.");
        }
    }
    else
    {
        sprintf_s(szErrInfo, sizeof(szErrInfo), "Init video failed, please init camera first.");
    }
    memcpy(ErrInfo, szErrInfo, strlen(szErrInfo));

    memset(chLog, 0, sizeof(chLog));
    sprintf_s(chLog, sizeof(chLog), "Plate_InitVideo, end, ErrInfo = %s", ErrInfo);
    g_WriteLog(chLog);
    return  (bRet) ? TRUE : FALSE;
}

PLATERECOGNIZE_API BOOL CALLING_CONVENTION Plate_CloseVideo(HWND hHandle)
{
    char chLog[256] = { 0 };
    sprintf_s(chLog, sizeof(chLog), "Plate_CloseVideo, begin, hHandle= %d", hHandle);
    g_WriteLog(chLog);

    bool bRet = FALSE;
    if (CheckCamerIsValid())
    {
        g_pCamera->StopPlayVideo();
        bRet = true;
        g_WriteLog("StopPlayVideo succeed.");
    }

    memset(chLog, 0, sizeof(chLog));
    sprintf_s(chLog, sizeof(chLog), "Plate_CloseVideo, end");
    g_WriteLog(chLog);
    return  (bRet) ? TRUE : FALSE;
}

PLATERECOGNIZE_API BOOL CALLING_CONVENTION Plate_Screenshot(char * ImageInfo)
{
    char chLog[256] = { 0 };
    sprintf_s(chLog, sizeof(chLog), "Plate_Screenshot, begin, ImageInfo= %s", ImageInfo);
    g_WriteLog(chLog);

    bool bRet = FALSE;
    if (CheckCamerIsValid())
    {
        int iBufferSize = COMPRESS_IMG_SIZE;
        uint8_t* pBuffer = GetImgBufferAddress();
        bRet = g_pCamera->GetOneImgFromVideo(1, pBuffer, &iBufferSize);
        if (!bRet)
        {
            g_WriteLog("GetOneImgFromVideo failed.");
        }
        else
        {
            memset(chLog, 0, sizeof(chLog));
            sprintf_s(chLog, sizeof(chLog), "GetOneImgFromVideo, succeed, image length = %d", iBufferSize);
            g_WriteLog("GetOneImgFromVideo succeed.");
            if (g_overlayText.length() > 0)
            {
                ImgDataStruct dataStruct;
                dataStruct.srcImgData = pBuffer;
                dataStruct.srcImgDataLengh = iBufferSize;
                OverlayInfo oInfo;
                oInfo.iColorR = 255;
                oInfo.iColorG = 255;
                oInfo.iColorB = 255;
                oInfo.iFontSize = 32;
                oInfo.szOverlayString = g_overlayText.c_str();
                uint8_t* pDestBuffer = new uint8_t[MAX_IMG_SIZE];
                long iDestBufSize = MAX_IMG_SIZE;

                if (0 == DrawStringToImg(dataStruct, oInfo, pDestBuffer, iDestBufSize))
                {
                    g_WriteLog("DrawStringToImg success.");
                    uint8_t* pJPGBuffer = GetImgBufferAddress();
                    DWORD iJpgBufSize = MAX_IMG_SIZE;

                    if (Tool_Img_ScaleJpg(pDestBuffer, iDestBufSize, pJPGBuffer, &iJpgBufSize, 1920, 1080, 30))
                    {
                        g_WriteLog("compress image success.");
                        bRet = Tool_SaveFileToDisk(ImageInfo, pJPGBuffer, iJpgBufSize);
                        pJPGBuffer = NULL;
                    }
                    else
                    {
                        g_WriteLog("compress image failed, use orig image.");
                        bRet = Tool_SaveFileToDisk(ImageInfo, pBuffer, iBufferSize);
                    }                    
                }
                else
                {
                    g_WriteLog("DrawStringToImg failed, use default.");
                    bRet = Tool_SaveFileToDisk(ImageInfo, pBuffer, iBufferSize);
                }                
                dataStruct.srcImgData = NULL;
                oInfo.szOverlayString = NULL;

                if (pDestBuffer)
                {
                    delete[] pDestBuffer;
                    pDestBuffer = NULL;
                }
            }
            else
            {
                g_WriteLog("Overlay info is less than 0,  use orig image.");
                bRet = Tool_SaveFileToDisk(ImageInfo, pBuffer, iBufferSize);
            }
            
            if (!bRet)
            {
                g_WriteLog("Tool_SaveFileToDisk failed.");
            }
        }
        pBuffer = NULL;
    }

    memset(chLog, 0, sizeof(chLog));
    sprintf_s(chLog, sizeof(chLog), "Plate_Screenshot, end");
    g_WriteLog(chLog);
    return  (bRet) ? TRUE : FALSE;
}

PLATERECOGNIZE_API BOOL CALLING_CONVENTION Plate_Timing(char* StandardTime, char * ReturnTime)
{
    char chLog[256] = { 0 };
    sprintf_s(chLog, sizeof(chLog), "Plate_Timing, begin, StandardTime= %s, ReturnTime= %p", StandardTime, ReturnTime);
    g_WriteLog(chLog);

    if (!StandardTime || !ReturnTime || strlen(StandardTime) <14 || strlen(StandardTime) > 19)
    {
        g_WriteLog("Plate_Timing, StandardTime or ReturnTime is invalid, return false. ");
        return FALSE;
    }

    bool bRet = false;
    if (CheckCamerIsValid())
    {
        int iYear = 0, iMonth = 0, iDay = 0, iHour = 0, iMin = 0, iSecond = 0;
        sscanf_s(StandardTime, "%d-%d-%d %d:%d:%d", &iYear, &iMonth, &iDay, &iHour, &iMin, &iSecond);

        memset(chLog, 0, sizeof(chLog));
        sprintf_s(chLog, sizeof(chLog), "Plate_Timing, get the time value year= %d, month= %d, day = %d, hour = %d, minute = %d, second= %d",
            iYear, iMonth, iDay, iHour, iMin, iSecond);
        g_WriteLog(chLog);

        if (iYear < 2000 || iYear > 2100 || iMonth < 1 || iMonth > 12
            || iHour < 0 || iHour > 23 || iMin < 0 || iMin > 59 || iSecond < 0 || iSecond > 59)
        {
            g_WriteLog("the value is invalid, return false.");
            return FALSE;
        }
        if ((((iYear % 4 == 0 && iYear % 100 != 0) || (iYear % 400 == 0)) && iDay > 29 && iMonth == 2)
            || (!((iYear % 4 == 0 && iYear % 100 != 0) || (iYear % 400 == 0)) && iDay > 28 && iMonth == 2)
            || ((iMonth == 1 || iMonth == 3 || iMonth == 5 || iMonth == 7 || iMonth == 8 || iMonth == 10 || iMonth == 12) && iDay > 31)
            || (!(iMonth == 1 || iMonth == 3 || iMonth == 5 || iMonth == 7 || iMonth == 8 || iMonth == 10 || iMonth == 12) && iDay > 30)
            || iDay < 1)
        {
            g_WriteLog("the time value is invalid, return false.");
            return FALSE;
        }

        if (g_pCamera->SynTime(iYear, iMonth, iDay, iHour, iMin, iSecond, 0))
        {
            g_WriteLog("SynTime succeed.");
            bRet = true;
        }
        else
        {
            g_WriteLog("SynTime failed.");
        }
        DeviceTime deviceTime;
        char chDeviceTime[128] = {0};
        if (g_pCamera->GetDeviceTime(deviceTime))
        {
            sprintf_s(chDeviceTime, sizeof(chDeviceTime), "%d-%02d-%02d %02d:%02d:%02d",
                deviceTime.iYear,
                deviceTime.iMonth,
                deviceTime.iDay,
                deviceTime.iHour,
                deviceTime.iMinutes,
                deviceTime.iSecond);

            memset(chLog, 0, sizeof(chLog));
            sprintf_s(chLog, sizeof(chLog), "Plate_Timing, get device time succeed, time = %s", chDeviceTime);
            g_WriteLog(chLog);
            memcpy(ReturnTime, chDeviceTime, strlen(chDeviceTime));
        }
        else
        {
            g_WriteLog("get device time failed.");
        }
    }

    memset(chLog, 0, sizeof(chLog));
    sprintf_s(chLog, sizeof(chLog), "Plate_Timing, end, ReturnTime = %s", ReturnTime);
    g_WriteLog(chLog);
    return  (bRet) ? TRUE : FALSE;
}

PLATERECOGNIZE_API BOOL CALLING_CONVENTION Plate_ShowTime()
{
    char chLog[256] = { 0 };
    sprintf_s(chLog, sizeof(chLog), "Plate_ShowTime, begin");
    g_WriteLog(chLog);

    bool bRet = false;
    if (CheckCamerIsValid())
    {
        g_pCamera->SetOverlayVideoTextEnable(0, true);
        //g_pCamera->SetOverlayTimePos(0, g_iTimePosX, g_iTimePosY);
        if (g_pCamera->SetOverlayTimeEnable(0, true))
        {
            g_WriteLog("show video time succeed.");
        }
        else
        {
            g_WriteLog("show video time failed.");
        }
    }

    memset(chLog, 0, sizeof(chLog));
    sprintf_s(chLog, sizeof(chLog), "Plate_ShowTime, end.");
    g_WriteLog(chLog);
    return  (bRet) ? TRUE : FALSE;
}

PLATERECOGNIZE_API BOOL CALLING_CONVENTION Plate_CloseTime()
{
    char chLog[256] = { 0 };
    sprintf_s(chLog, sizeof(chLog), "Plate_CloseTime, begin");
    g_WriteLog(chLog);

    bool bRet = false;
    if (CheckCamerIsValid())
    {
        if (g_pCamera->SetOverlayTimeEnable(0, false))
        {
            g_WriteLog("close video time succeed.");
        }
        else
        {
            g_WriteLog("close video time failed.");
        }
    }

    memset(chLog, 0, sizeof(chLog));
    sprintf_s(chLog, sizeof(chLog), "Plate_CloseTime, end.");
    g_WriteLog(chLog);
    return  (bRet) ? TRUE : FALSE;
}

PLATERECOGNIZE_API BOOL CALLING_CONVENTION Plate_GetDeviceInfo(char * PlateInfo)
{
    char chLog[256] = { 0 };
    sprintf_s(chLog, sizeof(chLog), "Plate_GetDeviceInfo, begin, PlateInfo = %p", PlateInfo);
    g_WriteLog(chLog);

    bool bRet = false;
    if (CheckCamerIsValid())
    {
        BasicInfo info;
        if (g_pCamera->GetHardWareInfo(info))
        {
            TCHAR szFileName[MAX_PATH] = { 0 };
            GetModuleFileName(NULL, szFileName, MAX_PATH);	//取得包括程序名的全路径
            PathRemoveFileSpec(szFileName);				//去掉程序名
            char chPath[MAX_PATH] = { 0 };
            MY_SPRINTF(chPath, sizeof(chPath), "%s\\PLATERECOGNIZE.dll", szFileName);
            std::string strApiVerion = GetSoftVersion(chPath);

            //                                    例如：2 | HKWS - 1 - ds | 2.0.1 | 3.2.1.1
            //                                    表示为海康威视的型号为HKWS - 1 - ds的车牌识别，其固件版本号为2.0.1，动态库版本号为3.2.1.1
            char szDevType[128] = { 0 }, szDevVersion[64] = { 0 }, szDllVersion[64] = {0};
            memcpy(szDevType, info.szDevType, strlen(info.szDevType));
            memcpy(szDevVersion, info.szDevVersion, strlen(info.szDevVersion));

            char szVersionInfo[256] = {0};
            sprintf_s(szVersionInfo, sizeof(szVersionInfo), "%d | %s | %s | %s", 1, szDevType, szDevVersion, strApiVerion.c_str());

            memset(chLog, 0, sizeof(chLog));
            sprintf_s(chLog, sizeof(chLog), "Plate_GetDeviceInfo, Get version info %s.", szVersionInfo);
            g_WriteLog(chLog);

            memcpy(PlateInfo, szVersionInfo, strlen(szVersionInfo));
            bRet = true;
        }
    }

    memset(chLog, 0, sizeof(chLog));
    sprintf_s(chLog, sizeof(chLog), "Plate_GetDeviceInfo, end, PlateInfo = %s.", PlateInfo);
    g_WriteLog(chLog);
    return  (bRet) ? TRUE : FALSE;
}
