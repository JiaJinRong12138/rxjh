// InjectDLL.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//


#include <iostream>
#include <windows.h>
#define GAME_WND_CLASS "D3D Window"
#define DLL_NAME "MFC_DLL.dll"
#define GAME_TITLE "YB_OnlineClient"

using namespace std;


BOOL InjectDll(const char* DllPath, HWND GameH) {

	DWORD pid = 0;
	HANDLE hProcess = NULL;
	LPDWORD lpAddr = NULL; // 获取远程分配成功的地址
	DWORD size = NULL;
	HANDLE threadHandle = NULL;

	cout << DllPath << endl;
	if (GameH != 0) {
		//句柄获取成功
		// 获取进程PID

		GetWindowThreadProcessId(GameH, &pid);
		if (pid != 0) {
			// PID 获取成功
			// 获取进程句柄
			// 开启所以权限打开进程

			hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE ,pid);
			if (hProcess != NULL) {
				// 打开进程成功
				// 分配内存空间，写入动态链接库的全路径名
				//D:\\c_work\\MFC_DLL\\Debug\\MFC_DLL.dll
				
				lpAddr = (LPDWORD)VirtualAllocEx(hProcess, NULL, 256, MEM_COMMIT, PAGE_READWRITE);
				if (lpAddr != NULL) {
					// 地址分配成功, 写入DLL 的全路径
					WriteProcessMemory(hProcess, lpAddr, DllPath, strlen(DllPath) + 1, &size);

					if (size >= strlen(DllPath)) {
						threadHandle = CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryA, lpAddr, NULL, NULL);
						// 等待注入DLL 的线程执行完再执行下一步(等待的进程句柄， 等到多少毫秒)
					
						// 清除内存数据
						WaitForSingleObject(threadHandle, 0xFFFFFFFF);
						// 关闭线程
						CloseHandle(threadHandle);
						// 释放进程
						VirtualFreeEx(hProcess, lpAddr, 256, MEM_DECOMMIT);
						// 关闭句柄
						CloseHandle(hProcess);
						return TRUE;
					}
					else {
						cout << "写入DLL 失败" << endl;
						return FALSE;
					}
				}
				else {
					cout << "地址分配失败" << endl;
					return FALSE;
				}

			}
			else {
				cout << "打开进程失败" << endl;
				return FALSE;
			}
		}
		else {
			cout << "获取PID 失败" << endl;
			return FALSE;
		}


	}
	else {
		cout << "获取窗口句柄失败" << endl;
		return FALSE;
	}



}

int main()
{
    
	

	// 注入DLL
	// 打开游戏进程的句柄
	/*
	1、 获取游戏进程的PID
	2、 根据PID 获取进程
	3、 在目标进程分配内存空间，方便写入DLL 全路径
	4、 将DLL 全路径写入到目标进程
	5、 远程注入DLL
	6、 等待目标进程执行完成
	7、 释放进程空间
	8、 关闭线程句柄
	*/


	cout << "开始注入DLL" << endl;

	DWORD dwPID;
	// 获取游戏窗口句柄
	HWND GameH = FindWindow((LPCTSTR)GAME_WND_CLASS, GAME_TITLE);

	char path[256] = "D:\\c_work\\MFC_DLL\\lib";

	// 拼接字符串（添加要查找的DLL 文件）
	strcat_s(path, "\\");
	strcat_s(path, DLL_NAME);

	if (InjectDll(path, GameH)) {
		cout << "注入DLL 成功" << endl;
	}
	else {
		cout << "注入DLL 失败" << endl;
	}

	//system("pause");

}

