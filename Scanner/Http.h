/**
* 管理http代理
**/
#pragma once

#include <vector>

class Http
{
public:
	Http(void);
	virtual ~Http(void);

public:
	void init();
	tagHttp* getNext();

private:
	void loadHttpFile();
	void saveHttpFile();

private:
	int m_nPos;
	CLock m_lock;
	CString m_sHttpFilePath;
	std::vector<tagHttp*> m_vectorHttp;
};

