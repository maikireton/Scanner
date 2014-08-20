#pragma once
class XiCiSubTask
{
public:
	XiCiSubTask(tagAccount* account);
	~XiCiSubTask(void);

public:
	void run();

private:
	BOOL step1();
	BOOL step2();
	BOOL step3();

private:
	tagAccount* m_account;
	HINTERNET m_hInternet;
};

