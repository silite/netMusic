#include <windows.h>
#include <iostream>
#include <Tlhelp32.h>
#include <stdio.h>
using namespace std;

/// <summary>
/// 根据进程名称获取进程信息
/// </summary>
/// <param name="info"></param>
/// <param name="processName"></param>
/// <returns></returns>
BOOL getProcess32Info(PROCESSENTRY32* info, const TCHAR processName[])
{
    HANDLE handle; //定义CreateToolhelp32Snapshot系统快照句柄
    handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);//获得系统快照句柄
    //PROCESSENTRY32 结构的 dwSize 成员设置成 sizeof(PROCESSENTRY32)
    info->dwSize = sizeof(PROCESSENTRY32);
    //调用一次 Process32First 函数，从快照中获取进程列表
    Process32First(handle, info);
    //重复调用 Process32Next，直到函数返回 FALSE 为止
    while (Process32Next(handle, info) != FALSE)
    {
        if (wcscmp(processName, info->szExeFile) == 0)
        {
            return TRUE;
        }
    }
    return FALSE;
}

/// <summary>
/// 注入DLL文件
/// </summary>
/// <param name="DllFullPath">DLL文件的全路径</param>
/// <param name="dwRemoteProcessId">要注入的程序的PID</param>
/// <returns></returns>
BOOL InjectDLL(const wchar_t* DllFullPath, const DWORD dwRemoteProcessId)
{
    // 计算路径的字节数
    int pathSize = (wcslen(DllFullPath) + 1) * sizeof(wchar_t);
    // 获取句柄后，可以完全控制进程
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwRemoteProcessId);
    if (hProcess == NULL) {
        cout << "获取句柄失败" << endl;
        return FALSE;
    }
    // TerminateProcess(hProcess, 0);//关闭句柄对象
    // 实现注入
    // 1.首先要提升权限，打开进程的访问令牌
    // 【参数1】当前程序
    // 【参数2】权限，可添加的权限|可查询的权限
    HANDLE hToken;
    if (FALSE == OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES |
        TOKEN_QUERY, &hToken)) {
        // 权限修改失败
        cout << "权限修改失败" << endl;
        return FALSE;
    }
    //2.查看与进程相关的特权信息
    LUID luid;
    if (FALSE == LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid)) {
        // 特权信息查询失败
        cout << "特权信息查询失败" << endl;
        return FALSE;
    };
    //3.调节进程的访问令牌的特权属性
    // 这几行代码固定不变
    TOKEN_PRIVILEGES tkp;
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Luid = luid;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; // 打开特权
    // 【参数1】访问令牌
    // 【参数2】是否禁用特权
    // 【参数3】新特权所占的字节数
    // 【参数4】原来的特权是否需要保存
    // 【参数5】原特权的长度
    if (FALSE == AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL))
    {
        // 提升特权失败
        cout << "提升特权失败" << endl;
        return FALSE;
    };

    //在远程进程中申请内存空间
    // 【参数1】程序的句柄对象
    // 【参数2】申请的内存地址，由系统分配，所以为NULL
    // 【参数3】申请的内存长度
    // 【参数4】调用物理存储器
    // 【参数5】这块内存可读可写，可执行
    // 【返回】申请到的地址
    LPVOID lpAddr = VirtualAllocEx(hProcess, NULL, pathSize, MEM_COMMIT,
        PAGE_EXECUTE_READWRITE);
    if (lpAddr == NULL) {
        // 在远程进程中申请内存失败
        cout << "在远程进程中申请内存失败" << endl;
        return FALSE;
    }
    // 把DLL路径写入到远程进程中
    // 强行修改程序的内存
    // 【参数1】程序的句柄
    // 【参数2】申请到的内存首地址
    // 【参数3】写入的内容
    // 【参数4】要写入的字节数
    // 【参数5】
    if (FALSE == WriteProcessMemory(hProcess, lpAddr, DllFullPath,
        pathSize, NULL)) {
        // 在远程进程中写入数据失败
        cout << "在远程进程中写入数据失败" << endl;
        return FALSE;
    };


    // 调用Kernel32.dll中的LoadLibraryW方法用以加载DLL文件
    PTHREAD_START_ROUTINE pfnStartAssr =
        (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(L"Kernel32.dll"),
            "LoadLibraryW");

    // 在远程进程中开辟线程
    // 【参数1】远程线程的句柄
    // 【参数2】线程属性。NULL表示使用默认属性
    // 【参数3】堆栈大小。0代表默认
    // 【参数4】加载DLL文件的对象
    // 【参数5】加载文件的路径
    // 【参数6】延迟时间。0代表立即启动
    // 【参数7】线程ID。为NULL就行了
    HANDLE hRemoteThread = CreateRemoteThread(hProcess, NULL, 0, pfnStartAssr, lpAddr, 0,
        NULL);
    if (hRemoteThread == NULL) {
        // 创建远程线程失败
        cout << "创建远程线程失败" << endl;
        // 释放内存
        VirtualFreeEx(hProcess, lpAddr, 0, MEM_FREE);
        return FALSE;
    }
    cout << "注入成功" << endl;
    // 等待线程结束
    WaitForSingleObject(hRemoteThread, -1);
    // 关闭线程
    CloseHandle(hRemoteThread);
    // 释放内存
    VirtualFreeEx(hProcess, lpAddr, 0, MEM_FREE);
}


int main()
{
    PROCESSENTRY32 info;
    if (getProcess32Info(&info, L"cloudmusic.exe"))
    {
        // 24
        InjectDLL(L"C:/Users/silite/Desktop/netMusic/release/betterncm.dll", info.th32ProcessID);//这个dll你所要注入的dll文件，这个"数字"是你想注入的进程的PID号
    }
    else {
        cout << "查找失败" << endl;
    }
    return 0;
}
