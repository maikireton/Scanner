#pragma once

#include "Config.h"
#include "Http.h"
#include "Account.h"
#include "Result.h"
#include "Tasks.h"

class Global
{
public:
	Global(void);
	~Global(void);

public:
	void init();
	void uninit();
	Config* getConfig();
	Http* getHttp();
	Account* getAccount();
	Result* getResult();
	Tasks* getTasks();
	void setState(int state);
	int getState();

private:
	Config* m_config;
	Http* m_http;
	Account* m_account;
	Result* m_result;
	Tasks* m_tasks;
	int m_state;
};

