#include "stdafx.h"
#include "Http.h"


Http::Http(void)
{

}


Http::~Http(void)
{
	OutputDebugString("Http::~Http\n");
	saveHttpFile();
	std::vector<tagHttp*>::iterator iter = m_vectorHttp.begin();
	while (iter != m_vectorHttp.end())
	{
		delete *iter;
		++iter;
	}
	m_vectorHttp.clear();
}

void Http::init()
{
	OutputDebugString("Http::init\n");
	m_nPos = 0;
	m_sHttpFilePath = Function::ApplicationPath();
	m_sHttpFilePath.Append("http.txt");
	loadHttpFile();
}

tagHttp* Http::getNext()
{
	OutputDebugString("Http::getNext\n");
	tagHttp* http = NULL;
	m_lock.Lock();
	if (m_vectorHttp.size() > 0)
	{
		m_nPos = m_nPos%m_vectorHttp.size();
		http = m_vectorHttp[m_nPos++];
	}
	m_lock.UnLock();
	return http;
}

void Http::loadHttpFile()
{
	OutputDebugString("Http::loadHttpFile\n");
	if (!PathFileExists(m_sHttpFilePath)) OutputDebugString("http.txt ²»´æÔÚ");
	
	CStdioFile file(m_sHttpFilePath, CFile::modeRead);
	CString line;
	while (file.ReadString(line))
	{
		line.Trim();
		if (line.GetLength() == 0)
		{
			continue;
		}
		tagHttp *temp = new tagHttp();
		sscanf(line.GetBuffer(), "%[^:]%*c%[^:]%*c%d%*c", temp->mType.GetBuffer(256), temp->mIp.GetBuffer(256), &temp->mPort);
		temp->mType.ReleaseBuffer();
		temp->mIp.ReleaseBuffer();
		m_vectorHttp.push_back(temp);
	}
	file.Close();
}

void Http::saveHttpFile()
{
	OutputDebugString("Http::saveHttpFile\n");
	CStdioFile file(m_sHttpFilePath, CFile::modeCreate|CFile::modeWrite);
	int size = m_vectorHttp.size();
	tagHttp* http = NULL;
	for (int i=0; i<size; i++)
	{
		int pos = (m_nPos+i)%size;
		http = m_vectorHttp[pos];
		CString s;
		s.Format("%s:%s:%u\n", http->mType, http->mIp, http->mPort);
		file.WriteString(s);
	}
	file.Close();
}