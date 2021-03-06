#include "stdafx.h"
#include "MyH264Saver.h"
#include "utilityFunc/ToolFunction.h"
//#include <QDebug>

#define VIDEO_FRAME_LIST_SIZE (1024)
#define  WINDOWS


MyH264Saver::MyH264Saver():
m_bExit(false),
m_iSaveH264Flag(0),
m_iTimeFlag(0),
M_iStopTimeFlag(TIME_FLAG_UNDEFINE),
m_iTmpTime(0),
m_lastvideoidx(-1),
m_bFirstSave(false),
//m_pLocker(NULL),
m_hThreadSaveH264(NULL)
{
    InitializeCriticalSection(&m_Locker);
	InitializeCriticalSection(&m_DataListLocker);

    m_hThreadSaveH264 = CreateThread(NULL, 0, H264DataProceesor, this, 0, NULL);
}


MyH264Saver::~MyH264Saver()
{
    SetIfExit(true);
    SetSaveFlag(false);
    Tool_SafeCloseThread(m_hThreadSaveH264);

    DeleteCriticalSection(&m_Locker);
	DeleteCriticalSection(&m_DataListLocker);
}

bool MyH264Saver::addDataStruct(CustH264Struct* pDataStruct)
{
    if (pDataStruct == NULL)
    {
        return false;
    }
	//char buf[256] = { 0 };
	//sprintf_s(buf, "%ld\n", pDataStruct->m_llFrameTime);
	//OutputDebugString(buf);

    std::shared_ptr<CustH264Struct> pData = std::shared_ptr<CustH264Struct>(pDataStruct);
	EnterCriticalSection(&m_DataListLocker);
    if (m_lDataStructList.size() > VIDEO_FRAME_LIST_SIZE)
    {
        m_lDataStructList.pop_front();
    }
    m_lDataStructList.push_back(pData);
	LeaveCriticalSection(&m_DataListLocker);

    return true;
}

bool MyH264Saver::StartSaveH264(INT64 beginTimeStamp, const char* pchFilePath)
{
    //qDebug()<< "StartSaveH264 :"<< "beginTimeStamp "<< beginTimeStamp;
    WriteFormatLog("MyH264Saver::StartSaveH264 beginTimeStamp = %I64d, pchFilePath = %s", beginTimeStamp, pchFilePath);
    SetSavePath(pchFilePath, strlen(pchFilePath));
    SetTimeFlag(beginTimeStamp);
    SetIfFirstSave(true);
    SetSaveFlag(SAVING_FLAG_SAVING);
    SetStopTimeFlag(TIME_FLAG_UNDEFINE);

	m_lastvideoidx = -1;
    return true;
}

bool MyH264Saver::StopSaveH264(INT64 TimeFlag)
{
    //SetSaveFlag(SAVING_FLAG_SHUT_DOWN);
    SetStopTimeFlag(TimeFlag);
    return true;
}

DWORD WINAPI MyH264Saver::H264DataProceesor(LPVOID lpThreadParameter)
{
    if (NULL == lpThreadParameter)
    {
        return 0;
    }
    MyH264Saver* pSaver = (MyH264Saver*)lpThreadParameter;
    return pSaver->processH264Data();
}

