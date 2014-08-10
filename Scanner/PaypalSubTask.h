#pragma once

#include <map>

class PaypalSubTask
{
public:
	PaypalSubTask(tagAccount* account, tagHttp* http);
	~PaypalSubTask(void);

public:
	void run();

private:
	BOOL step1();
	BOOL step2();
	BOOL step3();

private:
	tagHttp* m_http;
	tagAccount* m_account;
	HINTERNET m_hInternet;
	std::map<CString, CString> m_mapCookie;
	CString m_sDispatch;
	CString m_sLocation;
	CString m_sPerson;
	CString m_sVerify;
	CString m_sCountry;
	CString m_sMoney;
};

