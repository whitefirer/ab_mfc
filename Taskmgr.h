#ifndef _TASKMGR_H_
#define _TASKMGR_H_
#include <tlhelp32.h>
#include <conio.h>
#include <psapi.h>  
#pragma comment(lib,"psapi.lib")

typedef enum  _THREADINFOCLASS   {  
    ThreadBasicInformation,  
        ThreadTimes,  
        ThreadPriority,  
        ThreadBasePriority,  
        ThreadAffinityMask,  
        ThreadImpersonationToken,  
        ThreadDescriptorTableEntry,  
        ThreadEnableAlignmentFaultFixup,  
        ThreadEventPair_Reusable,  
        ThreadQuerySetWin32StartAddress,  
        ThreadZeroTlsCell,  
        ThreadPerformanceCount,  
        ThreadAmILastThread,  
        ThreadIdealProcessor,  
        ThreadPriorityBoost,  
        ThreadSetTlsArrayAddress,  
        ThreadIsIoPending,  
        ThreadHideFromDebugger,  
        ThreadBreakOnTermination,  
        MaxThreadInfoClass  
}THREADINFOCLASS;
THREADINFOCLASS   ThreadInformationClass;

typedef struct _CLIENT_ID{ 
  HANDLE   UniqueProcess; 
  HANDLE   UniqueThread; 
}CLIENT_ID, *PCLIENT_ID; 

typedef   struct   _THREAD_BASIC_INFORMATION   {   //   Information   Class   0 
  LONG    ExitStatus; 
  PVOID    TebBaseAddress; 
  CLIENT_ID  ClientId; 
  LONG    AffinityMask; 
  LONG    Priority; 
  LONG    BasePriority; 
}THREAD_BASIC_INFORMATION, *PTHREAD_BASIC_INFORMATION; 
   
typedef LONG (__stdcall *PFN_ZwQueryInformationThread)(  
                            IN   HANDLE   ThreadHandle,  
                            IN   THREADINFOCLASS   ThreadInformationClass,  
                            OUT   PVOID   ThreadInformation,  
                            IN   ULONG   ThreadInformationLength,  
                            OUT   PULONG   ReturnLength   OPTIONAL  
                                                       );


#endif