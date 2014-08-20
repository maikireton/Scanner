#include "stdafx.h"
#include "XiCiSubTask.h"


XiCiSubTask::XiCiSubTask(tagAccount* account)
	: m_account(account)
{
	m_hInternet = InternetOpen("Mozilla/5.0 (Windows NT 5.1) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/15.0.849.0 Safari/535.1", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
}

XiCiSubTask::~XiCiSubTask(void)
{
}

void XiCiSubTask::run()
{
	HINTERNET hRequest = InternetOpenUrl(m_hInternet, m_account->mName, NULL, NULL, INTERNET_FLAG_NO_COOKIES|INTERNET_FLAG_DONT_CACHE|INTERNET_FLAG_RELOAD, 0);
	if (hRequest)
	{
		DWORD dwSizeOfRq = 4; 
		DWORD dwStatus = 0; 
		HttpQueryInfo(hRequest,   HTTP_QUERY_STATUS_CODE   |   HTTP_QUERY_FLAG_NUMBER, 
			(LPVOID)&dwStatus,   &dwSizeOfRq,   NULL);
		if (dwStatus == 200)
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
			CStringW w = CA2W(sTemp, CP_UTF8);
			sTemp = CW2A(w);
			int nPos = 0;
			int nEnd = 0;
			//<title>国外透明免费HTTP代理IP__第1页国外透明</title>
			nPos = sTemp.Find("<title>");
			nEnd = sTemp.Find("</title>");
			if (nPos != -1)
			{
				CString title = sTemp.Mid(nPos+7, nEnd-nPos-7);
				//gGlobal->getResult()->addResult(RESULT_CORRECT, title);
			}
			while (true)
			{
				CString ip, port, type;
				nPos = sTemp.Find("<tr class=", nPos);
				if (nPos == -1) break;
				nPos = sTemp.Find("<td>", nPos);
				if (nPos == -1) break;
				
				nPos += 4;
				nPos = sTemp.Find("<td>", nPos);
				if (nPos == -1) break;
				nEnd = sTemp.Find("</td>", nPos);
				if (nEnd == -1) break;
				ip = sTemp.Mid(nPos+4, nEnd-nPos-4);
				
				nPos += 4;
				nPos = sTemp.Find("<td>", nPos);
				if (nPos == -1) break;
				nEnd = sTemp.Find("</td>", nPos);
				if (nEnd == -1) break;
				port = sTemp.Mid(nPos+4, nEnd-nPos-4);

				nPos += 4;
				nPos = sTemp.Find("<td>", nPos);

				nPos += 4;
				nPos = sTemp.Find("<td>", nPos);

				nPos += 4;
				nPos = sTemp.Find("<td>", nPos);
				if (nPos == -1) break;
				nEnd = sTemp.Find("</td>", nPos);
				if (nEnd == -1) break;
				type = sTemp.Mid(nPos+4, nEnd-nPos-4);

				CString s = type+":"+ip+":"+port;
				if (type.Find("HTTP") != -1)
					gGlobal->getResult()->addResult(RESULT_CORRECT, s);
			}
		}
		InternetCloseHandle(hRequest);
	}
	gGlobal->getAccount()->updateAccountState(m_account, Account_State_Scanned);
}