//DWORD MyH264Saver::processH264Data()
//{
//    int  iSaveFlag = 0;
//    int iVideoWidth = -1;
//    int iVideoHeight = -1;
//    while (!GetIfExit())
//    {
//        //Sleep(50);
//		EnterCriticalSection(&m_DataListLocker);
//        if (m_lDataStructList.size() <= 0)
//        {
//			LeaveCriticalSection(&m_DataListLocker);
//            Sleep(50);
//            continue;
//        }
//		LeaveCriticalSection(&m_DataListLocker);
//
//		char buf[256] = { 0 };
//
//        if( GetStopTimeFlag() != TIME_FLAG_UNDEFINE
//                && GetStopTimeFlag() < GetTickCount() )
//        {
//            //qDebug()<< "GetStopTimeFlag "  <<GetStopTimeFlag() << "< "<< "GetTickCount" <<GetTickCount();
//            iSaveFlag = SAVING_FLAG_SHUT_DOWN;
//            SetStopTimeFlag(TIME_FLAG_UNDEFINE);
//        }
//        else
//        {
//            iSaveFlag = GetSaveFlag();
//        }
//
//        std::shared_ptr<CustH264Struct > pData = nullptr;
//        switch (iSaveFlag)
//        {
//        case SAVING_FLAG_NOT_SAVE:
//            Sleep(10);
//            break;
//        case SAVING_FLAG_SAVING:
//            EnterCriticalSection(&m_DataListLocker);
//            for (auto r = m_lDataStructList.begin(); r != m_lDataStructList.end(); )
//            {
//                if ((*r)->m_llFrameTime < GetTimeFlag())
//                {
//                    r = m_lDataStructList.erase(r);
//                    //r++;
//                }
//                else
//                {
//                    if ( (*r)->m_llFrameTime > m_iTmpTime
//                            || ( (*r)->m_llFrameTime == m_iTmpTime && ( (*r)->index == m_lastvideoidx + 1 || ((*r)->index == 0 && m_lastvideoidx == 400))))
//                    {
//                        pData = (*r);
//                        m_iTmpTime = (*r)->m_llFrameTime;
//                        m_lastvideoidx = (*r)->index;
//                        break;
//                    }
//                    r++;
//                }
//            }
//            LeaveCriticalSection(&m_DataListLocker);
//
//            //pData = m_lDataStructList.front();
//
//            if (pData == nullptr)
//            {
//                break;
//            }
//
////            sprintf_s(buf, "%ld, ", pData->m_llFrameTime);
////            OutputDebugStringA(buf);
////            sprintf_s(buf, "index = %d\n",  pData->index);
////            OutputDebugStringA(buf);
//
//            //EnterCriticalSection(&m_DataListLocker);
//            //m_lDataStructList.pop_front();
//            //LeaveCriticalSection(&m_DataListLocker);
//            iVideoWidth = pData->m_iWidth;
//            iVideoHeight = pData->m_iHeight;
//            if (GetIfFirstSave())
//            {
//                if (!m_264AviLib.IsNULL())
//                {
//                    m_264AviLib.close();
//                }
//                if (iVideoWidth > 0
//                    && iVideoHeight > 0)
//                {
//                    m_264AviLib.setAviInfo((char*)GetSavePath(), iVideoWidth, iVideoHeight, 25, "H264");
//                    SetIfFirstSave(false);
//                }
//            }
//            if (!m_264AviLib.IsNULL()
//                && pData->m_llFrameTime > GetTimeFlag()
//                && 0== m_264AviLib.writeFrame((char*)pData->m_pbH264FrameData, pData->m_iDataSize, pData->m_isIFrame))
//            {
//                //SetIfFirstSave(false);
//            }
//            break;
//        case SAVING_FLAG_SHUT_DOWN:
//            if (!m_264AviLib.IsNULL())
//            {
//				m_iTmpTime = 0;
//                m_264AviLib.close();
//                SetSaveFlag(SAVING_FLAG_NOT_SAVE);
//            }
//            break;
//        default:
//            break;
//        }       
//
//    }
//    return 0;
//}

