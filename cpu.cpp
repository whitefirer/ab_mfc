#include <stdafx.h>
#include "CPU.h"

BOOL IniteCPUInformationGet(void)
{	
	NtQuerySystemInformation = (PROCNTQSI)GetProcAddress(GetModuleHandle("ntdll"),"NtQuerySystemInformation");
	if (!NtQuerySystemInformation) 
		return -1;

	LONG status = NtQuerySystemInformation(SystemBasicInformation,&SysBaseInfo,sizeof(SysBaseInfo),NULL); 
	if (status != NO_ERROR) 
		return -1;
	return 0;
}

UINT GetCPUtime(void) 
{ 
	SYSTEM_PERFORMANCE_INFORMATION SysPerfInfo; 
	SYSTEM_TIME_INFORMATION SysTimeInfo; 
	double dbIdleTime = 0.0; 
	double dbSystemTime; 
	LONG status; 
	static LARGE_INTEGER liOldIdleTime = {0,0}; 
	static LARGE_INTEGER liOldSystemTime = {0,0};

	// 得到系统中的进程数
	status = NtQuerySystemInformation(SystemTimeInformation, &SysTimeInfo,sizeof(SysTimeInfo),0); 
	if (status != NO_ERROR) 
		return -1;

	// 得到系统使用时间 
	status = NtQuerySystemInformation(SystemPerformanceInformation,&SysPerfInfo,sizeof(SysPerfInfo),NULL); 
	if (status != NO_ERROR) 
		return -1;
	
	if (liOldIdleTime.QuadPart != 0) 
	{ 
			dbIdleTime = Li2Double(SysPerfInfo.liIdleTime) - Li2Double(liOldIdleTime); 
			dbSystemTime = Li2Double(SysTimeInfo.liKeSystemTime) - Li2Double(liOldSystemTime);
			dbIdleTime = dbIdleTime / dbSystemTime;
			dbIdleTime = 100.0 - dbIdleTime * 100.0 / (double)SysBaseInfo.bKeNumberProcessors + 0.5; 
#ifdef DEBUG
			printf("%3d%", (UINT)dbIdleTime); // DEBUG
#endif
	}

	liOldIdleTime = SysPerfInfo.liIdleTime; 
	liOldSystemTime = SysTimeInfo.liKeSystemTime;

 return (UINT)dbIdleTime;
}