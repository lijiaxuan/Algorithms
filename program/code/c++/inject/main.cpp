#include <windows.h>
//#include <winnt.h>
#include <psapi.h>
#include <stdlib.h>
#include <tchar.h>
#include <stdio.h>
#include <string>
#define THREAD_SIZE 512
using namespace std;
typedef struct _REMOTE_PARAMETER
{
    CHAR m_msgContent[MAX_PATH];
    CHAR m_msgTitle[MAX_PATH];
    DWORD m_dwMessageBoxAddr;
}RemotePara,*pRemoteParaAddr;
int AdjustProcessTokenPrivilege()
{
    LUID luidTmp;
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;
    if(!OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&hToken))
    {
        OutputDebugString("OpenProcessToken Failed \n");
        return false;
    }
    if(!LookupPrivilegeValue(NULL,SE_DEBUG_NAME,&luidTmp))
    {
        OutputDebugString("LookupPrivilegeValue Failed \n");
        CloseHandle(hToken);
        return false;
    }
    tkp.PrivilegeCount=1;
    tkp.Privileges[0].Luid=luidTmp;
    tkp.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;
    if(!AdjustTokenPrivileges(hToken,false,&tkp,sizeof(tkp),NULL,NULL))
    {
        OutputDebugString("AdjustProcessTokenPrivilege AdjustTokenPrivileges Failed \n");
        CloseHandle(hToken);
        return false;
    }
    return true;
}
/** \brief get the process name by process ID provided
 *
 * \param
 * \param
 * \return
 *
 */

bool ProcessIsExplorer(DWORD dwProcessId)
{
    HANDLE hProcess;
    hProcess=NULL;
    hProcess=OpenProcess(PROCESS_VM_READ|PROCESS_QUERY_INFORMATION,false,dwProcessId);
    if(hProcess==NULL)
    {
        OutputDebugString("ProcessIsExplorer - OpenProcess Failed , Error Code Is %d , Error Message Is %s !");
        return false;
    }
    DWORD dwNameLen;
    TCHAR pathArray[MAX_PATH];
    ZeroMemory(pathArray,MAX_PATH);
    dwNameLen=0;
    dwNameLen=GetModuleFileNameEx(hProcess, NULL, pathArray, MAX_PATH);
    if(dwNameLen==0)
    {
        CloseHandle(hProcess);
        return false;
    }
    char exeNameArray[MAX_PATH];
    ZeroMemory(exeNameArray,MAX_PATH);
    //_splitpath(pathArray, NULL, NULL, exeNameArray, NULL);
    int i=0,j=0;
    char *p=pathArray,*tmp1=NULL,*tmp2=NULL;
    for(i=0;*p!=0;i++,p++);
    tmp1=p;
    for(;*p!='\\';i--,p--);
    p++;
    for(j=0;*p!='.';j++,p++)
    {
        exeNameArray[j]=*p;
    }
    printf("%s\n",exeNameArray);
    string exename=exeNameArray;
    if(exename.compare("Explorer")==0||exename.compare("explorer")==0)
    {
        CloseHandle(hProcess);
        return TRUE;
    }
    return FALSE;
}
void GetMessageBoxPamameter(pRemoteParaAddr pRemoteParaAddr)
{
    HMODULE hUser32=LoadLibrary("User32.dll");
    pRemoteParaAddr->m_dwMessageBoxAddr=(DWORD)GetProcAddress(hUser32,"MessageBoxA");
    strcat(pRemoteParaAddr->m_msgContent,"Hello,Sapphire");
    strcat(pRemoteParaAddr->m_msgTitle,"Hello");
    FreeLibrary(hUser32);
}
DWORD WINAPI RemoteThreadProc(pRemoteParaAddr pRemotePara)
{
    typedef int (WINAPI *MESSAGEBOXA)(HWND,LPCSTR,LPCSTR,UINT);
    MESSAGEBOXA MessageBoxA;
    MessageBoxA=(MESSAGEBOXA)pRemotePara->m_dwMessageBoxAddr;
    MessageBoxA(NULL,pRemotePara->m_msgContent,pRemotePara->m_msgTitle,MB_OK);
    return 0;
}
int main()
{
    int s;
    DWORD dwProcess[100];
    DWORD dwExplorer=0;
    HANDLE hProcess=NULL;
    int i=0;
    DWORD dwNeeded=100;
    AdjustProcessTokenPrivilege();
    EnumProcesses(dwProcess,sizeof(dwProcess),&dwNeeded);
    //return 0;
    for(i=0;i<dwNeeded/sizeof(DWORD);i++)
    {
        if(dwProcess[i]!=0)
        {
            if(ProcessIsExplorer(dwProcess[i]))
            {
                dwExplorer=dwProcess[i];
                break;
            }
        }
    }
    hProcess=OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwExplorer);
    if(NULL==hProcess)
    {
        OutputDebugStr("Open explore failed");
    }

    PVOID pRemoteThread=VirtualAllocEx(hProcess,NULL,THREAD_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if(pRemoteThread==NULL)
    {
        printf("remote fail\n");
        CloseHandle(hProcess);
    }
    RemotePara remotePara;
    ZeroMemory(&remotePara,sizeof(RemotePara));
    GetMessageBoxPamameter(&remotePara);
    pRemoteParaAddr PRemoteParaAddr=(pRemoteParaAddr)VirtualAllocEx(hProcess,NULL,sizeof(RemotePara),MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);
    if(PRemoteParaAddr==NULL)
    {
        printf("REMOTEPARA fail");
        VirtualFreeEx(hProcess,pRemoteThread,0,MEM_RELEASE);
        CloseHandle(hProcess);
    }

    if(WriteProcessMemory(hProcess,pRemoteThread,(LPCVOID)&RemoteThreadProc,THREAD_SIZE,0)==FALSE)
    {
        printf("write process memory fail");
        VirtualFreeEx(hProcess,pRemoteThread,0,MEM_RELEASE);
        CloseHandle(hProcess);
    }
    if(WriteProcessMemory(hProcess,PRemoteParaAddr,&remotePara,sizeof(RemotePara),0)==FALSE)
    {
        printf("write parameter fail");
        VirtualFreeEx(hProcess, pRemoteThread, 0, MEM_RELEASE);
        VirtualFreeEx(hProcess, PRemoteParaAddr, 0, MEM_RELEASE);
        CloseHandle(hProcess);
    }


    HANDLE hThread;
    DWORD dwThreadId;
    hThread=NULL;
    dwThreadId=0;
    hThread=CreateRemoteThread(hProcess,NULL,0,(DWORD(WINAPI*)(LPVOID))pRemoteThread,PRemoteParaAddr,0,&dwThreadId);
    if(NULL==hThread)
    {
        printf("remote fail");
    }
    else
    {
        printf("inject success");
    }
    WaitForSingleObject(hThread,INFINITE);
    CloseHandle(hThread);
    VirtualFreeEx(hProcess, pRemoteThread, 0, MEM_RELEASE);
    VirtualFreeEx(hProcess, PRemoteParaAddr, 0, MEM_RELEASE);
    CloseHandle(hProcess);
    //scanf("%d",&s);
    //ProcessIsExplorer(s);
}
