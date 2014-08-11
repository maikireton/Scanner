#pragma once

#define RESULT_CORRECT	1
#define RESULT_WRONG	2
#define RESULT_UNKNOWN	3
#define RESULT_SECURITY 4

class Result
{
public:
	Result(void);
	virtual ~Result(void);

public:
	void init();
	void addResult(int type, CString sResult);

private:
	CLock m_lock;
	CStdioFile m_fileCorrect;
	CStdioFile m_fileWrong;
	CStdioFile m_fileUnknown;
	CStdioFile m_fileSecurity;
};

