#pragma once

// HookGameMainThread.h
#define MSG_USEGOODSFORNAME 1 //使用物品的消息种类
#define MSG_TEST 2 // 测试怪物消息
#define MSG_ACTIONTEST 3 //测试动作
// 挂载主线程
DWORD HookMainThread();

// 卸载主线程
DWORD UnHookMainThread();

// 使用物品
DWORD msgUseGoodsForName(char* szpName);

// 测试怪物
DWORD msgTest(LPVOID lpData);

// 测试动作
DWORD testActionMsg(LPVOID lpData);