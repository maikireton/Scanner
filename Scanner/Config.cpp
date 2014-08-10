#include "stdafx.h"
#include "Config.h"

#define SECTION_COMMON "section_common"
#define THREAD_NUM	"thread_num"
#define TASK_INTERVAL "task_interval"

Config::Config(void)
{
	m_sConfigPath = Function::ApplicationPath();
	PathAppend(m_sConfigPath.GetBuffer(), "config.ini");
}


Config::~Config(void)
{
}

void Config::setThreadNum(int nThreadNum)
{
	CString value;
	value.Format("%d", nThreadNum);
	WritePrivateProfileStringA(SECTION_COMMON, THREAD_NUM, value, m_sConfigPath);
}

int Config::getThreadNum()
{
	return GetPrivateProfileInt(SECTION_COMMON, THREAD_NUM, 1, m_sConfigPath);
}
	
void Config::setTaskInterval(int nInterval)
{
	CString value;
	value.Format("%d", nInterval);
	WritePrivateProfileStringA(SECTION_COMMON, TASK_INTERVAL, value, m_sConfigPath);
}

int Config::getTaskInterval()
{
	return GetPrivateProfileInt(SECTION_COMMON, TASK_INTERVAL, 0, m_sConfigPath);
}
