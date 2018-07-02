#include "stdafx.h"
#include "ToolFunction.h"
#include<shellapi.h>
//#include <afx.h>

#include <gdiplus.h>
using namespace Gdiplus;
#pragma  comment(lib, "gdiplus.lib")

#ifdef WIN32
#include <io.h>
#pragma comment(lib, "version.lib")  
#endif

TiXmlElement SelectElementByName(const char* InputInfo, char* pName, int iXMLType)
{
	//注：XMLTYPE 为1时，InputInfo为XML路径，当为2时,InputInfo为二进制文件内容
	TiXmlDocument cXmlDoc;
	TiXmlElement* pRootElement = NULL;
	if (iXMLType == 1)
	{
		if (!cXmlDoc.LoadFile(InputInfo))
		{
			printf("parse XML file failed \n");
			return TiXmlElement("");
		}
	}
	else if (iXMLType == 2)
	{
		if (!cXmlDoc.Parse(InputInfo))
		{
			printf("parse XML failed \n");
			return TiXmlElement("");
		}
	}

	pRootElement = cXmlDoc.RootElement();
	if (NULL == pRootElement)
	{
		printf("no have root Element\n");
		return TiXmlElement("");
	}
	else
	{
		TiXmlElement* pTempElement = NULL;
		pTempElement = ReadElememt(pRootElement, pName);
		if (pTempElement)
		{
			printf("find the Name : %s, Text = %s\n", pTempElement->Value(), pTempElement->GetText());
			return *pTempElement;
		}
		else
		{
			return TiXmlElement("");
		}		
	}
}

TiXmlElement* ReadElememt(TiXmlElement* InputElement, char* pName)
{
	TiXmlElement* ptemp = NULL;
	if (InputElement && 0 == strcmp(pName, InputElement->Value()))
	{
		printf("Find the element :%s \n", InputElement->Value());
		ptemp = InputElement;
		return ptemp;
	}
	else
	{
		printf("%s \n", InputElement->Value());
	}

	TiXmlElement* tmpElement = InputElement;
	if (tmpElement->FirstChildElement())
	{
		ptemp = ReadElememt(tmpElement->FirstChildElement(), pName);
	}
	if (!ptemp)
	{
		tmpElement = tmpElement->NextSiblingElement();
		if (tmpElement)
		{
			ptemp = ReadElememt(tmpElement, pName);
		}
	}
	return ptemp;
}

void Tool_ReadKeyValueFromConfigFile(const char* IniFileName, const char* nodeName, const char* keyName, char* keyValue, int bufferSize)
{
	if ((int)strlen(keyValue) > bufferSize)
	{
		return;
	}
	char FileName[MAX_PATH];
	GetModuleFileNameA(NULL, FileName, MAX_PATH - 1);

	PathRemoveFileSpecA(FileName);
    char iniFileName[MAX_PATH] = { 0 };
	//char iniDeviceInfoName[MAX_PATH] = { 0 };
	MY_SPRINTF(iniFileName, sizeof(iniFileName), "%s\\%s", FileName, IniFileName);

	GetPrivateProfileStringA(nodeName, keyName, "0", keyValue, bufferSize, iniFileName);

	//WritePrivateProfileStringA(nodeName, keyName, keyValue, iniFileName);
}

void g_WriteKeyValueFromConfigFile(const char* INIFileName, const char* nodeName, const char* keyName, char* keyValue, int bufferSize)
{
    if (strlen(keyValue) > (size_t)bufferSize)
	{
		return;
	}
	char FileName[MAX_PATH];
	GetModuleFileNameA(NULL, FileName, MAX_PATH - 1);

	PathRemoveFileSpecA(FileName);
	char iniFileName[MAX_PATH] = { 0 };
	char iniDeviceInfoName[MAX_PATH] = { 0 };
	strcat_s(iniFileName, FileName);
	strcat_s(iniFileName, INIFileName);


	//GetPrivateProfileStringA(nodeName, keyName, "172.18.109.97", keyValue, bufferSize, iniFileName);

	WritePrivateProfileStringA(nodeName, keyName, keyValue, iniFileName);
}

int g_checkIP(const char* p)
{
	int n[4];
	char c[4];
	//if (sscanf(p, "%d%c%d%c%d%c%d%c",
	//	&n[0], &c[0], &n[1], &c[1],
	//	&n[2], &c[2], &n[3], &c[3])
	//	== 7)
	if (sscanf_s(p, "%d%c%d%c%d%c%d%c",
		&n[0], &c[0], 1,
		&n[1], &c[1], 1,
		&n[2], &c[2], 1,
		&n[3], &c[3], 1)
		== 7)
	{
		int i;
		for (i = 0; i < 3; ++i)
		if (c[i] != '.')
			return 0;
		for (i = 0; i < 4; ++i)
		if (n[i] > 255 || n[i] < 0)
			return 0;
        if (n[0] == 0 && n[1] == 0 && n[2] == 0 && n[3] == 0)
		{
            return 0;
		}
		return 1;
	}
	else
		return 0;
}

bool Tool_IsFileExist(const char* FilePath)
{
	if (FilePath == NULL)
	{
		return false;
	}
	FILE* tempFile = NULL;
	bool bRet = false;
	//tempFile = fopen(FilePath, "r");
	fopen_s(&tempFile, FilePath, "r");
	if (tempFile)
	{
		bRet = true;
		fclose(tempFile);
		tempFile = NULL;
	}
	return bRet;
}

bool Tool_MakeDir(const char* chImgPath)
{
    if (NULL == chImgPath)
    {
        //WriteLog("the path is null ,Create Dir failed.");
        return false;
    }
    std::string tempFile(chImgPath);
    size_t iPosition = tempFile.rfind("\\");
    std::string tempDir = tempFile.substr(0, iPosition + 1);
    if (MakeSureDirectoryPathExists(tempDir.c_str()))
    {
        return true;
    }
    else
    {
        //WriteLog("Create Dir failed.");
        return false;
    }
}

long Tool_GetFileSize(const char *FileName)
{
	//FILE* tmpFile = fopen(FileName, "rb");
	FILE* tmpFile = NULL;
	fopen_s(&tmpFile, FileName, "rb");
	if (tmpFile)
	{
		fseek(tmpFile, 0, SEEK_END);
		long fileSize = ftell(tmpFile);
		fclose(tmpFile);
		tmpFile = NULL;
		return fileSize;
	}
	else
	{
		//"open file failed.";
		return 0;
	}
}

