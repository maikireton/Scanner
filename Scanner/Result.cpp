#include "stdafx.h"
#include "Result.h"


Result::Result(void)
{

}


Result::~Result(void)
{
	OutputDebugString("Result::~Result\n");
	if (m_file.m_hFile != CFile::hFileNull)
		m_file.Close();
}

void Result::init()
{
	OutputDebugString("Result::init\n");
	CString path = Function::ApplicationPath();
	PathAppend(path.GetBuffer(), "result\\");
	if (!PathIsDirectory(path))
		CreateDirectory(path, NULL);
	CString name;
	name.Format("%s-%d.txt", Function::Date(), time(NULL));
	PathAppend(path.GetBuffer(), name);
	if (!m_file.Open(path, CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone))
		OutputDebugString("Result::init open file error\n");
}

void Result::addResult(CString sResult)
{
	OutputDebugString("Result::addResult\n");
	m_lock.Lock();
	m_file.WriteString(Function::Now());
	m_file.WriteString("\t");
	m_file.WriteString(sResult);
	m_file.WriteString("\n");
	m_file.Flush();
	m_lock.UnLock();
}