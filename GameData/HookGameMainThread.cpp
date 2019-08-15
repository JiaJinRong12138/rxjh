// HookGameMainThread.cpp
#include "StructGame.h"
#include "HookGameMainThread.h"
#include "gloab_var.h"
#include "BaseGame.h"

HHOOK g_hhkGame;
const DWORD MyMsgCode = RegisterWindowMessageA("MyMsgCode");
static DWORD ndObjID = 0;
DWORD isLive = -1;
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
				// 使用下标调用动作
				//ptLIst->UseActionByIndex(1);
				DWORD i = tMonList.getData()->getMinDistanceMonIndex();
				ndObjID = tMonList.getData()->tMonList[i].ndID;
				if (ndObjID != 0) {
					tRoleObj.SelObj(ndObjID);
					ptLIst->UseActionByName((char*)("攻击"));
				}
				else {
					// 击怪失败，提示音
					MessageBeep(0);
				}

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
	//[[0x2E64A28 + 4 * 1c8b]+ 3c0]
	if (isLive == 1) {
		// 上一个怪物死亡才能进行下一个怪物的选中
		SendMessageA(GetGameWndHandle(), MyMsgCode, MSG_ACTIONTEST, (LPARAM)lpData);
	}
	else if (isLive == -1)
	{
		// 第一次执行
		DbgPrintMine((char*)("第一次执行"));
		SendMessageA(GetGameWndHandle(), MyMsgCode, MSG_ACTIONTEST, (LPARAM)lpData);
	}
	_try{
		// 获取上一个怪物的生命状态
		isLive = *(DWORD*)(*(DWORD*)(BaseAllObjList + 4 * ndObjID) + 0x3c0);
	}_except(1) {
		DbgPrintMine((char*)("读取内存失败 at isLive = *(DWORD*)(*(DWORD*)(BaseAllObjList + 4 * ndObjID) + 0x3c0);"));
	}
	
	return 0;
}

