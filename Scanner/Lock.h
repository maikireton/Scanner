/**
 @brief ���ļ򵥷�װ\n

 @version 1.1
 @author �ϳ�
*/
#pragma once

class CLock
{
public:
	CLock();
	~CLock();

	void Lock();
	void UnLock();

private:
	CRITICAL_SECTION m_cs;
};