bool PingIPaddress(const char* IpAddress)
{
	//FILE* pfile;
	//char chBuffer[1024] = {0};
	char chCMD[256] = { 0 };
	sprintf_s(chCMD, "ping %s -n 1", IpAddress);
	//std::string strPingResult;
	//pfile = _popen(chCMD, "r");
	//if (pfile != NULL)
	//{
	//	while(fgets(chBuffer, 1024, pfile) != NULL)
	//	{
	//		strPingResult.append(chBuffer);
	//	}
	//}
	//else
	//{
	//	printf("popen failed. \n");
	//	return false;
	//}
	//_pclose(pfile);
	//printf("%s", strPingResult.c_str());
	//if (std::string::npos != strPingResult.find("TTL") || std::string::npos != strPingResult.find("ttl"))
	//{
	//	return true;
	//}
	//else
	//{
	//	return false;
	//}


	char pbuf[1024]; // 缓存  
	DWORD len;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HANDLE hRead1 = NULL, hWrite1 = NULL;  // 管道读写句柄  
	BOOL b;
	SECURITY_ATTRIBUTES saAttr;

	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE; // 管道句柄是可被继承的  
	saAttr.lpSecurityDescriptor = NULL;

	// 创建匿名管道，管道句柄是可被继承的  
	b = CreatePipe(&hRead1, &hWrite1, &saAttr, 1024);
	if (!b)
	{
		//MessageBox(hwnd, "管道创建失败。","Information",0);  
		printf("管道创建失败\n");
		return false;
	}

	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	si.hStdOutput = hWrite1; // 设置需要传递到子进程的管道写句柄  


	// 创建子进程，运行ping命令，子进程是可继承的  
	if (!CreateProcess(NULL, chCMD, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
	{
		//itoa(GetLastError(), pbuf, 10); 
		sprintf_s(pbuf, "%d", GetLastError());
		//MessageBox(hwnd, pbuf,"Information",0);
		printf("%s\n", pbuf);
		CloseHandle(hRead1);
		hRead1 = NULL;
		CloseHandle(hWrite1);
		hWrite1 = NULL;
		return false;
	}

	// 写端句柄已被继承，本地则可关闭，不然读管道时将被阻塞  
	CloseHandle(hWrite1);
	hWrite1 = NULL;

	// 读管道内容，并用消息框显示  
	len = 1000;
	DWORD l;

	std::string strInfo;
	while (ReadFile(hRead1, pbuf, len, &l, NULL))
	{
		if (l == 0) break;
		pbuf[l] = '\0';
		//MessageBox(hwnd, pbuf, "Information",0);  
		//printf("Information2:\n%s\n", pbuf);
		strInfo.append(pbuf);
		len = 1000;
	}

	//MessageBox(hwnd, "ReadFile Exit","Information",0);  
	printf("finish ReadFile buffer = %s\n", strInfo.c_str());
	CloseHandle(hRead1);
	hRead1 = NULL;

	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hThread);
	pi.hThread = NULL;
	CloseHandle(pi.hProcess);
	pi.hProcess = NULL;

	if (std::string::npos != strInfo.find("TTL") || std::string::npos != strInfo.find("ttl"))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Tool_Img_ScaleJpg(PBYTE pbSrc, int iSrcLen, PBYTE pbDst, size_t *iDstLen, int iDstWidth, int iDstHeight, int compressQuality)
{
    if (pbSrc == NULL || iSrcLen <= 0)
    {
        return false;
    }
    if (pbDst == NULL || iDstLen == NULL || *iDstLen <= 0)
    {
        return false;
    }
    if (iDstWidth <= 0 || iDstHeight <= 0)
    {
        return false;
    }

    // init gdi+
    ULONG_PTR gdiplusToken = NULL;
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // 创建流
    IStream *pstmp = NULL;
    CreateStreamOnHGlobal(NULL, TRUE, &pstmp);
    if (pstmp == NULL)
    {
        GdiplusShutdown(gdiplusToken);
        gdiplusToken = NULL;
        return false;
    }

    // 初始化流
    LARGE_INTEGER liTemp = { 0 };
    ULARGE_INTEGER uLiZero = { 0 };
    pstmp->Seek(liTemp, STREAM_SEEK_SET, NULL);
    pstmp->SetSize(uLiZero);

    // 将图像放入流中
    ULONG ulRealSize = 0;
    pstmp->Write(pbSrc, iSrcLen, &ulRealSize);

    // 从流创建位图
    Bitmap bmpSrc(pstmp);
    
    Bitmap bmpDst(iDstWidth, iDstHeight, PixelFormat24bppRGB);

    // 创建画图对象
    Graphics grDraw(&bmpDst);

    // 绘图
    //int iDestWidth = bmpSrc.GetWidth();
    //int iDestHeight = bmpSrc.GetHeight();
    //grDraw.DrawImage(&bmpSrc, 0, 0, bmpSrc.GetWidth(), bmpSrc.GetHeight());
    Rect destRect(0, 0, iDstWidth, iDstHeight);
    grDraw.DrawImage(&bmpSrc, destRect);
    if (Ok != grDraw.GetLastStatus())
    {
        pstmp->Release();
        pstmp = NULL;
        GdiplusShutdown(gdiplusToken);
        gdiplusToken = NULL;
        return false;
    }

    // 创建输出流
    IStream* pStreamOut = NULL;
    if (CreateStreamOnHGlobal(NULL, TRUE, &pStreamOut) != S_OK)
    {
        pstmp->Release();
        pstmp = NULL;
        GdiplusShutdown(gdiplusToken);
        gdiplusToken = NULL;
        return false;
    }

    CLSID jpgClsid;
    Tool_GetEncoderClsid(L"image/jpeg", &jpgClsid);

    // 初始化输出流
    pStreamOut->Seek(liTemp, STREAM_SEEK_SET, NULL);
    pStreamOut->SetSize(uLiZero);

    // 将位图按照JPG的格式保存到输出流中
    int iQuality = compressQuality % 100;
    EncoderParameters encoderParameters;
    encoderParameters.Count = 1;
    encoderParameters.Parameter[0].Guid = EncoderQuality;
    encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
    encoderParameters.Parameter[0].NumberOfValues = 1;
    encoderParameters.Parameter[0].Value = &iQuality;
    bmpDst.Save(pStreamOut, &jpgClsid, &encoderParameters);
    //bmpDst.Save(L"./dest1.jpg", &jpgClsid, &encoderParameters);
    //bmpSrc.Save(L"./src.jpg", &jpgClsid, &encoderParameters);

    // 获取输出流大小
    bool bRet = false;
    ULARGE_INTEGER libNewPos = { 0 };
    pStreamOut->Seek(liTemp, STREAM_SEEK_END, &libNewPos);      // 将流指针指向结束位置，从而获取流的大小 
    if (*iDstLen < (int)libNewPos.LowPart)                     // 用户分配的缓冲区不足
    {
        *iDstLen = libNewPos.LowPart;
        bRet = false;
    }
    else
    {
        pStreamOut->Seek(liTemp, STREAM_SEEK_SET, NULL);                   // 将流指针指向开始位置
        pStreamOut->Read(pbDst, libNewPos.LowPart, &ulRealSize);           // 将转换后的JPG图片拷贝给用户
        *iDstLen = ulRealSize;
        bRet = true;
    }


    // 释放内存
    if (pstmp != NULL)
    {
        pstmp->Release();
        pstmp = NULL;
    }
    if (pStreamOut != NULL)
    {
        pStreamOut->Release();
        pStreamOut = NULL;
    }

    GdiplusShutdown(gdiplusToken);
    gdiplusToken = NULL;

    return bRet;
}

bool Tool_Img_compress(PBYTE pbSrc, int iSrcLen, PBYTE pbDst, size_t *iDstLen, int iDstWidth, int iDstHeight, int wishSize)
{
    if (pbSrc == NULL || pbDst == NULL
        || iSrcLen <= 0 || *iDstLen <= 0
        || *iDstLen < (size_t)wishSize)
    {
        printf("Tool_Img_compress, pbSrc == NULL || pbDst == NULL || iSrcLen <= 0 || *iDstLen <= 0, the parameter is invalid.\n");
        return false;
    }
    if (iSrcLen <= wishSize)
    {
        printf("Tool_Img_compress, the size is already meet the requirements, copy to buffer directly.\n");
        memcpy(pbDst, pbSrc, iSrcLen);
        *iDstLen = iSrcLen;
        return true;
    }
    const int iMaxImgSize = 20 * 1024 * 1024;
    PBYTE pBuffer = new BYTE[iMaxImgSize];
    if (!pBuffer)
    {
        printf("Tool_Img_compress, the buffer is allocate failed.\n");
        return false;
    }

    size_t iBufSize = iMaxImgSize;
    int iCompressQuality = 65;
    memset(pBuffer, 0, iMaxImgSize);
    while (iCompressQuality > 0 && iBufSize >= (size_t)wishSize)
    {
        iBufSize = iMaxImgSize;
        memset(pBuffer, 0, iMaxImgSize);
        Tool_Img_ScaleJpg(pbSrc, iSrcLen, pBuffer, &iBufSize, iDstWidth, iDstHeight, iCompressQuality);
        iCompressQuality -= 15;
    }
    bool bRet = false;
    if (iBufSize > (size_t)wishSize)
    {
        printf("after compress , the size is still larger than wish size.\n");
        *iDstLen = iBufSize;
    }
    else
    {
        printf("after compress , the final size = %lu.\n", iBufSize);
        memcpy(pbDst, pBuffer, iBufSize);
        *iDstLen = iBufSize;
        bRet = true;
    }
    if (pBuffer)
    {
        delete[] pBuffer;
        pBuffer = NULL;
    }
    return bRet;
}

int Tool_GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
    UINT  num = 0;          // number of image encoders
    UINT  size = 0;         // size of the image encoder array in bytes

    ImageCodecInfo* pImageCodecInfo = NULL;

    GetImageEncodersSize(&num, &size);
    if (size == 0)
        return -1;  // Failure

    pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
    if (pImageCodecInfo == NULL)
        return -1;  // Failure

    GetImageEncoders(num, size, pImageCodecInfo);

    for (UINT j = 0; j < num; ++j)
    {
        if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
        {
            *pClsid = pImageCodecInfo[j].Clsid;
            free(pImageCodecInfo);
            return j;  // Success
        }
    }
    free(pImageCodecInfo);
    return -1;  // Failure
}

void Tool_ExcuteShellCMD(char* pChCommand)
{
    if (NULL == pChCommand)
    {
        return;
    }
    ShellExecute(NULL, "open", "C:\\WINDOWS\\system32\\cmd.exe", pChCommand, "", SW_HIDE);
}

bool Tool_OverlayStringToImg(unsigned char** pImgsrc, long srcSize,
    unsigned char** pImgDest, long& DestSize,
    const wchar_t* DestString, int FontSize,
    int x, int y, int colorR, int colorG, int colorB,
    int compressQuality)
{
    if (!pImgsrc || !pImgDest || srcSize <= 0 || DestSize <= 0)
    {
        //WriteLog("传入参数为非法值");
        return false;
    }
    if (wcslen(DestString) <= 0 || x < 0 || y < 0)
    {
        //WriteLog("字符串长度为0");
        return false;
    }

    //构造图像	
    IStream *pSrcStream = NULL;
    IStream *pDestStream = NULL;
    CreateStreamOnHGlobal(NULL, TRUE, &pSrcStream);
    CreateStreamOnHGlobal(NULL, TRUE, &pDestStream);
    if (!pSrcStream || !pDestStream)
    {
        //WriteLog("流创建失败.");
        return false;
    }
    LARGE_INTEGER liTemp = { 0 };
    pSrcStream->Seek(liTemp, STREAM_SEEK_SET, NULL);
    pSrcStream->Write(*pImgsrc, srcSize, NULL);
    Bitmap bmp(pSrcStream);
    int iImgWith = bmp.GetWidth();
    int iImgHeight = bmp.GetHeight();

    Graphics grp(&bmp);

    SolidBrush brush(Color(colorR, colorG, colorB));
    FontFamily fontFamily(L"宋体");
    //Gdiplus::Font font(&fontFamily, (REAL)FontSize);
    Gdiplus::Font font(&fontFamily, (REAL)FontSize, FontStyleRegular, UnitPixel);

    RectF layoutRect((float)x, (float)y, (float)(iImgWith - x), 0.0);
    RectF FinalRect;
    INT codePointsFitted = 0;
    INT linesFitted = 0;
    int strLenth = wcslen(DestString);
    grp.MeasureString(DestString, strLenth, &font, layoutRect, NULL, &FinalRect, &codePointsFitted, &linesFitted);
    grp.DrawString(DestString, -1, &font, FinalRect, NULL, &brush);
    Gdiplus::Status iState = grp.GetLastStatus();
    if (iState == Ok)
    {
        //WriteLog("字符叠加成功");
    }
    else
    {
        //char chLog[260] = { 0 };
        //sprintf(chLog, "字符叠加失败， 错误码为%d", iState);
        //WriteLog(chLog);
    }

    pSrcStream->Seek(liTemp, STREAM_SEEK_SET, NULL);
    pDestStream->Seek(liTemp, STREAM_SEEK_SET, NULL);

    // 将位图按照JPG的格式保存到输出流中
    CLSID jpgClsid;
    Tool_GetEncoderClsid(L"image/jpeg", &jpgClsid);
    int iQuality = compressQuality;
    EncoderParameters encoderParameters;
    encoderParameters.Count = 1;
    encoderParameters.Parameter[0].Guid = EncoderQuality;
    encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
    encoderParameters.Parameter[0].NumberOfValues = 1;
    encoderParameters.Parameter[0].Value = &iQuality;
    bmp.Save(pDestStream, &jpgClsid, &encoderParameters);

    ULARGE_INTEGER uiSize;
    pDestStream->Seek(liTemp, STREAM_SEEK_CUR, &uiSize);
    long iFinalSize = (long)uiSize.QuadPart;
    if (iFinalSize <= DestSize)
    {
        pDestStream->Seek(liTemp, STREAM_SEEK_SET, NULL);
        pDestStream->Read(*pImgDest, iFinalSize, NULL);
        DestSize = iFinalSize;
    }
    else
    {
        DestSize = 0;
        if (pSrcStream)
        {
            pSrcStream->Release();
            pSrcStream = NULL;
        }
        if (pDestStream)
        {
            pDestStream->Release();
            pDestStream = NULL;
        }
        //WriteLog("传入空间不足，字符叠加失败");
        return false;
    }

    if (pSrcStream)
    {
        pSrcStream->Release();
        pSrcStream = NULL;
    }
    if (pDestStream)
    {
        pDestStream->Release();
        pDestStream = NULL;
    }
    return true;
}

bool GetDataFromAppenedInfo(char *pszAppendInfo, std::string strItemName, char *pszRstBuf, int *piRstBufLen)
{
    if (pszAppendInfo == NULL || piRstBufLen == NULL || *piRstBufLen <= 0)
    {
        return false;
    }

    // <RoadNumber value="0" chnname="车道" />
    // <StreetName value="" chnname="路口名称" />
    std::string strAppendInfo = pszAppendInfo;
    size_t siStart = strAppendInfo.find(strItemName);
    if (siStart == std::string::npos)
    {
        return false;
    }
    siStart = strAppendInfo.find("\"", siStart + 1);
    if (siStart == std::string::npos)
    {
        return false;
    }
    size_t siEnd = strAppendInfo.find("\"", siStart + 1);
    if (siEnd == std::string::npos)
    {
        return false;
    }

    std::string strRst = strAppendInfo.substr(siStart + 1, siEnd - siStart - 1);
    if (*piRstBufLen < (int)strRst.length())
    {
        *piRstBufLen = (int)strRst.length();
        return false;
    }

    strncpy_s(pszRstBuf, *piRstBufLen, strRst.c_str(), (int)strRst.length());
    *piRstBufLen = (int)strRst.length();
    return true;
}

void ExcuteCMD(char* pChCommand)
{
#ifdef WIN32

    if (NULL == pChCommand)
    {
        return;
    }
    ShellExecute(NULL, "open", "C:\\WINDOWS\\system32\\cmd.exe", pChCommand, "", SW_HIDE);

#endif // WIN32
}

std::wstring Img_string2wstring(std::string strSrc)
{
    std::wstring wstrDst;
    int iWstrLen = MultiByteToWideChar(CP_ACP, 0, strSrc.c_str(), strSrc.size(), NULL, 0);
    wchar_t* pwcharBuf = new wchar_t[iWstrLen + sizeof(wchar_t)];   // 多一个结束符
    if (pwcharBuf == NULL || iWstrLen <= 0)
    {
        return L"";
    }
    memset(pwcharBuf, 0, iWstrLen*sizeof(wchar_t)+sizeof(wchar_t));
    MultiByteToWideChar(CP_ACP, 0, strSrc.c_str(), strSrc.size(), pwcharBuf, iWstrLen);
    pwcharBuf[iWstrLen] = L'\0';
    wstrDst.append(pwcharBuf);
    delete[] pwcharBuf;
    pwcharBuf = NULL;
    return wstrDst;
}

#ifdef USE_MFC
bool DeleteDirectory(char* strDirName)
{
    CFileFind tempFind;

    char strTempFileFind[MAX_PATH];

    MY_SPRINTF(strTempFileFind, sizeof(strTempFileFind), "%s//*.*", strDirName);

    BOOL IsFinded = tempFind.FindFile(strTempFileFind);

    while (IsFinded)
    {
        IsFinded = tempFind.FindNextFile();

        if (!tempFind.IsDots())
        {
            char strFoundFileName[MAX_PATH];

            //strcpy(strFoundFileName, tempFind.GetFileName().GetBuffer(MAX_PATH));
            strcpy_s(strFoundFileName, tempFind.GetFileName().GetBuffer(MAX_PATH));

            if (tempFind.IsDirectory())
            {
                char strTempDir[MAX_PATH];

                MY_SPRINTF(strTempDir, sizeof(strTempDir), "%s//%s", strDirName, strFoundFileName);

                DeleteDirectory(strTempDir);
            }
            else
            {
                char strTempFileName[MAX_PATH];

                MY_SPRINTF(strTempFileName, sizeof(strTempFileName), "%s//%s", strDirName, strFoundFileName);

                DeleteFile(strTempFileName);
            }
        }
    }

    tempFind.Close();

    if (!RemoveDirectory(strDirName))
    {
        return FALSE;
    }

    return TRUE;
}

int CirclelaryDelete(char* folderPath, int iBackUpDays)
{
    printf("进入环覆盖线程主函数,开始查找制定目录下的文件夹");
    char myPath[MAX_PATH] = { 0 };
    //sprintf(myPath, "%s\\*", folderPath);
    MY_SPRINTF(myPath,sizeof(myPath),  "%s\\*", folderPath);

    CTime tmCurrentTime = CTime::GetCurrentTime();
    CTime tmLastMonthTime = tmCurrentTime - CTimeSpan(iBackUpDays, 0, 0, 0);
    int Last_Year = tmLastMonthTime.GetYear();
    int Last_Month = tmLastMonthTime.GetMonth();
    int Last_Day = tmLastMonthTime.GetDay();
    //cout<<Last_Year<<"-"<<Last_Month<<"-"<<Last_Day<<endl;

    CFileFind myFileFind;
    BOOL bFinded = myFileFind.FindFile(myPath);
    char DirectoryName[MAX_PATH] = { 0 };
    while (bFinded)
    {
        bFinded = myFileFind.FindNextFileA();
        if (!myFileFind.IsDots())
        {
            MY_SPRINTF(DirectoryName, sizeof(DirectoryName), "%s", myFileFind.GetFileName().GetBuffer());
            if (myFileFind.IsDirectory())
            {
                int iYear, iMonth, iDay;
                iYear = iMonth = iDay = 0;
                //sscanf(DirectoryName,"%d-%d-%d",&iYear, &iMonth, &iDay);
                sscanf_s(DirectoryName, "%d-%d-%d", &iYear, &iMonth, &iDay);
                if (iYear == 0 && iMonth ==0 && iDay == 0)
                {
                    continue;
                }
                if (iYear < Last_Year)
                {
                    MY_SPRINTF(DirectoryName,sizeof(DirectoryName), "%s\\%s", folderPath, myFileFind.GetFileName().GetBuffer());
                    printf("delete the DirectoryB :%s\n", DirectoryName);
                    DeleteDirectory(DirectoryName);

                    char chLog[MAX_PATH] = { 0 };
                    MY_SPRINTF(chLog,sizeof(chLog), "年份小于当前年份，删除文件夹%s", DirectoryName);
                    printf(chLog);
                }
                else if (iYear == Last_Year)
                {
                    if (iMonth < Last_Month)
                    {
                        MY_SPRINTF(DirectoryName, sizeof(DirectoryName), "%s\\%s", folderPath, myFileFind.GetFileName().GetBuffer());
                        printf("delete the DirectoryB :%s\n", DirectoryName);
                        DeleteDirectory(DirectoryName);

                        char chLog[MAX_PATH] = { 0 };
                        MY_SPRINTF(chLog,sizeof(chLog), "月份小于上一月，删除文件夹%s", DirectoryName);
                        printf(chLog);
                    }
                    else if (iMonth == Last_Month)
                    {
                        if (iDay < Last_Day)
                        {
                            MY_SPRINTF(DirectoryName, sizeof(DirectoryName), "%s\\%s", folderPath, myFileFind.GetFileName().GetBuffer());
                            printf("delete the DirectoryB :%s\n", DirectoryName);
                            DeleteDirectory(DirectoryName);

                            char chLog[MAX_PATH] = { 0 };
                            MY_SPRINTF(chLog, sizeof(chLog), "日号小于指定天数，删除文件夹%s", DirectoryName);
                            printf(chLog);
                        }
                    }
                }
            }
        }
    }
    myFileFind.Close();
    printf("查询结束，退出环覆盖线程主函数..");
    return 0;
}
#endif
bool Tool_SaveFileToDisk(char* chImgPath, void* pImgData, DWORD dwImgSize)
{
    printf("begin SaveImgToDisk");
    if (NULL == pImgData)
    {
        printf("end1 SaveImgToDisk");
        return false;
    }
    bool bRet = false;
    size_t iWritedSpecialSize = 0;
    std::string tempFile(chImgPath);
    size_t iPosition = tempFile.rfind("\\");
    std::string tempDir = tempFile.substr(0, iPosition + 1);
    if (MakeSureDirectoryPathExists(tempDir.c_str()))
    {
        FILE* fp = NULL;
        //fp = fopen(chImgPath, "wb+");
        fopen_s(&fp, chImgPath, "wb+");
        if (fp)
        {
            //iWritedSpecialSize = fwrite(pImgData, dwImgSize , 1, fp);
            iWritedSpecialSize = fwrite(pImgData, 1, dwImgSize, fp);
            fclose(fp);
            fp = NULL;
            bRet = true;
        }
        if (iWritedSpecialSize == dwImgSize)
        {
            char chLogBuff[MAX_PATH] = { 0 };
            //sprintf_s(chLogBuff, "%s save success", chImgPath);
            sprintf_s(chLogBuff, "%s save success", chImgPath);
            printf(chLogBuff);
        }
    }
    else
    {
        char chLogBuff[MAX_PATH] = { 0 };
        //sprintf_s(chLogBuff, "%s save failed", chImgPath);
        sprintf_s(chLogBuff, "%s save failed", chImgPath);
        printf(chLogBuff);
        bRet = false;
    }
    printf("end SaveImgToDisk");
    return bRet;
}

#ifdef WIN32
std::string GetSoftVersion(const char* exepath)
{
    std::string strVersionInfo = "";
    if (!exepath)
        return strVersionInfo;
    if (_access(exepath, 0) != 0)
        return strVersionInfo;
    UINT infoSize = GetFileVersionInfoSize(exepath, 0);
    if (infoSize != 0) {
        strVersionInfo.resize(infoSize, 0);
        char *pBuf = NULL;
        pBuf = new char[infoSize];
        VS_FIXEDFILEINFO *pVsInfo;
        if (GetFileVersionInfo(exepath, 0, infoSize, pBuf)) {
            if (VerQueryValue(pBuf, "\\", (void **)&pVsInfo, &infoSize))
            {
                //sprintf(pBuf, "%d.%d.%d.%d", HIWORD(pVsInfo->dwFileVersionMS), LOWORD(pVsInfo->dwFileVersionMS), HIWORD(pVsInfo->dwFileVersionLS), LOWORD(pVsInfo->dwFileVersionLS));
                sprintf_s(pBuf, infoSize,"%d.%d.%d.%d", HIWORD(pVsInfo->dwFileVersionMS), LOWORD(pVsInfo->dwFileVersionMS), HIWORD(pVsInfo->dwFileVersionLS), LOWORD(pVsInfo->dwFileVersionLS));
                strVersionInfo = pBuf;
            }
        }
        delete[] pBuf;
    }
    return strVersionInfo;
}

#endif

int DrawStringToImg(const ImgDataStruct dataStruct, const OverlayInfo overlayInfo, void* destImgBuffer, size_t& destBufferSize)
{
#define POS_UP_TO_IMG (-1)
#define POS_DOWN_TO_IMG (-2)

#define STYLE_ONLY_FONT_BACKGROUND (1)
#define STYLE_WHOLE_LINE (2)

    if (!dataStruct.srcImgData || dataStruct.srcImgDataLengh <= 0 || !destImgBuffer || destBufferSize <= 0)
    {
        printf("myDrawString, the parameter is invalid, return 1.\n ");
        return 1;
    }
    int iRet = 0;

    UCHAR* pSrcData = dataStruct.srcImgData;
    long srcLength = dataStruct.srcImgDataLengh;

    // 创建流
    IStream *pStreamSrc = NULL;
    // 创建输出流
    IStream* pStreamOut = NULL;
    LARGE_INTEGER liTemp = { 0 };
    ULARGE_INTEGER uLiZero = { 0 };

    HRESULT hr1 = CreateStreamOnHGlobal(NULL, TRUE, &pStreamSrc);
    HRESULT hr2 = CreateStreamOnHGlobal(NULL, TRUE, &pStreamOut);
    if (hr1 != S_OK || hr2 != S_OK)
    {
        printf("CreateStreamOnHGlobal failed, return 2.\n ");
        iRet = 2;

        if (pStreamOut != NULL)
        {
            pStreamOut->Release();
            pStreamOut = NULL;
        }

        if (pStreamSrc != NULL)
        {
            pStreamSrc->Release();
            pStreamSrc = NULL;
        }
        return 2;
    }

    // 初始化流
    pStreamSrc->Seek(liTemp, STREAM_SEEK_SET, NULL);
    pStreamSrc->SetSize(uLiZero);

    pStreamOut->Seek(liTemp, STREAM_SEEK_SET, NULL);
    pStreamOut->SetSize(uLiZero);

    // 将图像放入流中
    ULONG ulRealSize = 0;
    if (S_OK != pStreamSrc->Write(pSrcData, srcLength, &ulRealSize))
    {
        printf("pStreamSrc failed.\n");
        iRet = 3;

        if (pStreamOut != NULL)
        {
            pStreamOut->Release();
            pStreamOut = NULL;
        }

        if (pStreamSrc != NULL)
        {
            pStreamSrc->Release();
            pStreamSrc = NULL;
        }
        return 3;
    }

    // 从流创建位图
    Bitmap bmpSrc(pStreamSrc);
    //CLSID clsidEncoder;
    //Tool_GetEncoderClsid(L"image/bmp", &clsidEncoder);
    //bmpSrc.Save(L"./3.bmp", &clsidEncoder);
    int iSrcWidth = bmpSrc.GetWidth();
    int iSrcHeight = bmpSrc.GetHeight();

    Gdiplus::Color fontColor(overlayInfo.st_fontColor.iColorAlpha, overlayInfo.st_fontColor.iColorR, overlayInfo.st_fontColor.iColorG, overlayInfo.st_fontColor.iColorB);
    Gdiplus::SolidBrush  fontBrush(fontColor);
    Gdiplus::FontFamily  fontFamily(L"Times New Roman");
    Gdiplus::Font        font(&fontFamily, (float)(overlayInfo.iFontSize), FontStyleRegular, UnitPixel);

    Gdiplus::RectF  rectfOut;
    {
        //计算消息主题的高度
        Gdiplus::Bitmap bgtest(iSrcWidth, iSrcHeight);
        Gdiplus::Graphics    graphicsTest(&bgtest);
        Gdiplus::RectF rtGdiplus;//计算消息主题的宽度
        rtGdiplus.X = (overlayInfo.st_FontPosition.iPosX > 0) ? overlayInfo.st_FontPosition.iPosX : 0;
        rtGdiplus.Y = (overlayInfo.st_FontPosition.iPosY > 0) ? overlayInfo.st_FontPosition.iPosY : 0;
        rtGdiplus.Width = (float)iSrcWidth;
        rtGdiplus.Height = -1;
        graphicsTest.MeasureString(overlayInfo.szOverlayString, -1, &font, rtGdiplus, &rectfOut);
        printf("MeasureString width = %f, height = %f\n", rectfOut.Width, rectfOut.Height);
    }
    int iDestWidth = 0.0, iDestHeight = 0.0;
    if (overlayInfo.st_FontPosition.iPosY == POS_UP_TO_IMG   \
        || overlayInfo.st_FontPosition.iPosY == POS_DOWN_TO_IMG
        )
    {
        iDestWidth = iSrcWidth;
        iDestHeight = (iSrcHeight + rectfOut.Height);
    }
    else
    {
        iDestWidth =iSrcWidth;
        iDestHeight = iSrcHeight;
    }
    Gdiplus::Bitmap bmpDst(iDestWidth, iDestHeight);
    Gdiplus::Graphics    graphics(&bmpDst);

    //先画图，再画矩形
    Gdiplus::Status rSata = Gdiplus::Ok;
    Gdiplus::Rect destRect;
    destRect.X =0;
    destRect.Y = (overlayInfo.st_FontPosition.iPosY == POS_UP_TO_IMG) ? rectfOut.Height : 0;
    destRect.Width = iSrcWidth;
    destRect.Height = iSrcHeight;
    rSata = graphics.DrawImage(&bmpSrc, destRect);
    if (rSata != Gdiplus::Ok)
    {
        printf("draw image failed.\n");
        iRet = 4;

        if (pStreamOut != NULL)
        {
            pStreamOut->Release();
            pStreamOut = NULL;
        }

        if (pStreamSrc != NULL)
        {
            pStreamSrc->Release();
            pStreamSrc = NULL;
        }
        return 4;
    }

    //绘制矩形填充区
    
    switch (overlayInfo.st_FontPosition.iPosY)
    {
    case POS_UP_TO_IMG:        //矩形绘制到图片上方之外
        destRect.X = 0;
        destRect.Y = 0;
        break;
    case POS_DOWN_TO_IMG:        //矩形绘制到图片下方之外
        destRect.X = 0;
        destRect.Y = iSrcHeight;
        break;
    default:       //图片之内
        destRect.X = (overlayInfo.st_FontPosition.iPosX >= 0) ? overlayInfo.st_FontPosition.iPosX : 0;
        destRect.Y = (overlayInfo.st_FontPosition.iPosY < 0) ? 0 : overlayInfo.st_FontPosition.iPosY;
        break;
    }

    //矩形宽度决策
    switch (overlayInfo.iStyle)
    {
    case STYLE_ONLY_FONT_BACKGROUND:        //
        destRect.Width = rectfOut.Width;
        break;
    case STYLE_WHOLE_LINE:        //矩形绘制到整行        
        destRect.Width = iSrcWidth;
        break;
    default:       //图片之内
        destRect.Width = iSrcWidth;
        break;
    }

    destRect.Height = rectfOut.Height;
    Gdiplus::SolidBrush myBrush(Gdiplus::Color(overlayInfo.st_backgroundColor.iColorAlpha, overlayInfo.st_backgroundColor.iColorR, overlayInfo.st_backgroundColor.iColorG, overlayInfo.st_backgroundColor.iColorB));
    rSata = graphics.FillRectangle(&myBrush, destRect);  //绘制矩形背景颜色
    if (rSata != Gdiplus::Ok)
    {
        printf("draw Rectangle failed.\n");
        iRet = 5;

        if (pStreamOut != NULL)
        {
            pStreamOut->Release();
            pStreamOut = NULL;
        }

        if (pStreamSrc != NULL)
        {
            pStreamSrc->Release();
            pStreamSrc = NULL;
        }
        return 5;
    }

    Gdiplus::RectF rectString;
    rectString.X = overlayInfo.st_FontPosition.iPosX;
    rectString.Y = destRect.Y;
    rectString.Width = rectfOut.Width;
    rectString.Height = rectfOut.Height;
//#ifdef DEBUG
//    graphics.DrawRectangle(&myPen, rectString);
//#endif

    rSata = graphics.DrawString(overlayInfo.szOverlayString, -1, &font, rectString, NULL, &fontBrush);
    if (rSata != Gdiplus::Ok)
    {
        printf("draw string failed.\n");
        iRet = 6;

        if (pStreamOut != NULL)
        {
            pStreamOut->Release();
            pStreamOut = NULL;
        }

        if (pStreamSrc != NULL)
        {
            pStreamSrc->Release();
            pStreamSrc = NULL;
        }
        return 6;
    }
    printf("DrawString status = %d\n", rSata);

    CLSID jpgClsid;
    CLSID bmpClsid;
    Tool_GetEncoderClsid(L"image/jpeg", &jpgClsid);
    Tool_GetEncoderClsid(L"image/bmp", &bmpClsid);

    // 将位图按照JPG的格式保存到输出流中
    //int iQuality = 80 % 100;
    //EncoderParameters encoderParameters;
    //encoderParameters.Count = 1;
    //encoderParameters.Parameter[0].Guid = EncoderQuality;
    //encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
    //encoderParameters.Parameter[0].NumberOfValues = 1;
    //encoderParameters.Parameter[0].Value = &iQuality;
    //bmpDst.Save(pStreamOut, &jpgClsid, &encoderParameters);
    rSata = bmpDst.Save(pStreamOut, &bmpClsid, NULL);
    if (rSata != Gdiplus::Ok)
    {
        printf("save to stream out failed.\n");
        iRet = 7;

        if (pStreamOut != NULL)
        {
            pStreamOut->Release();
            pStreamOut = NULL;
        }

        if (pStreamSrc != NULL)
        {
            pStreamSrc->Release();
            pStreamSrc = NULL;
        }
        return 7;
    }

    // 获取输出流大小
    ULARGE_INTEGER libNewPos = { 0 };
    pStreamOut->Seek(liTemp, STREAM_SEEK_END, &libNewPos);      // 将流指针指向结束位置，从而获取流的大小 
    if (destBufferSize < (int)libNewPos.LowPart)                     // 用户分配的缓冲区不足
    {
        destBufferSize = libNewPos.LowPart;
        iRet = 8;
        printf("the buffer size is not enough.\n");

        if (pStreamOut != NULL)
        {
            pStreamOut->Release();
            pStreamOut = NULL;
        }

        if (pStreamSrc != NULL)
        {
            pStreamSrc->Release();
            pStreamSrc = NULL;
        }
        return 8;
    }
    else
    {
        pStreamOut->Seek(liTemp, STREAM_SEEK_SET, NULL);                   // 将流指针指向开始位置
        pStreamOut->Read(destImgBuffer, libNewPos.LowPart, &ulRealSize);           // 将转换后的JPG图片拷贝给用户
        destBufferSize = ulRealSize;
        iRet = 0;
    }

    // 释放内存
    if (pStreamOut != NULL)
    {
        pStreamOut->Release();
        pStreamOut = NULL;
    }

    if (pStreamSrc != NULL)
    {
        pStreamSrc->Release();
        pStreamSrc = NULL;
    }

    return iRet;
}

const int BIN_BIT_COUNT = 8;
const int BIN_WIDTH = 112;
const int BIN_HEIGHT = 20;
const int BIN_STRIDE = BIN_WIDTH / BIN_BIT_COUNT;
const int BIN_BYTE_COUNT = BIN_HEIGHT * BIN_STRIDE;
void Tool_RgbBin2ByteBin(PBYTE m_rgbBinImage, PBYTE pbByteBinImage)
{
    for (int y = 0; y < BIN_HEIGHT; y++)
    {
        BYTE *pSrc = &m_rgbBinImage[(BIN_HEIGHT - 1) * BIN_STRIDE];
        BYTE *pDest = pbByteBinImage;
        for (int y = 0; y < BIN_HEIGHT; y++)
        {
            memcpy(pDest, pSrc, BIN_STRIDE);
            pSrc -= BIN_STRIDE;
            pDest += BIN_STRIDE;
        }
        //字节中前后bit位互换
        for (int i = 0; i < BIN_BYTE_COUNT; i++)
        {
            BYTE bTemp = pbByteBinImage[i];
            BYTE bTemp2 = 0;
            bTemp2 |= (bTemp & 0x80) >> 7;
            bTemp2 |= (bTemp & 0x40) >> 5;
            bTemp2 |= (bTemp & 0x20) >> 3;
            bTemp2 |= (bTemp & 0x10) >> 1;
            bTemp2 |= (bTemp & 0x08) << 1;
            bTemp2 |= (bTemp & 0x04) << 3;
            bTemp2 |= (bTemp & 0x02) << 5;
            bTemp2 |= (bTemp & 0x01) << 7;
            pbByteBinImage[i] = bTemp2;
        }
    }
}

BOOL Tool_BinImage2BitmapData(int iCX, int iCY, BYTE* pbByteBinImage, char* pbBitmapData, int* piBitmapDataLen)
{
    BOOL bSuccess = TRUE;
    int iByteBinImageLen = BIN_BYTE_COUNT;
    if (iCX != BIN_WIDTH || iCY != BIN_HEIGHT || pbByteBinImage == NULL || piBitmapDataLen == NULL)
    {
        bSuccess = FALSE;
    }
    else
    {
        int iStride = ((BIN_STRIDE + 3) >> 2) << 2;
        int iBmpDataSize = iStride * BIN_HEIGHT;

        BITMAPFILEHEADER bmfHdr;
        bmfHdr.bfType = 0x4d42;
        bmfHdr.bfSize = iBmpDataSize + sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+2 * sizeof(RGBQUAD);
        bmfHdr.bfReserved1 = 0;
        bmfHdr.bfReserved2 = 0;
        bmfHdr.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+2 * sizeof(RGBQUAD);

        BITMAPINFO *pInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFOHEADER)+2 * sizeof(RGBQUAD));
        pInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        pInfo->bmiHeader.biWidth = BIN_WIDTH;
        pInfo->bmiHeader.biHeight = BIN_HEIGHT;
        pInfo->bmiHeader.biPlanes = 1;
        pInfo->bmiHeader.biBitCount = 1;
        pInfo->bmiHeader.biCompression = BI_RGB;
        pInfo->bmiHeader.biSizeImage = 0;
        pInfo->bmiHeader.biXPelsPerMeter = 0;
        pInfo->bmiHeader.biYPelsPerMeter = 0;
        pInfo->bmiHeader.biClrUsed = 0;
        pInfo->bmiHeader.biClrImportant = 0;

        pInfo->bmiColors[0].rgbBlue = 0;
        pInfo->bmiColors[0].rgbGreen = 0;
        pInfo->bmiColors[0].rgbRed = 0;
        pInfo->bmiColors[0].rgbReserved = 0;
        pInfo->bmiColors[1].rgbBlue = 255;
        pInfo->bmiColors[1].rgbGreen = 255;
        pInfo->bmiColors[1].rgbRed = 255;
        pInfo->bmiColors[1].rgbReserved = 0;

        BYTE *pBmpData = (BYTE*)malloc(iBmpDataSize);
        memset(pBmpData, 0, iBmpDataSize);
        BYTE *pSrc = pbByteBinImage;
        BYTE *pDest = pBmpData;
        for (int i = 0; i < BIN_HEIGHT; i++)
        {
            memcpy(pDest, pSrc, BIN_STRIDE);
            pDest += iStride;
            pSrc += BIN_STRIDE;
        }

        memcpy(pbBitmapData, &bmfHdr, sizeof(BITMAPFILEHEADER));
        pbBitmapData += sizeof(BITMAPFILEHEADER);
        *piBitmapDataLen += sizeof(BITMAPFILEHEADER);

        memcpy(pbBitmapData, pInfo, sizeof(BITMAPINFOHEADER)+2 * sizeof(RGBQUAD));
        pbBitmapData += sizeof(BITMAPINFOHEADER)+2 * sizeof(RGBQUAD);
        *piBitmapDataLen += sizeof(BITMAPINFOHEADER)+2 * sizeof(RGBQUAD);

        memcpy(pbBitmapData, pBmpData, iBmpDataSize);
        pbBitmapData += iBmpDataSize;
        *piBitmapDataLen += iBmpDataSize;

        free(pBmpData);
        free(pInfo);
    }

    return bSuccess;
}

