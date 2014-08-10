#pragma once
#include "Task.h"
#include <list>

class Tasks
{
public:
	Tasks(void);
	~Tasks(void);

public:
	void addTask(Task* task);
	void delTask(Task* task);
	void stopAll();
	BOOL isEmpty();
	int size();

private:
	CLock m_lock;
	std::list<Task*> m_listTask;
};

