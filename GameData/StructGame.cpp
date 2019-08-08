#include "StructGame.h"
#include "BaseGame.h"
#include <iostream>
TROLE_PROPERTY* TROLE_PROPERTY::GetData()
{

	// ����쳣����
	try {

		// +0����������
		szRoleName = (char*)BaseRole;
		// + 80������ֵ(�� / HP)
		ndRoleHP = *(DWORD*)(BaseRole + 0x80);
		// + 84���ڹ�ֵ(�� / MP)
		ndRoleMP = *(DWORD*)(BaseRole + 0x84);
		// + 88����ŭֵ
		ndRoleAnger = *(DWORD*)(BaseRole + 0x88);
		// + 8C���������ֵ
		ndRoleMaxHP = *(DWORD*)(BaseRole + 0x8c);
		// + 90������ڹ�ֵ
		ndRoleMaxMP = *(DWORD*)(BaseRole + 0x90);
		// + 94�����ֵ��ŭֵ
		nqRoleMaxAnger = *(QWORD*)(BaseRole + 0x94);
		// + 98����ǰ����ֵ
		nqRoleExprienceNow = *(QWORD*)(BaseRole + 0x98);
		// + A0����������һ��Ҫ�ľ���ֵ
		ndRoleExperienceNext = *(DWORD*)(BaseRole + 0xA0);
		// + 36������
		szReputation = (char*)(BaseRole + 0x36);
		// + 34��һ�ֽڿռ��ʾ�ȼ�
		nbClassValue = *(BYTE*)(BaseRole + 0x34);
		// + 35��һ�ֽڿռ��ʾ ��ת
		nbJZ = *(BYTE*)(BaseRole + 0x35);
		// + AC������
		ndExprience = *(DWORD*)(BaseRole + 0xac);
		// + C8������
		ndAttack = *(DWORD*)(BaseRole + 0xc8);
		// + CC������
		ndDefense = *(DWORD*)(BaseRole + 0xcc);
		// + D4���ر�
		ndAvoid = *(DWORD*)(BaseRole + 0xd4);
		// + B0����
		ndHeart = *(DWORD*)(BaseRole + 0xb0);
		// + B4����
		ndGas = *(DWORD*)(BaseRole + 0xb4);
		// + B8����
		ndBody = *(DWORD*)(BaseRole + 0x8c);
		// + BC����
		ndSoul = *(DWORD*)(BaseRole + 0xbc);
		// + E4�����ֵ
		nqMoney = *(QWORD*)(BaseRole + 0xe4);


		for (int i = 0; i < 32; i++) {
			ndQg[i] = *(BYTE*)(BaseRole + 0xf0 + 4 * (i + 1));
		}
	}
	catch (...) {
		// �������е��쳣
		DbgPrintMine((char*)("%s\r\n"), "��ȡ���������쳣");
	}
	


	return this;
}

// ��ȡ��ɫ������
char* TROLE_PROPERTY::GetRoleName() {
	return GetData()->szRoleName;
}


TGOODSLIST_PROPERTY* TGOODSLIST_PROPERTY::getData()
{

	// ͨ����ȡ������ַ��ÿ����Ʒ���з���

// *��Ʒ����ָ�� + 0x64 = ��Ʒ����
#define GOODSNAME 0x64
// * ��Ʒ����ָ�� + 0xf9 = ����Ʒ������
#define GOODSINTRO 0xf9
// * ��Ʒ����ָ�� + 0xC4C = ��Ʒʣ������
#define GOODSNUM 0xc4c

	// ������ʽ�� ndBaseAddr + num*4 + 0x43c
	try {
		// ��ȡ������ַ
		DWORD ndBase = *(DWORD*)(BaseBackpack);
		// ��һ����Ʒ�ĵ�ַ
		DWORD ndFirstGoodsBase = ndBase + 4 * 0 + 0x43c;
		// ��һ����Ʒ�Ķ���
		DWORD ndObj = NULL;
		for (int i = 0; i < nGoodsNum; i++) {
			ndObj = *(DWORD*)(ndFirstGoodsBase + 4 * i); // ȡ����i������ĵ�ַ
			if (ndObj == NULL) {
				// �����ȡ����Ϊ0===> ������һ��û����Ʒ
				this->mtGoodsList[i].ndGoodsNum = 0;
				continue;
			}
			// ��ȡ��Ʒ������
			this->mtGoodsList[i].szGoodsName = (char*)(ndObj + GOODSNAME);
			// ��ȡ��Ʒ�Ľ���
			this->mtGoodsList[i].szGoodsIntro = (char*)(ndObj + GOODSINTRO);
			// ��ȡ��Ʒ��ʣ������
			this->mtGoodsList[i].ndGoodsNum = *(DWORD*)(ndObj + GOODSNUM);

		}
	}
	catch (...) {
		// ���������쳣
		DbgPrintMine((char*)("%s\r\n"), "��ȡ���������쳣");
		MessageBox(NULL, "��ȡ���������쳣(StructGame)", "Error", MB_OK);
	}

	return this;
}