void Tool_Bin2BMP(PBYTE pbBinData, PBYTE pbBmpData, INT& nBmpLen)
{
    PBYTE pbByteBinImage[BIN_BYTE_COUNT] = { 0 };
    Tool_RgbBin2ByteBin(pbBinData, (PBYTE)pbByteBinImage);

    int iBitmapDataLen = 0;
    Tool_BinImage2BitmapData(BIN_WIDTH, BIN_HEIGHT, (PBYTE)pbByteBinImage, (char*)pbBmpData, &iBitmapDataLen);
    nBmpLen = iBitmapDataLen;
}

int DrawHeadStyleString(void* srcImgData, size_t srcLength, void* destImgData, size_t& destLength, const char* overlayString, int posX, int posY)
{
    if (NULL == srcImgData
        || NULL == destImgData
        || srcLength <= 0
        || destLength <= 0
        || NULL == overlayString
        )
    {
        return false;
    }
    ImgDataStruct dataStruct;
    dataStruct.srcImgData = (unsigned char*)srcImgData;
    dataStruct.srcImgDataLengh = srcLength;

    OverlayInfo overlayInfo1;
    overlayInfo1.st_backgroundColor.iColorAlpha = 55;
    overlayInfo1.st_backgroundColor.iColorR = 0;
    overlayInfo1.st_backgroundColor.iColorG = 0;
    overlayInfo1.st_backgroundColor.iColorB = 0;

    overlayInfo1.iFontSize = 32;
    overlayInfo1.st_fontColor.iColorAlpha = 255;
    overlayInfo1.st_fontColor.iColorR = 255;
    overlayInfo1.st_fontColor.iColorG = 255;
    overlayInfo1.st_fontColor.iColorB = 255;

    overlayInfo1.st_FontPosition.iPosX = posX;
    overlayInfo1.st_FontPosition.iPosY = posY;

    std::wstring wstrOverlayInfo = Img_string2wstring(overlayString);
    overlayInfo1.szOverlayString = wstrOverlayInfo.c_str();
    overlayInfo1.iStyle = 1;

    return DrawStringToImg(dataStruct, overlayInfo1, destImgData, destLength);
}

