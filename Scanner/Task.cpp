#include "stdafx.h"
#include "Task.h"
#include "XiCiSubTask.h"

Task::Task(void)
{
	gGlobal->getTasks()->addTask(this);
}


Task::~Task(void)
{

}

DWORD WINAPI fnSubWork(LPVOID pParam)
{
	OutputDebugString("fnSubWork start\n");
	Task* p = (Task*)pParam;
	do
	{
		if (p->m_account == NULL)
		{
			p->stop();
			OutputDebugString("fnSubWork account is NULL\n");
			break;
		}
		XiCiSubTask* paypal = new XiCiSubTask(p->m_account);
		paypal->run();
		delete paypal;
	} while (false);
	OutputDebugString("fnSubWork stop\n");
	return 1;
}

DWORD WINAPI fnWork(LPVOID pParam)
{
	OutputDebugString("fnWork start\n");
	Task* p = (Task*)pParam;
	HANDLE h = NULL;
	DWORD begin = 0;
	
	while (true)
	{
		if (p->isStop())
		{
			if (h)
			{
				OutputDebugString("fnWork quit kill thread\n");
				if (p->m_account)
				{
					gGlobal->getAccount()->updateAccountState(p->m_account, Account_State_Init);
				}
				TerminateThread(h, 1);
				CloseHandle(h);
			}
			break;
		}
		if (h == NULL)
		{
			p->m_account = gGlobal->getAccount()->getNext();
			p->m_http = gGlobal->getHttp()->getNext();
			OutputDebugString("fnWork create thread\n");
			h = CreateThread(NULL, NULL, fnSubWork, pParam, NULL, NULL);
			begin = time(NULL);
		} 
		else 
		{
			if (WaitForSingleObject(h, 0) == WAIT_OBJECT_0)
			{
				OutputDebugString("fnSubWork quit\n");
				p->m_account = NULL;
				p->m_http = NULL;
				CloseHandle(h);
				h = NULL;
			}
			else 
			{
				if (time(NULL)-begin > gGlobal->getConfig()->getTaskInterval())
				{
					OutputDebugString("fnWork overtime kill thread\n");
					gGlobal->getAccount()->updateAccountState(p->m_account, Account_State_Init);
					p->m_account = NULL;
					p->m_http = NULL;
					TerminateThread(h, 1);
					CloseHandle(h);
					h = NULL;
				}
			}
		}
		Sleep(100);
	}
	OutputDebugString("fnWork stop\n");
	gGlobal->getTasks()->delTask(p);
	delete p;
	return 1;
}

void Task::start()
{
	OutputDebugString("Task::start\n");
	m_bStop = FALSE;
	m_hHandle = CreateThread(NULL, NULL, fnWork, (LPVOID)this, NULL, NULL);
}

void Task::stop()
{
	OutputDebugString("Task::stop\n");
	m_bStop = TRUE;
}

BOOL Task::isStop()
{
	return m_bStop;
}
