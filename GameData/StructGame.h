#pragma once
#include <windows.h>

// 声明函数 ==> 获取游戏句柄
HWND GetGameWndHandle();

// 定义外接函数
extern void DbgPrintMine(char* pszFromat, ...);

// 背包内物品格数量
#define nGoodsNum 36
typedef unsigned __int64 QWORD;
// 游戏结构以及偏移量的管理

// 角色结构
typedef struct TROLE_PROPERTY {

	// +0：人物名字
	char* szRoleName;
	// + 80：生命值(红 / HP)
	DWORD ndRoleHP;
	// + 84：内功值(蓝 / MP)
	DWORD ndRoleMP;
	// + 88：愤怒值
	DWORD ndRoleAnger;
	// + 8C：最大生命值
	DWORD ndRoleMaxHP;
	// + 90：最大内功值
	DWORD ndRoleMaxMP;
	// + 94：最大值愤怒值
	QWORD nqRoleMaxAnger;
	// + 98：当前经验值
	QWORD nqRoleExprienceNow;
	// + A0：升级到下一级要的经验值
	DWORD ndRoleExperienceNext;
	// + 36：名声
	char* szReputation;
	// + 34：一字节空间表示等级
	BYTE nbClassValue;
	// + 35：一字节空间表示 几转
	BYTE nbJZ;
	// + AC：历练
	DWORD ndExprience;
	// + C8：攻击
	DWORD ndAttack;
	// + CC：防御
	DWORD ndDefense;
	// + D4：回避
	DWORD ndAvoid;
	// + B0：心
	DWORD ndHeart;
	// + B4：气
	DWORD ndGas;
	// + B8：体
	DWORD ndBody;
	// + BC：魂
	DWORD ndSoul;
	// + E4：金币值
	QWORD nqMoney;
	// 气功
	DWORD ndQg[32];
	// 角色背包基址
	DWORD ndBaseBackPackAddr;

	TROLE_PROPERTY* GetData();

	char* GetRoleName();

}_TROLE_PROPERTY;

// 物品结构
typedef struct TBACKPACK_GOODS {

	// *物品对象指针 + 0x64 = 物品名字
	char* szGoodsName;
	// * 物品对象指针 + 0xf9 = 对物品的描述
	char* szGoodsIntro;
	// * 物品对象指针 + 0xC4C = 物品剩余数量
	DWORD ndGoodsNum;

} _TBACKPACK_GOODS;

// 背包结构
typedef struct TGOODSLIST_PROPERTY {
	// 背包列表
	_TBACKPACK_GOODS mtGoodsList[nGoodsNum];

	// 对数据的初始化
	TGOODSLIST_PROPERTY* getData();

	// 使用背包物品
	int UseGoodsForIndex(DWORD ndIndex);

	// 通过名字查询下标，存在返回下标，不存在返回FALSE
	int GetGoodsIndexByName(char* szGoodsName);

	// 根据物品的名字进行使用
	int UseGoodsForName(char* szGoodsName);
}_TGOODSLIST_PROPERTY;


// 怪物结构
typedef struct TMonseterObj {
	//+5f4 怪物血量
	DWORD ndHp;
	//+ 5f8 怪物等级
	DWORD ndLevel;
	//+ 360 怪物名字
	char* szMName;
	//+ 1060 怪物位置X
	float flX;
	//+ 1068 怪物位置Y
	float flY;
	//+ 3C0 怪物生命状态 0活 / 1死
	BOOL IsDead;
}_TMonseterObj;

// 怪物列表
#define MONSETERNUM 10
typedef struct TMonseterList {

	_TMonseterObj tMonList[MONSETERNUM];

	// 初始化
	TMonseterList* getData();

	// 打印信息
	BOOL dbgPrintMsg();
}_TMonseterList;


// 动作对象的结构
typedef struct TActionObj {

	// 对象名字
	char* szpName;
	// 调用CALL 的参数
	DWORD ndActionID;

}_TActionObj;

// 动作对象数组
#define ActionNum 18
typedef struct TCActionList {

	// 定义动作数组
	_TActionObj tList[ActionNum];

	// 初始化
	TCActionList* getData();

	// 打印信息
	BOOL TestActionMsg();

	// 使用动作通过下标
	BOOL UseActionByIndex(DWORD ndIndex);

	// 使用动作通过名字
	BOOL UseActionByName(char* szpName);

}_TCActionList;