DWORD MyH264Saver::processH264Data()
{
    WriteFormatLog("MyH264Saver::processH264Data begin.\n");
    int  iSaveFlag = 0;
    int iVideoWidth = -1;
    int iVideoHeight = -1;

    int iFlag = 1;

    long long iCurrentFrameTimeFlag = -1;
    long long iLastTimeFlag = -1;
    long long iVideoStopTimeFlag = -1;

    long long iLastSaveFlag = -1;
    long long iVideoBeginTimeFlag = -1;
    long long iTimeNowFlag = -1;

    int iLastIndex = -1;
    int iCurrentFrameIndex = -1;
    while (!GetIfExit())
    {
        //Sleep(50);
#ifdef WINDOWS
        EnterCriticalSection(&m_DataListLocker);
#else
        m_DataListLocker.lock();
#endif
        if (m_lDataStructList.size() <= 0)
        {
#ifdef WINDOWS
            LeaveCriticalSection(&m_DataListLocker);
#else
            m_DataListLocker.unlock();
#endif
            //printf("MyH264Saver::processH264Data m_lDataStructList.size() <= 0 \n");
            //usleep(10 * 1000);
            Sleep(10);
            continue;
        }
#ifdef WINDOWS
        LeaveCriticalSection(&m_DataListLocker);
#else
        m_DataListLocker.unlock();
#endif
        char buf[256] = { 0 };
        iVideoStopTimeFlag = GetStopTimeFlag();
        iVideoBeginTimeFlag = GetTimeFlag();
        iTimeNowFlag = GetTickCount();

        //        if( iStopTimeFlag != TIME_FLAG_UNDEFINE
        //                &&  iTimeNowFlag > iStopTimeFlag )
        //        {
        //            //qDebug()<< "GetStopTimeFlag "  <<GetStopTimeFlag() << "< "<< "GetTickCount" <<GetTickCount();
        //            iSaveFlag = SAVING_FLAG_SHUT_DOWN;
        //            SetStopTimeFlag(TIME_FLAG_UNDEFINE);
        //        }
        //        else
        {
            iSaveFlag = GetSaveFlag();
        }

        if (iSaveFlag != iLastSaveFlag)
        {
            WriteFormatLog("stop time flag change, lastStopTimeFlag = %I64d, current Stop time flag = %I64d, system time Now = %I64d,  video beginTIme = %I64d, save flag = %d\n",
                iLastSaveFlag,
                iVideoStopTimeFlag,
                iTimeNowFlag,
                iVideoBeginTimeFlag,
                iSaveFlag);
            iLastSaveFlag = iSaveFlag;
        }

        std::shared_ptr<CustH264Struct > pData = nullptr;
        switch (iSaveFlag)
        {
        case SAVING_FLAG_NOT_SAVE:
            //usleep(10 * 1000);
            //printf("MyH264Saver::processH264Data SAVING_FLAG_NOT_SAVE, break \n");
            Sleep(10);
            break;
        case SAVING_FLAG_SAVING:
            //printf("MyH264Saver::processH264Data SAVING_FLAG_SAVING \n",GetSavePath() );
#ifdef WINDOWS
            EnterCriticalSection(&m_DataListLocker);
#else
            m_DataListLocker.lock();
#endif

            pData = m_lDataStructList.front();
            m_lDataStructList.pop_front();

#ifdef WINDOWS
            LeaveCriticalSection(&m_DataListLocker);
#else
            m_DataListLocker.unlock();
#endif

            iCurrentFrameTimeFlag = pData->m_llFrameTime;
            iCurrentFrameIndex = pData->index;
            //iStopTimeFlag = GetTimeFlag();
            if (iCurrentFrameTimeFlag < iVideoBeginTimeFlag)
            {
                WriteFormatLog("iCurrentTimeFlag (%I64d) < iVideoBeginTimeFlag (%I64d), erase frame, index = %d",
                    iCurrentFrameTimeFlag,
                    iVideoBeginTimeFlag,
                    iCurrentFrameIndex);
                pData = nullptr;
                continue;
            }
            else
            {
                if (iVideoStopTimeFlag != TIME_FLAG_UNDEFINE
                    && iCurrentFrameTimeFlag >= iVideoStopTimeFlag)
                {
                    WriteFormatLog(" iStopTimeFlag != TIME_FLAG_UNDEFINE  && iCurrentTimeFlag (%I64d) >= iStopTimeFlag (%I64d), change saveFlag ",
                        iCurrentFrameTimeFlag,
                        iVideoStopTimeFlag);
                    SetSaveFlag(SAVING_FLAG_SHUT_DOWN);

                    iLastTimeFlag = -1;
                    iLastIndex = -1;
                }
                else
                {
                    if (iLastTimeFlag == -1
                        || iLastIndex == -1
                        || ((iCurrentFrameTimeFlag > iLastTimeFlag) && (iCurrentFrameIndex == iLastIndex + 1))
                        || ((iCurrentFrameTimeFlag > iLastTimeFlag) && (iCurrentFrameIndex == 0) && iLastIndex == 400)
                        )
                    {
                        WriteFormatLog("iCurrentTimeFlag (%I64d) > iVideoBeginTimeFlag (%I64d),and < video stop time(%I64d) save frame, index = %d, iLastTimeFlag = %I64d, iLastIndex = %d",
                            iCurrentFrameTimeFlag,
                            iVideoBeginTimeFlag,
                            iVideoStopTimeFlag,
                            iCurrentFrameIndex,
                            iLastTimeFlag,
                            iLastIndex);

                        iLastTimeFlag = iCurrentFrameTimeFlag;
                        iLastIndex = iCurrentFrameIndex;
                    }
                    else
                    {
                        WriteFormatLog("iCurrentTimeFlag (%I64d) > iVideoBeginTimeFlag (%I64d),and < video stop time(%I64d) but nosave frame, index = %d, iLastTimeFlag = %I64d, iLastIndex = %d",
                            iCurrentFrameTimeFlag,
                            iVideoBeginTimeFlag,
                            iVideoStopTimeFlag,
                            iCurrentFrameIndex,
                            iLastTimeFlag,
                            iLastIndex);
                        pData = nullptr;
                    }
                }
            }

            //#ifdef WINDOWS
            //            LeaveCriticalSection(&m_DataListLocker);
            //#else
            //            m_DataListLocker.unlock();
            //#endif

            //pData = m_lDataStructList.front();

            if (pData == nullptr)
            {
                //printf("MyH264Saver::processH264Data SAVING_FLAG_SAVING  pData == nullptr\n" );
                break;
            }

            iVideoWidth = pData->m_iWidth;
            iVideoHeight = pData->m_iHeight;
            if (GetIfFirstSave())
            {
                if (!m_264AviLib.IsNULL())
                {
                    WriteFormatLog("m_264AviLib.IsNULL == false, close file.\n");
                    m_264AviLib.close();
                }
                if (iVideoWidth > 0
                    && iVideoHeight > 0)
                {
                    WriteFormatLog("m_264AviLib.setAviInfo , %s.\n", GetSavePath());
                    m_264AviLib.setAviInfo((char*)GetSavePath(), iVideoWidth, iVideoHeight, 25, "H264");
                    SetIfFirstSave(false);
                }
            }

            if (!m_264AviLib.IsNULL()
                && pData->m_llFrameTime > GetTimeFlag()
                )
            {
                iFlag = m_264AviLib.writeFrame((char*)pData->m_pbH264FrameData, pData->m_iDataSize, pData->m_isIFrame);
                if (iFlag != 0)
                {
                    WriteFormatLog(" m_264AviLib.writeFrame = %d\n", iFlag);
                }
                //SetIfFirstSave(false);
            }
            pData = nullptr;
            break;
        case SAVING_FLAG_SHUT_DOWN:
            if (!m_264AviLib.IsNULL())
            {
                m_iTmpTime = 0;
                m_264AviLib.close();
                //SetSaveFlag(SAVING_FLAG_NOT_SAVE);
                WriteFormatLog("m_264AviLib.close SAVING_FLAG_SHUT_DOWN.\n", GetSavePath());
            }
            else
            {
                WriteFormatLog("MyH264Saver::processH264Data SAVING_FLAG_SHUT_DOWN m_264AviLib.IsNULL.\n", GetSavePath());
            }
            SetSaveFlag(SAVING_FLAG_NOT_SAVE);
            break;
        default:
            WriteFormatLog("MyH264Saver::processH264Data default break.\n", GetSavePath());
            break;
        }

    }

    WriteFormatLog("MyH264Saver::processH264Data finish.\n");
    return 0;
}



