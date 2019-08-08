#include "StructGame.h"
#include "BaseGame.h"
#include <iostream>
TROLE_PROPERTY* TROLE_PROPERTY::GetData()
{

	// 添加异常处理
	try {

		// +0：人物名字
		szRoleName = (char*)BaseRole;
		// + 80：生命值(红 / HP)
		ndRoleHP = *(DWORD*)(BaseRole + 0x80);
		// + 84：内功值(蓝 / MP)
		ndRoleMP = *(DWORD*)(BaseRole + 0x84);
		// + 88：愤怒值
		ndRoleAnger = *(DWORD*)(BaseRole + 0x88);
		// + 8C：最大生命值
		ndRoleMaxHP = *(DWORD*)(BaseRole + 0x8c);
		// + 90：最大内功值
		ndRoleMaxMP = *(DWORD*)(BaseRole + 0x90);
		// + 94：最大值愤怒值
		nqRoleMaxAnger = *(QWORD*)(BaseRole + 0x94);
		// + 98：当前经验值
		nqRoleExprienceNow = *(QWORD*)(BaseRole + 0x98);
		// + A0：升级到下一级要的经验值
		ndRoleExperienceNext = *(DWORD*)(BaseRole + 0xA0);
		// + 36：名声
		szReputation = (char*)(BaseRole + 0x36);
		// + 34：一字节空间表示等级
		nbClassValue = *(BYTE*)(BaseRole + 0x34);
		// + 35：一字节空间表示 几转
		nbJZ = *(BYTE*)(BaseRole + 0x35);
		// + AC：历练
		ndExprience = *(DWORD*)(BaseRole + 0xac);
		// + C8：攻击
		ndAttack = *(DWORD*)(BaseRole + 0xc8);
		// + CC：防御
		ndDefense = *(DWORD*)(BaseRole + 0xcc);
		// + D4：回避
		ndAvoid = *(DWORD*)(BaseRole + 0xd4);
		// + B0：心
		ndHeart = *(DWORD*)(BaseRole + 0xb0);
		// + B4：气
		ndGas = *(DWORD*)(BaseRole + 0xb4);
		// + B8：体
		ndBody = *(DWORD*)(BaseRole + 0x8c);
		// + BC：魂
		ndSoul = *(DWORD*)(BaseRole + 0xbc);
		// + E4：金币值
		nqMoney = *(QWORD*)(BaseRole + 0xe4);


		for (int i = 0; i < 32; i++) {
			ndQg[i] = *(BYTE*)(BaseRole + 0xf0 + 4 * (i + 1));
		}
	}
	catch (...) {
		// 处理所有的异常
		DbgPrintMine((char*)("%s\r\n"), "读取人物数据异常");
	}
	


	return this;
}

// 获取角色的名称
char* TROLE_PROPERTY::GetRoleName() {
	return GetData()->szRoleName;
}


TGOODSLIST_PROPERTY* TGOODSLIST_PROPERTY::getData()
{

	// 通过获取背包基址对每样物品进行分析

// *物品对象指针 + 0x64 = 物品名字
#define GOODSNAME 0x64
// * 物品对象指针 + 0xf9 = 对物品的描述
#define GOODSINTRO 0xf9
// * 物品对象指针 + 0xC4C = 物品剩余数量
#define GOODSNUM 0xc4c

	// 背包公式： ndBaseAddr + num*4 + 0x43c
	try {
		// 读取背包基址
		DWORD ndBase = *(DWORD*)(BaseBackpack);
		// 第一个物品的地址
		DWORD ndFirstGoodsBase = ndBase + 4 * 0 + 0x43c;
		// 第一个物品的对象
		DWORD ndObj = NULL;
		for (int i = 0; i < nGoodsNum; i++) {
			ndObj = *(DWORD*)(ndFirstGoodsBase + 4 * i); // 取出第i个对象的地址
			if (ndObj == NULL) {
				// 如果读取数据为0===> 背包这一格没有物品
				this->mtGoodsList[i].ndGoodsNum = 0;
				continue;
			}
			// 读取物品的名字
			this->mtGoodsList[i].szGoodsName = (char*)(ndObj + GOODSNAME);
			// 读取物品的介绍
			this->mtGoodsList[i].szGoodsIntro = (char*)(ndObj + GOODSINTRO);
			// 读取物品的剩余数量
			this->mtGoodsList[i].ndGoodsNum = *(DWORD*)(ndObj + GOODSNUM);

		}
	}
	catch (...) {
		// 处理所有异常
		DbgPrintMine((char*)("%s\r\n"), "读取背包数据异常");
		MessageBox(NULL, "读取背包数据异常(StructGame)", "Error", MB_OK);
	}

	return this;
}