int DrawEnd1String(void* srcImgData, size_t srcLength, void* destImgData, size_t& destLength, const char* overlayString, int posX, int posY)
{
    if (NULL == srcImgData
        || NULL == destImgData
        || srcLength <= 0
        || destLength <= 0
        || NULL == overlayString
        )
    {
        return false;
    }
    ImgDataStruct dataStruct;
    dataStruct.srcImgData = (unsigned char*)srcImgData;
    dataStruct.srcImgDataLengh = srcLength;

    OverlayInfo overlayInfo1;
    overlayInfo1.st_backgroundColor.iColorAlpha = 55;
    overlayInfo1.st_backgroundColor.iColorR = 0;
    overlayInfo1.st_backgroundColor.iColorG = 0;
    overlayInfo1.st_backgroundColor.iColorB = 0;

    overlayInfo1.iFontSize = 32;
    overlayInfo1.st_fontColor.iColorAlpha = 255;
    overlayInfo1.st_fontColor.iColorR = 255;
    overlayInfo1.st_fontColor.iColorG = 255;
    overlayInfo1.st_fontColor.iColorB = 255;

    overlayInfo1.st_FontPosition.iPosX = posX;
    overlayInfo1.st_FontPosition.iPosY = posY;

    std::wstring wstrOverlayInfo = Img_string2wstring(overlayString);
    overlayInfo1.szOverlayString = wstrOverlayInfo.c_str();
    overlayInfo1.iStyle = 1;

    return DrawStringToImg(dataStruct, overlayInfo1, destImgData, destLength);
}

