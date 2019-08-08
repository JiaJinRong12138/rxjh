// MFC_DLL.cpp: 定义 DLL 的初始化例程。
//

#include "pch.h"
#include "framework.h"
#include "MFC_DLL.h"
// 包含含有主窗口的class1的头文件
#include "CMainDialogWnd.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO:  如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。  这意味着
//		它必须作为以下项中的第一个语句:
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CMFCDLLApp

BEGIN_MESSAGE_MAP(CMFCDLLApp, CWinApp)
END_MESSAGE_MAP()


// CMFCDLLApp 构造

CMFCDLLApp::CMFCDLLApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 CMFCDLLApp 对象

CMFCDLLApp theApp;


// CMFCDLLApp 初始化

// 定义全局的窗口变量
CMainDialogWnd* PMainDialog;

DWORD WINAPI ShowDialog(LPARAM lpData) {
	// 添加显示窗口的代码
	// 创建对象，划分空间
	PMainDialog = new CMainDialogWnd;
	//DoModal 是以阻塞的方式来运行
	PMainDialog->DoModal();
	// 释放空间
	delete PMainDialog;
	// 在执行完成后，释放掉动态链接库(传递dll 的句柄和执行代码)
	FreeLibraryAndExitThread(theApp.m_hInstance, 1);
	return TRUE;
}


BOOL CMFCDLLApp::InitInstance()
{
	CWinApp::InitInstance();

	// 创建线程函数
	::CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ShowDialog, NULL, NULL, NULL);
	return TRUE;
}
