#pragma once

// HookGameMainThread.h
#define MSG_USEGOODSFORNAME 1 //ʹ����Ʒ����Ϣ����
#define MSG_TEST 2 // ���Թ�����Ϣ
#define MSG_ACTIONTEST 3 //���Զ���
// �������߳�
DWORD HookMainThread();

// ж�����߳�
DWORD UnHookMainThread();

// ʹ����Ʒ
DWORD msgUseGoodsForName(char* szpName);

// ���Թ���
DWORD msgTest(LPVOID lpData);

// ���Զ���
DWORD testActionMsg(LPVOID lpData);