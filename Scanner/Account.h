/**
* π‹¿Ì’ ∫≈
**/
#pragma once

#include <vector>

class Account
{
public:
	Account(void);
	virtual ~Account(void);

public:
	void init();
	tagAccount* getNext();
	void updateAccountState(tagAccount* account, int state);
	int getAccountStateNum(int state);

private:
	void loadAccountFile();
	void saveAccountFile();

private:
	int m_nPos;
	CLock m_lock;
	CString m_sAccountFilePath;
	std::vector<tagAccount*> m_vectorAccount;
	int m_nStateInit;
	int m_nStateScanning;
	int m_nStateScanned;
};

