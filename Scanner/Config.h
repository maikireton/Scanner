#pragma once
class Config
{
public:
	Config(void);
	virtual ~Config(void);

public:
	void setThreadNum(int nThreadNum);
	int getThreadNum();
	void setTaskInterval(int nInterval);
	int getTaskInterval();

private:
	CString m_sConfigPath;
};

