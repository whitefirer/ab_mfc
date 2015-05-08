#include "taskmgr.h"

BOOL GetModules(HANDLE hProcess, char* Strings)
{
  DWORD processid[1024], needed, processcount, modulecount;
  HMODULE hModule[1024];

  DWORD cb = 0;
  BOOL ret = 1;
  char path[MAX_PATH] = "", temp[MAX_PATH], basename[MAX_PATH];
  EnumProcesses(processid, sizeof(processid), &needed);
  processcount = 1;// needed/sizeof(DWORD);
 
  for (DWORD i = 0; i< processcount; i++)           // 列举一下explorer下的模块
  {
    if (hProcess)
    {    
      EnumProcessModules(hProcess, hModule, sizeof(hModule), &needed);
      modulecount = needed / sizeof(DWORD);
      //_itoa(processid[i], temp, 10);
      for (DWORD j = 0; j < modulecount; j++)
      {
        GetModuleFileNameEx(hProcess, hModule[j], path, sizeof(path));
        GetModuleBaseName(hProcess, hModule[j], basename, sizeof(basename));
        GetShortPathName(path, path, 256);
        
        if(!strcmp(basename, Strings))
        {
          ret = 1;
        }
        printf("%s\t\t%s\n", basename, path);
      }
    }
  }

return ret;
}

int test()
{  
  HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

  PROCESSENTRY32 pinfo;
  THREADENTRY32 tinfo;
  DWORD dwHandle = 0; 
  TOKEN_PRIVILEGES tkp;
  HANDLE hToken;
  THREAD_BASIC_INFORMATION  TIB;
  
  memset(&pinfo,0, sizeof(pinfo));
  memset(&tinfo,0, sizeof(tinfo));

  if (!OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,&hToken))
  {
    printf("OpenProcessToken failed!");                    //获得进程句柄失败
  }
  LookupPrivilegeValue(NULL, SE_DEBUG_NAME,&tkp.Privileges[0].Luid);      //获得本地机唯一的标识
  tkp.PrivilegeCount = 1;
  tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
  AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES) NULL, 0);  //调整获得的权限
  PFN_ZwQueryInformationThread ZwQueryInformationThread = (PFN_ZwQueryInformationThread)GetProcAddress(GetModuleHandle("ntdll.dll"), "ZwQueryInformationThread"); 
  
  if (GetLastError() != ERROR_SUCCESS)
  {
    printf("AdjustTokenPrivileges enable failed!");              //修改权限失败
  }

  pinfo.dwSize = sizeof( PROCESSENTRY32 );
  BOOL report = Process32First(hProcess, &pinfo);                //调用Process32First使用快照返回的句柄对进程进行遍历
  while(report)
  {
      DWORD dwSize, dwWritten; 
      char str_dllname[] = "GUI32.dll";
                                         // 打开相应的进程
      HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, pinfo.th32ProcessID);
      hProcess = OpenProcess(PROCESS_ALL_ACCESS | PROCESS_QUERY_INFORMATION  | PROCESS_VM_READ, FALSE, pinfo.th32ProcessID);
      GetModules(hProcess, str_dllname);
      THREADENTRY32 te = { sizeof(THREADENTRY32) };
      printf("进程(PID): %d\n", pinfo.th32ProcessID);
    
      if (Thread32First(hThreadSnap, &te))                 // 开始枚举线程 
      {  
        do 
        {  
          if (te.th32OwnerProcessID == pinfo.th32ProcessID)
          {
            HANDLE hThread = OpenThread(THREAD_TERMINATE | THREAD_QUERY_INFORMATION, FALSE, te.th32ThreadID);
            if(hThread == NULL)                    // 以关闭与查询的方式打开线程
            {
              printf("Can not open the thread");
              continue;
            }

            PVOID StarAddress = NULL;                // 下面得到TIB表以及线程的起始地址
            DWORD statue = ZwQueryInformationThread(hThread, ThreadBasicInformation, &TIB, sizeof(TIB),  NULL);
            statue = ZwQueryInformationThread(hThread, ThreadQuerySetWin32StartAddress, &StarAddress, sizeof(StarAddress), NULL);
            if(!statue)
            {
              printf("线程(TID): %d\tStartAddress: 0x%p\tPID: %d \t", te.th32ThreadID, (int)StarAddress, (int)TIB.ClientId.UniqueProcess);
            }
            else
            {
                printf("QueryInformationThread faild");
            }

            char image[MAX_PATH];
            memset(image,0, sizeof(char));
            GetMappedFileName(hProcess, StarAddress, image, MAX_PATH);    // 得到线程的起始地址
            
            char temp[MAX_PATH];
            unsigned int j = 0;
            unsigned int len;

            if(image != NULL)
            {
              len = strlen(image) + 1;
            }
            else printf("new the len failed");

            for(DWORD i = 1; i < (len % MAX_PATH); i++)            // 得到名称
            {
              if(image[i - 1] == '\\')
                j = 0;
              temp[j] = image[i];
              j++;
            }
            temp[j] = '\0';

            printf("\nThe image: %s \n", temp);
            if(!strcmp(temp, str_dllname))
              TerminateThread(hThread, 0);
                                             // 关闭线程
            CloseHandle(hThread);
          }
        }while (Thread32Next(hThreadSnap, &te));
      }else printf("faild");

      printf("Erro Code:%d", GetLastError());

      dwSize = strlen(str_dllname) + 1; 
      LPVOID lpBuf = VirtualAllocEx(hProcess, NULL, dwSize, MEM_COMMIT, PAGE_READWRITE ); 
      
      if ( !WriteProcessMemory(hProcess, lpBuf, (LPVOID)str_dllname, dwSize, &dwWritten ) )     // 写入进程
      { 
        VirtualFreeEx(hProcess, lpBuf, dwSize, MEM_DECOMMIT); 
        CloseHandle(hProcess); 
        return FALSE; 
      }                                    
      
      LPVOID pFun = GetProcAddress(GetModuleHandle("Kernel32"), "GetModuleHandleA");// GetModuleHandleA; 
      HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0,(LPTHREAD_START_ROUTINE)pFun, lpBuf, 0, NULL); 
      if (hThread == NULL) 
      { 
        CloseHandle(hProcess); 
        return FALSE; 
      } 
      
      WaitForSingleObject(hThread, INFINITE );              // 等待GetModuleHandle运行完毕 
      GetExitCodeThread(hThread, &dwHandle );                // 获得GetModuleHandle的返回值 
      VirtualFreeEx(hProcess, lpBuf, dwSize, MEM_DECOMMIT );        // 释放目标进程中申请的空间 
      

      pFun = GetProcAddress(GetModuleHandle("Kernel32"), "FreeLibrary");  // 使目标进程调用FreeLibrary，卸载DLL 
      hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pFun, (LPVOID)dwHandle, 0, NULL); 
      WaitForSingleObject(hThread, INFINITE);                // 等待FreeLibrary卸载完毕 

      CloseHandle(hThreadSnap);
      CloseHandle(hThread);
    }
    report = Process32Next(hProcess, &pinfo);
  }
  printf("\n Clear it success!");
  printf("\n By whitefirer");
  _getch();
  CloseHandle(hProcess); 
  return 0;
}
