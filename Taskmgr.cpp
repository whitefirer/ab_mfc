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
 
  for (DWORD i = 0; i< processcount; i++)           // �о�һ��explorer�µ�ģ��
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
    printf("OpenProcessToken failed!");                    //��ý��̾��ʧ��
  }
  LookupPrivilegeValue(NULL, SE_DEBUG_NAME,&tkp.Privileges[0].Luid);      //��ñ��ػ�Ψһ�ı�ʶ
  tkp.PrivilegeCount = 1;
  tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
  AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES) NULL, 0);  //������õ�Ȩ��
  PFN_ZwQueryInformationThread ZwQueryInformationThread = (PFN_ZwQueryInformationThread)GetProcAddress(GetModuleHandle("ntdll.dll"), "ZwQueryInformationThread"); 
  
  if (GetLastError() != ERROR_SUCCESS)
  {
    printf("AdjustTokenPrivileges enable failed!");              //�޸�Ȩ��ʧ��
  }

  pinfo.dwSize = sizeof( PROCESSENTRY32 );
  BOOL report = Process32First(hProcess, &pinfo);                //����Process32Firstʹ�ÿ��շ��صľ���Խ��̽��б���
  while(report)
  {
      DWORD dwSize, dwWritten; 
      char str_dllname[] = "GUI32.dll";
                                         // ����Ӧ�Ľ���
      HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, pinfo.th32ProcessID);
      hProcess = OpenProcess(PROCESS_ALL_ACCESS | PROCESS_QUERY_INFORMATION  | PROCESS_VM_READ, FALSE, pinfo.th32ProcessID);
      GetModules(hProcess, str_dllname);
      THREADENTRY32 te = { sizeof(THREADENTRY32) };
      printf("����(PID): %d\n", pinfo.th32ProcessID);
    
      if (Thread32First(hThreadSnap, &te))                 // ��ʼö���߳� 
      {  
        do 
        {  
          if (te.th32OwnerProcessID == pinfo.th32ProcessID)
          {
            HANDLE hThread = OpenThread(THREAD_TERMINATE | THREAD_QUERY_INFORMATION, FALSE, te.th32ThreadID);
            if(hThread == NULL)                    // �Թر����ѯ�ķ�ʽ���߳�
            {
              printf("Can not open the thread");
              continue;
            }

            PVOID StarAddress = NULL;                // ����õ�TIB���Լ��̵߳���ʼ��ַ
            DWORD statue = ZwQueryInformationThread(hThread, ThreadBasicInformation, &TIB, sizeof(TIB),  NULL);
            statue = ZwQueryInformationThread(hThread, ThreadQuerySetWin32StartAddress, &StarAddress, sizeof(StarAddress), NULL);
            if(!statue)
            {
              printf("�߳�(TID): %d\tStartAddress: 0x%p\tPID: %d \t", te.th32ThreadID, (int)StarAddress, (int)TIB.ClientId.UniqueProcess);
            }
            else
            {
                printf("QueryInformationThread faild");
            }

            char image[MAX_PATH];
            memset(image,0, sizeof(char));
            GetMappedFileName(hProcess, StarAddress, image, MAX_PATH);    // �õ��̵߳���ʼ��ַ
            
            char temp[MAX_PATH];
            unsigned int j = 0;
            unsigned int len;

            if(image != NULL)
            {
              len = strlen(image) + 1;
            }
            else printf("new the len failed");

            for(DWORD i = 1; i < (len % MAX_PATH); i++)            // �õ�����
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
                                             // �ر��߳�
            CloseHandle(hThread);
          }
        }while (Thread32Next(hThreadSnap, &te));
      }else printf("faild");

      printf("Erro Code:%d", GetLastError());

      dwSize = strlen(str_dllname) + 1; 
      LPVOID lpBuf = VirtualAllocEx(hProcess, NULL, dwSize, MEM_COMMIT, PAGE_READWRITE ); 
      
      if ( !WriteProcessMemory(hProcess, lpBuf, (LPVOID)str_dllname, dwSize, &dwWritten ) )     // д�����
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
      
      WaitForSingleObject(hThread, INFINITE );              // �ȴ�GetModuleHandle������� 
      GetExitCodeThread(hThread, &dwHandle );                // ���GetModuleHandle�ķ���ֵ 
      VirtualFreeEx(hProcess, lpBuf, dwSize, MEM_DECOMMIT );        // �ͷ�Ŀ�����������Ŀռ� 
      

      pFun = GetProcAddress(GetModuleHandle("Kernel32"), "FreeLibrary");  // ʹĿ����̵���FreeLibrary��ж��DLL 
      hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pFun, (LPVOID)dwHandle, 0, NULL); 
      WaitForSingleObject(hThread, INFINITE);                // �ȴ�FreeLibraryж����� 

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