int DrawEnd2String(void* srcImgData, size_t srcLength, void* destImgData, size_t& destLength,const char* overlayString, int posX, int posY)
{
    if (NULL == srcImgData
        || NULL == destImgData
        || srcLength <= 0
        || destLength <= 0
        || NULL == overlayString
        )
    {
        return false;
    }
    ImgDataStruct dataStruct;
    dataStruct.srcImgData = (unsigned char*)srcImgData;
    dataStruct.srcImgDataLengh = srcLength;

    OverlayInfo overlayInfo1;
    overlayInfo1.st_backgroundColor.iColorAlpha = 255;
    overlayInfo1.st_backgroundColor.iColorR = 11;
    overlayInfo1.st_backgroundColor.iColorG = 113;
    overlayInfo1.st_backgroundColor.iColorB = 177;

    overlayInfo1.iFontSize = 32;
    overlayInfo1.st_fontColor.iColorAlpha = 255;
    overlayInfo1.st_fontColor.iColorR = 255;
    overlayInfo1.st_fontColor.iColorG = 255;
    overlayInfo1.st_fontColor.iColorB = 0;

    overlayInfo1.st_FontPosition.iPosX = posX;
    overlayInfo1.st_FontPosition.iPosY = posY;

    std::wstring wstrOverlayInfo = Img_string2wstring(overlayString);
    overlayInfo1.szOverlayString = wstrOverlayInfo.c_str();

    return DrawStringToImg(dataStruct, overlayInfo1, destImgData, destLength);
}

