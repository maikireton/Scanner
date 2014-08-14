#pragma once
class Task
{
public:
	Task(void);
	~Task(void);

public:
	void start();
	void stop();
	BOOL isStop();

public:
	tagAccount *m_account;
	tagHttp *m_http;

private:
	BOOL m_bStop;
	HANDLE m_hHandle;	
};