// 通过物品下标使用物品
int TGOODSLIST_PROPERTY::UseGoodsForIndex(DWORD ndIndex) {

	try {
		// 使用内联汇编
		__asm {
			mov eax, ndIndex
			push eax
			push 1
			push 0
			// 读取背包地址
			mov ecx, [BaseBackpack]
			mov eax, BaseCall_UseGoodsForIndex
			call eax
		}
	}
	catch (...) {
		DbgPrintMine((char*)("%s\r\n"), "物品使用异常");
	}

	return TRUE;

}

int TGOODSLIST_PROPERTY::UseGoodsForName(char* szGoodsName)
{
	// 查找物品的下标
	DWORD ndIndex = this->GetGoodsIndexByName(szGoodsName);
	if (ndIndex != -1) {
		this->UseGoodsForIndex(ndIndex);
		return TRUE;
	}
	return FALSE;
}

int TGOODSLIST_PROPERTY::GetGoodsIndexByName(char* szGoodsName) {
	// 遍历整个背包，看是否存在该物品
	TGOODSLIST_PROPERTY* g = this->getData();// 初始化背包结构
	for (int i = 0; i < nGoodsNum; i++) {
		// 当物品栏为空，跳到下一个
		if (g->mtGoodsList[i].ndGoodsNum == 0) {
			continue;
		}
		// 比较字符串，判断该物品是否存在
		if (strcmp(szGoodsName, g->mtGoodsList[i].szGoodsName) == 0) {
			return i;
		}
	}

	return -1;
}


HWND GetGameWndHandle() {
	HWND hGame = NULL;
	try
	{
		hGame = *(HWND*)BaseGameWndHandle;
	}
	catch (...)
	{
		return NULL;
	}
	return hGame;
}

#define TYPE 8

TMonseterList* TMonseterList::getData()
{
	DWORD ndObj = NULL;
	//memset(this, 0, sizeof(TMonseterList));
	try
	{
		for (int i = 0; i < MONSETERNUM; i++) {
			ndObj = *(DWORD*)(BaseMonseterList + 4 * i);
			if (ndObj == 0 || *(DWORD*)ndObj+TYPE==0x2E) {
				this->tMonList[i].ndLevel = 0;
				continue;
			}
			// 怪物名字
			this->tMonList[i].szMName = (char*)(ndObj + 0x360);
			// 怪物血量
			this->tMonList[i].ndHp = *(DWORD*)(ndObj + 0x5f4);
			// 怪物等级
			this->tMonList[i].ndLevel = *(DWORD*)(ndObj + 0x5f8);
			// 怪物位置X
			this->tMonList[i].flX = *(float*)(ndObj + 0x1060);
			// 怪物位置Y
			this->tMonList[i].flY = *(float*)(ndObj + 0x1068);
			// 怪物生命状态
			this->tMonList[i].IsDead = *(BOOL*)(ndObj + 0x3c0);
		}
	}
	catch (...)
	{
		// 处理所有的异常
		DbgPrintMine((char*)"读取怪物数据异常");
	}
	return this;
}