bool Tool_CalculateStringWithAndHeight(const char* overlayString, const int imageWidth, const int imageHeight, const int fontSize, MyRectf& rectfOut)
{
    if (overlayString == NULL
        || imageWidth <= 0
        || imageHeight <= 0
        || fontSize <= 0
        )
    {
        printf("Tool_CalculateStringWithAndHeight, failed, parameter is invalid, overlayString = %p, imageWidth = %d, imageHeight = %d, fontSize = %d\n",
            overlayString, \
            imageWidth, imageWidth, \
            fontSize);
        return false;
    }
    size_t iSrcWidth = 0, iSrcHeight = 0;
    iSrcWidth = imageWidth;
    iSrcHeight = imageHeight;

    Gdiplus::RectF  TempRectf;
    //计算消息主题的高度
    Gdiplus::Bitmap bgtest(iSrcWidth, iSrcHeight);
    Gdiplus::Graphics    graphicsTest(&bgtest);
    Gdiplus::RectF rtGdiplus;//计算消息主题的宽度
    std::wstring  wstrOverlay = Img_string2wstring(overlayString);
    Gdiplus::FontFamily  fontFamily(L"Times New Roman");
    Gdiplus::Font        font(&fontFamily, fontSize, FontStyleRegular, UnitPixel);

    rtGdiplus.X = 0;
    rtGdiplus.Y = 0;
    rtGdiplus.Width = iSrcWidth;
    rtGdiplus.Height = -1;
    Gdiplus::Status calSta = graphicsTest.MeasureString(wstrOverlay.c_str(), -1, &font, rtGdiplus, &TempRectf);
    printf("calSta =%d,  MeasureString width = %f, height = %f\n", calSta, TempRectf.Width, TempRectf.Height);

    rectfOut.X = TempRectf.X;
    rectfOut.Y = TempRectf.Y;
    rectfOut.Width = TempRectf.Width;
    rectfOut.Height = TempRectf.Height;
    return true;
}
