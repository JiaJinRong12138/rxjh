// HookGameMainThread.cpp
#include "StructGame.h"
#include "HookGameMainThread.h"
#include "gloab_var.h"

HHOOK g_hhkGame;
const DWORD MyMsgCode = RegisterWindowMessageA("MyMsgCode");
// 回调函数
LRESULT CALLBACK GameWndProc(
	int nCode,
	WPARAM wParam,
	LPARAM lParam
) {
	CWPSTRUCT* lpArg = (CWPSTRUCT*)lParam;
	if (nCode == HC_ACTION) {
		if (lpArg->hwnd == GetGameWndHandle() && lpArg->message == MyMsgCode) {
			DbgPrintMine((char*)("消息传到	%s\r\n"), lpArg->lParam);
			switch (lpArg->wParam)
			{
			case MSG_USEGOODSFORNAME: {
				TGOODSLIST_PROPERTY* g = goods.getData();
				if (g->UseGoodsForName((char*)lpArg->lParam)) {
					DbgPrintMine((char*)("使用 %s 成功"), lpArg->lParam);
				}
			}; break;
			case MSG_TEST: {
				TMonseterList* ptMonList = tMonList.getData();
				ptMonList->dbgPrintMsg();
			}; break;
			case MSG_ACTIONTEST: {
				TCActionList* ptLIst = tList.getData();
				ptLIst->TestActionMsg();
				//ptLIst->UseActionByIndex(1);
				ptLIst->UseActionByName((char*)("攻击"));
			}; break;
			default:
				break;
			}
			return 1;
		}
	}
	return CallNextHookEx(g_hhkGame, nCode, wParam, lParam);
}
DWORD HookMainThread() {

	HWND hGame = GetGameWndHandle();
	DWORD ndThreadId = GetWindowThreadProcessId(hGame, NULL);
	if (ndThreadId != 0) {
		// 安装钩子
		g_hhkGame = SetWindowsHookEx(WH_CALLWNDPROC, GameWndProc, NULL, ndThreadId);
	}
	return 1;
}


DWORD UnHookMainThread() {
	UnhookWindowsHookEx(g_hhkGame);
	return 1;
}

DWORD msgUseGoodsForName(char* szpName) {
	// 传递消息(句柄、自定义的注册消息、自定义消息类别、消息内容（字符串）)
	SendMessageA(GetGameWndHandle(), MyMsgCode, MSG_USEGOODSFORNAME, (LPARAM)szpName);
	return 1;
}

DWORD msgTest(LPVOID lpData)
{
	SendMessageA(GetGameWndHandle(), MyMsgCode, MSG_TEST, (LPARAM)lpData);
	return 0;
}

DWORD testActionMsg(LPVOID lpData) {
	SendMessageA(GetGameWndHandle(), MyMsgCode, MSG_ACTIONTEST, (LPARAM)lpData);
	return 0;
}

