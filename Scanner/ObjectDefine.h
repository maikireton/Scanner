#pragma once

#include <map>

/* http define */
struct tagHttp {
	CString mType;
	CString mIp;
	int mPort;
};

/* Account State */
enum Account_State {
	Account_State_Init,
	Account_State_Scanning,
	Account_State_Scanned
};

/* Account define */
struct tagAccount {
	CString mName;
	CString mPassWord;
	int mState;
};

/* Run State */
enum Run_State {
	Run_State_Free,
	Run_State_Running,
	Run_State_Stopping
};