void MyH264Saver::SetIfExit(bool bValue)
{
    EnterCriticalSection(&m_Locker);
    m_bExit = bValue;
    LeaveCriticalSection(&m_Locker);
}

bool MyH264Saver::GetIfExit()
{
    bool bValue = false;
    EnterCriticalSection(&m_Locker);
    bValue = m_bExit;
    LeaveCriticalSection(&m_Locker);
    return bValue;
}

void MyH264Saver::SetSaveFlag(int iValue)
{
    WriteFormatLog("MyH264Saver::SetSaveFlag %d", iValue);
    EnterCriticalSection(&m_Locker);
    m_iSaveH264Flag = iValue;
    LeaveCriticalSection(&m_Locker);
}

int MyH264Saver::GetSaveFlag()
{
    int iValue = false;
    EnterCriticalSection(&m_Locker);
    iValue = m_iSaveH264Flag;
    LeaveCriticalSection(&m_Locker);
    return iValue;
}

void MyH264Saver::SetTimeFlag(INT64 iValue)
{
    WriteFormatLog("MyH264Saver::SetTimeFlag %I64d", iValue);
    EnterCriticalSection(&m_Locker);
    m_iTimeFlag = iValue;
	m_iTmpTime = iValue;
    LeaveCriticalSection(&m_Locker);
}

INT64 MyH264Saver::GetTimeFlag()
{
    INT64 iValue = false;
    EnterCriticalSection(&m_Locker);
    iValue = m_iTimeFlag;
    LeaveCriticalSection(&m_Locker);
    return iValue;
}

