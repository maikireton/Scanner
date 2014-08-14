#include "stdafx.h"
#include "Account.h"


Account::Account(void)
{
}

Account::~Account(void)
{
	OutputDebugString("Account::~Account\n");
	saveAccountFile();
	std::vector<tagAccount*>::iterator iter = m_vectorAccount.begin();
	while (iter != m_vectorAccount.end())
	{
		delete *iter;
		++iter;
	}
	m_vectorAccount.clear();
}

void Account::init()
{
	OutputDebugString("Account::init\n");
	m_nPos = 0;
	m_nStateInit = 0;
	m_nStateScanning = 0;
	m_nStateScanned = 0;
	m_sAccountFilePath = Function::ApplicationPath();
	m_sAccountFilePath.Append("account.txt");
	loadAccountFile();
}

tagAccount* Account::getNext()
{
	OutputDebugString("Account::getNext\n");
	tagAccount* account = NULL;
	m_lock.Lock();
	if (m_vectorAccount.size() > 0)
	{
		int size = m_vectorAccount.size();
		for (int i=0; i<size; i++)
		{
			m_nPos = (m_nPos+i)%size;
			account = m_vectorAccount[m_nPos];
			if (account->mState == Account_State_Init)
				break;
			account = NULL;
		}
		if (account)
			updateAccountState(account, Account_State_Scanning);
	}
	m_lock.UnLock();
	return account;
}

void Account::loadAccountFile()
{
	OutputDebugString("Account::loadAccountFile\n");
	if (!PathFileExists(m_sAccountFilePath)) OutputDebugString("account.txt no exist");
	
	CStdioFile file(m_sAccountFilePath, CFile::modeRead);
	CString line;
	while (file.ReadString(line))
	{
		line.Trim();
		if (line.GetLength() == 0)
		{
			continue;
		}
		tagAccount* temp = new tagAccount();
		updateAccountState(temp, Account_State_Init);
		sscanf(line.GetBuffer(), "%[^\t]%*c%[^\t]", temp->mName.GetBuffer(256), temp->mPassWord.GetBuffer(256));
		temp->mName.ReleaseBuffer();
		temp->mPassWord.ReleaseBuffer();
		m_vectorAccount.push_back(temp);
	}
	file.Close();
}

void Account::saveAccountFile()
{
	OutputDebugString("Account::saveAccountFile\n");
	CStdioFile file(m_sAccountFilePath, CFile::modeCreate|CFile::modeWrite);
	int size = m_vectorAccount.size();
	tagAccount* account = NULL;
	for (int i=0; i<size; i++)
	{
		int pos = (m_nPos+i)%size;
		account = m_vectorAccount[pos];
		if (account->mState == Account_State_Scanned)
			continue;
		CString s;
		s.Format("%s\t%s\n", account->mName, account->mPassWord);
		file.WriteString(s);
	}
	file.Close();
}

void Account::updateAccountState(tagAccount* account, int state)
{
	m_lock.Lock();

	if (account->mState == Account_State_Init) --m_nStateInit;
	if (account->mState == Account_State_Scanning) --m_nStateScanning;
	if (account->mState == Account_State_Scanned) --m_nStateScanned;

	if (state == Account_State_Init) ++m_nStateInit;
	if (state == Account_State_Scanning) ++m_nStateScanning;
	if (state == Account_State_Scanned) ++m_nStateScanned;

	account->mState = state;

	m_lock.UnLock();
}

int Account::getAccountStateNum(int state)
{
	int num = 0;

	m_lock.Lock();

	if (state == Account_State_Init) num = m_nStateInit;
	else if (state == Account_State_Scanning) num = m_nStateScanning;
	else if (state == Account_State_Scanned) num = m_nStateScanned; 
	
	m_lock.UnLock();

	return num;
}