// ͨ����Ʒ�±�ʹ����Ʒ
int TGOODSLIST_PROPERTY::UseGoodsForIndex(DWORD ndIndex) {

	try {
		// ʹ���������
		__asm {
			mov eax, ndIndex
			push eax
			push 1
			push 0
			// ��ȡ������ַ
			mov ecx, [BaseBackpack]
			mov eax, BaseCall_UseGoodsForIndex
			call eax
		}
	}
	catch (...) {
		DbgPrintMine((char*)("%s\r\n"), "��Ʒʹ���쳣");
	}

	return TRUE;

}

int TGOODSLIST_PROPERTY::UseGoodsForName(char* szGoodsName)
{
	// ������Ʒ���±�
	DWORD ndIndex = this->GetGoodsIndexByName(szGoodsName);
	if (ndIndex != -1) {
		this->UseGoodsForIndex(ndIndex);
		return TRUE;
	}
	return FALSE;
}

int TGOODSLIST_PROPERTY::GetGoodsIndexByName(char* szGoodsName) {
	// �����������������Ƿ���ڸ���Ʒ
	TGOODSLIST_PROPERTY* g = this->getData();// ��ʼ�������ṹ
	for (int i = 0; i < nGoodsNum; i++) {
		// ����Ʒ��Ϊ�գ�������һ��
		if (g->mtGoodsList[i].ndGoodsNum == 0) {
			continue;
		}
		// �Ƚ��ַ������жϸ���Ʒ�Ƿ����
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
			// ��������
			this->tMonList[i].szMName = (char*)(ndObj + 0x360);
			// ����Ѫ��
			this->tMonList[i].ndHp = *(DWORD*)(ndObj + 0x5f4);
			// ����ȼ�
			this->tMonList[i].ndLevel = *(DWORD*)(ndObj + 0x5f8);
			// ����λ��X
			this->tMonList[i].flX = *(float*)(ndObj + 0x1060);
			// ����λ��Y
			this->tMonList[i].flY = *(float*)(ndObj + 0x1068);
			// ��������״̬
			this->tMonList[i].IsDead = *(BOOL*)(ndObj + 0x3c0);
		}
	}
	catch (...)
	{
		// �������е��쳣
		DbgPrintMine((char*)"��ȡ���������쳣");
	}
	return this;
}

BOOL TMonseterList::dbgPrintMsg() {
	for (int i = 0; i < MONSETERNUM; i++) {
		if (tMonList[i].ndLevel == 0) {
			continue;
		}
		DbgPrintMine((char*)("%s,�ȼ���%d����Ѫ����%d����ǰλ��X:%f Y:%f������״̬��%d"),
			tMonList[i].szMName,
			tMonList[i].ndLevel,
			tMonList[i].ndHp,
			tMonList[i].flX,
			tMonList[i].flY,
			tMonList[i].IsDead);
	}
	return TRUE;
}

// DbgPrintMine ��ӡ�ɱ䳤����������Ϣ
void DbgPrintMine(char* pszFromat, ...) {
	// �ж��Ƿ���DEBUG ģʽ��
#ifdef _DEBUG

	// ���建����
	char szBufFormat[0x1000];
	// �����ʽ������
	char szBufFormatGame[0x1008] = "GameDebug:";

	// ����list
	va_list argList;
	// ��ʼ��list
	va_start(argList, pszFromat);

	// ��ȡ�ɱ䳤����
	vsprintf_s(szBufFormat, pszFromat, argList);

	// �����ַ���
	strcat_s(szBufFormatGame, szBufFormat);

	// ���������Ϣ
	OutputDebugStringA(szBufFormatGame);

	// ���list
	va_end(argList);

#endif // _DEBUG



}

TCActionList* TCActionList::getData()
{
	//dc [[02e3bd58]+ 43c + 4 * 0] + 64
	//+ 64 ��������

	//[[02e3bd58]+ 43c + 4 * 0] + 54
	//+ 54 ����CALL����

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
		DbgPrintMine((char*)("�ڴ��ȡ�쳣"));
	}

	return this;
}

BOOL TCActionList::TestActionMsg()
{
	for (int i = 0; i < ActionNum; i++) {
		if (tList[i].ndActionID == 0) {
			continue;
		}
		DbgPrintMine((char*)("��������%s, ����ID��%X"),
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
		DbgPrintMine((char*)("����ʹ��ʧ��"));
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


