#pragma once

class Function {

public:
	static CString ApplicationPath() 
	{
		CString sPath;
		GetModuleFileName(NULL, sPath.GetBuffer(MAX_PATH), MAX_PATH);
		PathAppend(sPath.GetBuffer(), "..\\");
		sPath.ReleaseBuffer();
		return sPath;
	}

	static CString Now() 
	{
		SYSTEMTIME time;
		GetLocalTime(&time);
		CString str;
		str.Format("%4d-%02d-%02d %02d:%02d:%02d",time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond);
		return str;
	}

	static CString Date() 
	{
		SYSTEMTIME time;
		GetLocalTime(&time);
		CString str;
		str.Format("%4d-%02d-%02d",time.wYear,time.wMonth,time.wDay);
		return str;
	}
};