BOOL TMonseterList::dbgPrintMsg() {
	for (int i = 0; i < MONSETERNUM; i++) {
		if (tMonList[i].ndLevel == 0) {
			continue;
		}
		DbgPrintMine((char*)("%s,等级：%d级；血量：%d；当前位置X:%f Y:%f；生命状态：%d"),
			tMonList[i].szMName,
			tMonList[i].ndLevel,
			tMonList[i].ndHp,
			tMonList[i].flX,
			tMonList[i].flY,
			tMonList[i].IsDead);
	}
	return TRUE;
}

// DbgPrintMine 打印可变长参数调试信息
void DbgPrintMine(char* pszFromat, ...) {
	// 判断是否在DEBUG 模式下
#ifdef _DEBUG

	// 定义缓冲区
	char szBufFormat[0x1000];
	// 定义格式缓冲区
	char szBufFormatGame[0x1008] = "GameDebug:";

	// 定义list
	va_list argList;
	// 初始化list
	va_start(argList, pszFromat);

	// 获取可变长参数
	vsprintf_s(szBufFormat, pszFromat, argList);

	// 复制字符串
	strcat_s(szBufFormatGame, szBufFormat);

	// 输出调试信息
	OutputDebugStringA(szBufFormatGame);

	// 清除list
	va_end(argList);

#endif // _DEBUG



}

TCActionList* TCActionList::getData()
{
	//dc [[02e3bd58]+ 43c + 4 * 0] + 64
	//+ 64 动作名字

	//[[02e3bd58]+ 43c + 4 * 0] + 54
	//+ 54 调用CALL参数

	DWORD ndFirstObj = 0;
	DWORD ndObj;
	try {
		ndFirstObj = (*(DWORD*)(BaseActionList))+0x43C;
		for (int i = 0; i < ActionNum; i++) {
			ndObj = *(DWORD*)(ndFirstObj + 4 * i);
			if (ndObj == NULL) {
				tList[i].ndActionID = 0;
				continue;
			}
			tList[i].szpName = (char*)(ndObj + 0x64);
			tList[i].ndActionID = *(DWORD*)(ndObj + 0x54);
		}
	}
	catch (...) {
		DbgPrintMine((char*)("内存读取异常"));
	}

	return this;
}

BOOL TCActionList::TestActionMsg()
{
	for (int i = 0; i < ActionNum; i++) {
		if (tList[i].ndActionID == 0) {
			continue;
		}
		DbgPrintMine((char*)("动作名：%s, 动作ID：%X"),
			tList[i].szpName,
			tList[i].ndActionID);

	}
	return TRUE;
}



DWORD getObjByIndex(char* szpName) {
	TCActionList tList;
	TCActionList* ptList = tList.getData();
	for(int i = 0; i < ActionNum; i++) {
		if (strcmp(szpName, ptList->tList[i].szpName) == 0) {
			return i;
		}
	}
	return -1;
}

BOOL UseAction(DWORD ndIndex) {
	TCActionList tList;
	TCActionList* ptList = tList.getData();
	DWORD ndPrarm = ptList->tList[ndIndex].ndActionID;
	try {
		__asm {
			mov ecx, ndPrarm
			push ecx
			mov eax, BaseActionCall
			call eax
		}
	}
	catch (...) {
		DbgPrintMine((char*)("动作使用失败"));
		return FALSE;
	}
	return TRUE;
}

BOOL TCActionList::UseActionByIndex(DWORD ndIndex)
{
	if (UseAction(ndIndex)) {
		MessageBeep(0);
		return TRUE;
	}
	return FALSE;
}

BOOL TCActionList::UseActionByName(char* szpName)
{
	DWORD ndIndex = getObjByIndex(szpName);
	if (ndIndex != -1) {
		if (UseAction(ndIndex)) {
			MessageBeep(0);
			return TRUE;
		}
	}
	return FALSE;
}


