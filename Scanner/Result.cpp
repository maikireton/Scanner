#include "stdafx.h"
#include "Result.h"


Result::Result(void)
{

}


Result::~Result(void)
{
	OutputDebugString("Result::~Result\n");
	if (m_fileCorrect.m_hFile != CFile::hFileNull)
		m_fileCorrect.Close();
	if (m_fileWrong.m_hFile != CFile::hFileNull)
		m_fileWrong.Close();
	if (m_fileUnknown.m_hFile != CFile::hFileNull)
		m_fileUnknown.Close();
	if (m_fileSecurity.m_hFile != CFile::hFileNull)
		m_fileSecurity.Close();
}

void Result::init()
{
	OutputDebugString("Result::init\n");
	CString path = Function::ApplicationPath();
	PathAppend(path.GetBuffer(), "result\\");
	if (!PathIsDirectory(path))
		CreateDirectory(path, NULL);
	CString name;
	name.Format("%s-%d", Function::Date(), time(NULL));
	CString file;
	file.Format("%s正确-%s.txt", path.GetBuffer(), name.GetBuffer());
	if (!m_fileCorrect.Open(file, CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone))
		OutputDebugString("Result::init open m_fileCorrect error\n");
	file.Format("%s错误-%s.txt", path.GetBuffer(), name.GetBuffer());
	if (!m_fileWrong.Open(file, CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone))
		OutputDebugString("Result::init open m_fileWrong error\n");
	file.Format("%s未知-%s.txt", path.GetBuffer(), name.GetBuffer());
	if (!m_fileSecurity.Open(file, CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone))
		OutputDebugString("Result::init open m_fileSecurity error\n");
	file.Format("%s验证码-%s.txt", path.GetBuffer(), name.GetBuffer());
	if (!m_fileUnknown.Open(file, CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone))
		OutputDebugString("Result::init open file error\n");
}

void Result::addResult(int type, CString sResult)
{
	OutputDebugString("Result::addResult\n");
	m_lock.Lock();
	CStdioFile *file = NULL;
	if (type == RESULT_CORRECT)
	{
		file = &m_fileCorrect;
	}
	else if (type == RESULT_WRONG)
	{
		file = &m_fileWrong;
	}
	else if (type == RESULT_UNKNOWN)
	{
		file = &m_fileUnknown;
	}
	else if (type == RESULT_SECURITY)
	{
		file = &m_fileSecurity;
	}
	file->WriteString(Function::Now());
	file->WriteString("\t");
	file->WriteString(sResult);
	file->WriteString("\n");
	file->Flush();
	m_lock.UnLock();
}