// HookGameMainThread.cpp
#include "StructGame.h"
#include "HookGameMainThread.h"
#include "gloab_var.h"

HHOOK g_hhkGame;
const DWORD MyMsgCode = RegisterWindowMessageA("MyMsgCode");
// �ص�����
LRESULT CALLBACK GameWndProc(
	int nCode,
	WPARAM wParam,
	LPARAM lParam
) {
	CWPSTRUCT* lpArg = (CWPSTRUCT*)lParam;
	if (nCode == HC_ACTION) {
		if (lpArg->hwnd == GetGameWndHandle() && lpArg->message == MyMsgCode) {
			DbgPrintMine((char*)("��Ϣ����	%s\r\n"), lpArg->lParam);
			switch (lpArg->wParam)
			{
			case MSG_USEGOODSFORNAME: {
				TGOODSLIST_PROPERTY* g = goods.getData();
				if (g->UseGoodsForName((char*)lpArg->lParam)) {
					DbgPrintMine((char*)("ʹ�� %s �ɹ�"), lpArg->lParam);
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
				ptLIst->UseActionByName((char*)("����"));
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
		// ��װ����
		g_hhkGame = SetWindowsHookEx(WH_CALLWNDPROC, GameWndProc, NULL, ndThreadId);
	}
	return 1;
}


DWORD UnHookMainThread() {
	UnhookWindowsHookEx(g_hhkGame);
	return 1;
}

DWORD msgUseGoodsForName(char* szpName) {
	// ������Ϣ(������Զ����ע����Ϣ���Զ�����Ϣ�����Ϣ���ݣ��ַ�����)
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

