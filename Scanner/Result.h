#pragma once

class Result
{
public:
	Result(void);
	virtual ~Result(void);

public:
	void init();
	void addResult(CString sResult);

private:
	CLock m_lock;
	CStdioFile m_file;
};