void MyH264Saver::SetStopTimeFlag(INT64 iValue)
{
    WriteFormatLog("MyH264Saver::SetStopTimeFlag %I64d", iValue);
    EnterCriticalSection(&m_Locker);
    M_iStopTimeFlag = iValue;
    LeaveCriticalSection(&m_Locker);
}

INT64 MyH264Saver::GetStopTimeFlag()
{
    INT64 iValue = false;
    EnterCriticalSection(&m_Locker);
    iValue = M_iStopTimeFlag;
    LeaveCriticalSection(&m_Locker);
    return iValue;
}

void MyH264Saver::SetIfFirstSave(bool bValue)
{
    WriteFormatLog("MyH264Saver::SetIfFirstSave %d", bValue);
    EnterCriticalSection(&m_Locker);
    m_bFirstSave = bValue;
    LeaveCriticalSection(&m_Locker);
}

bool MyH264Saver::GetIfFirstSave()
{
    bool bValue = false;
    EnterCriticalSection(&m_Locker);
    bValue = m_bFirstSave;
    LeaveCriticalSection(&m_Locker);
    return bValue;
}

void MyH264Saver::SetSavePath(const char* filePath, size_t bufLength)
{
    EnterCriticalSection(&m_Locker);
    if (bufLength < sizeof(m_chFilePath))
    {
        memcpy(m_chFilePath, filePath, bufLength);
        m_chFilePath[bufLength] = '\0';
    }
    LeaveCriticalSection(&m_Locker);
}

const char* MyH264Saver::GetSavePath()
{
    const char* pValue = NULL;
    EnterCriticalSection(&m_Locker);
    pValue = m_chFilePath;
    LeaveCriticalSection(&m_Locker);
    return pValue;
}


void MyH264Saver::WriteFormatLog(const char *szfmt, ...)
{
    return;

    static char g_szString[10240] = { 0 };
    memset(g_szString, 0, sizeof(g_szString));

    va_list arg_ptr;
    va_start(arg_ptr, szfmt);
    //vsnprintf_s(g_szPbString, sizeof(g_szPbString), szfmt, arg_ptr);
    //_vsnprintf(g_szPbString,  sizeof(g_szPbString),  szfmt, arg_ptr);
    vsnprintf(g_szString, sizeof(g_szString), szfmt, arg_ptr);
    va_end(arg_ptr);

    char chBuffer1[256] = { 0 };
    //_getcwd(chBuffer1, sizeof(chBuffer1));
    sprintf_s(chBuffer1, sizeof(chBuffer1), "%s", Tool_GetCurrentPath());

    SYSTEMTIME  stimeNow = Tool_GetCurrentTime();

    strcat(chBuffer1, "\\XLWLog\\");
    Tool_MakeDir(chBuffer1);

    char chLogFileName[512] = { 0 };
    //sprintf(chLogFileName, "%s/Video_%d-%02d_%02d.log",
    //    chBuffer1,
    //    stimeNow.tm_year + 1900,
    //    stimeNow.tm_mon + 1,
    //    stimeNow.tm_mday);
    sprintf_s(chLogFileName, sizeof(chLogFileName) ,"%s/Video_%d-%02d_%02d_%02d.log",
        chBuffer1,
        stimeNow.wYear,
        stimeNow.wMonth,
        stimeNow.wDay,
        stimeNow.wHour);

    FILE *file = NULL;
    file = fopen(chLogFileName, "a+");
    //fopen_s(&file, chLogFileName, "a+");
    if (file)
    {
        //fprintf(file, "[%04d-%02d-%02d %02d:%02d:%02d:%03lld] : %s\n",
        //    stimeNow.tm_year + 1900,
        //    stimeNow.tm_mon + 1,
        //    stimeNow.tm_mday,
        //    stimeNow.tm_hour,
        //    stimeNow.tm_min,
        //    stimeNow.tm_sec,
        //    iTimeInMilliseconds % 1000,
        //    g_szString);

        fprintf(file, "[%04d-%02d-%02d %02d:%02d:%02d:%03d] : %s\n",
            stimeNow.wYear,
            stimeNow.wMonth,
            stimeNow.wDay,
            stimeNow.wHour,
            stimeNow.wMinute,
            stimeNow.wSecond,
            stimeNow.wMilliseconds,
            g_szString);

        fclose(file);
        file = NULL;
    }
}