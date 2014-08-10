/**
 @brief ËøµÄ¼òµ¥·â×°\n

 @version 1.1
 @author ÐÏ³¬
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