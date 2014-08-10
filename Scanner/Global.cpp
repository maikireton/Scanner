#include "stdafx.h"
#include "Global.h"


Global::Global(void)
{
}


Global::~Global(void)
{
}

void Global::init()
{
	m_account = new Account();
	m_config = new Config();
	m_result = new Result();
	m_http = new Http();
	m_tasks = new Tasks();
	m_account->init();
	m_result->init();
	m_http->init();
	m_state = Run_State_Free;
}

void Global::uninit()
{
	delete m_account;
	delete m_config;
	delete m_result;
	delete m_http;
	delete m_tasks;
}

Config* Global::getConfig()
{
	return m_config;
}

Http* Global::getHttp()
{
	return m_http;
}

Account* Global::getAccount()
{
	return m_account;
}

Result* Global::getResult()
{
	return m_result;
}

Tasks* Global::getTasks()
{
	return m_tasks;
}

void Global::setState(int state)
{
	m_state = state;
}

int Global::getState()
{
	return m_state;
}