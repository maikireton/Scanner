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

private:
	BOOL m_bStop;
	HANDLE m_hHandle;
};

