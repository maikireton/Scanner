#include "stdafx.h"
#include "Tasks.h"


Tasks::Tasks(void)
{
}


Tasks::~Tasks(void)
{
	OutputDebugString("Tasks::~Tasks\n");
	stopAll();
	BOOL have = TRUE;
	while (have)
	{
		m_lock.Lock();
		have = m_listTask.size()>0?TRUE:FALSE;
		m_lock.UnLock();
		Sleep(10);
	}
}

void Tasks::addTask(Task* task)
{
	OutputDebugString("Tasks::addTask\n");
	m_lock.Lock();
	m_listTask.push_back(task);
	m_lock.UnLock();
}

void Tasks::delTask(Task* task)
{
	OutputDebugString("Tasks::delTask\n");
	m_lock.Lock();
	std::list<Task*>::iterator iter = m_listTask.begin();
	while (iter != m_listTask.end())
	{
		if (*iter == task)
			iter = m_listTask.erase(iter);
		else
			++iter;
	}
	m_lock.UnLock();
}

void Tasks::stopAll()
{
	OutputDebugString("Tasks::stopAll\n");
	m_lock.Lock();
	std::list<Task*>::iterator iter = m_listTask.begin();
	while (iter != m_listTask.end())
	{
		(*iter)->stop();
		++iter;
	}
	m_lock.UnLock();
}

BOOL Tasks::isEmpty()
{
	int num = 0;
	m_lock.Lock();
	num = m_listTask.size();
	m_lock.UnLock();
	return num == 0;
}

int Tasks::size()
{
	int num = 0;
	m_lock.Lock();
	num = m_listTask.size();
	m_lock.UnLock();
	return num;
}