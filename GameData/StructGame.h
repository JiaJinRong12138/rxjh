#pragma once
#ifndef _STRUCTGAME_H
#define _STRUCTGAME_H

#include <windows.h>

// �������� ==> ��ȡ��Ϸ���
HWND GetGameWndHandle();

// ������Ӻ���
extern void DbgPrintMine(char* pszFromat, ...);

extern DWORD play2PointDistance(float x1, float y1, float x2, float y2);

// ��������Ʒ������
#define nGoodsNum 36
typedef unsigned __int64 QWORD;
// ��Ϸ�ṹ�Լ�ƫ�����Ĺ���

// ��ɫ�ṹ
typedef struct TROLE_PROPERTY {

	// +0����������
	char* szRoleName;
	// + 80������ֵ(�� / HP)
	DWORD ndRoleHP;
	// + 84���ڹ�ֵ(�� / MP)
	DWORD ndRoleMP;
	// + 88����ŭֵ
	DWORD ndRoleAnger;
	// + 8C���������ֵ
	DWORD ndRoleMaxHP;
	// + 90������ڹ�ֵ
	DWORD ndRoleMaxMP;
	// + 94�����ֵ��ŭֵ
	QWORD nqRoleMaxAnger;
	// + 98����ǰ����ֵ
	QWORD nqRoleExprienceNow;
	// + A0����������һ��Ҫ�ľ���ֵ
	DWORD ndRoleExperienceNext;
	// + 36������
	char* szReputation;
	// + 34��һ�ֽڿռ��ʾ�ȼ�
	BYTE nbClassValue;
	// + 35��һ�ֽڿռ��ʾ ��ת
	BYTE nbJZ;
	// + AC������
	DWORD ndExprience;
	// + C8������
	DWORD ndAttack;
	// + CC������
	DWORD ndDefense;
	// + D4���ر�
	DWORD ndAvoid;
	// + B0����
	DWORD ndHeart;
	// + B4����
	DWORD ndGas;
	// + B8����
	DWORD ndBody;
	// + BC����
	DWORD ndSoul;
	// + E4�����ֵ
	QWORD nqMoney;
	// ����
	DWORD ndQg[32];
	// ��ɫ������ַ
	DWORD ndBaseBackPackAddr;

	TROLE_PROPERTY* GetData();

	char* GetRoleName();

}_TROLE_PROPERTY;

// ��Ʒ�ṹ
typedef struct TBACKPACK_GOODS {

	// *��Ʒ����ָ�� + 0x64 = ��Ʒ����
	char* szGoodsName;
	// * ��Ʒ����ָ�� + 0xf9 = ����Ʒ������
	char* szGoodsIntro;
	// * ��Ʒ����ָ�� + 0xC4C = ��Ʒʣ������
	DWORD ndGoodsNum;

} _TBACKPACK_GOODS;

// �����ṹ
typedef struct TGOODSLIST_PROPERTY {
	// �����б�
	_TBACKPACK_GOODS mtGoodsList[nGoodsNum];

	// �����ݵĳ�ʼ��
	TGOODSLIST_PROPERTY* getData();

	// ʹ�ñ�����Ʒ
	int UseGoodsForIndex(DWORD ndIndex);

	// ͨ�����ֲ�ѯ�±꣬���ڷ����±꣬�����ڷ���FALSE
	int GetGoodsIndexByName(char* szGoodsName);

	// ������Ʒ�����ֽ���ʹ��
	int UseGoodsForName(char* szGoodsName);
}_TGOODSLIST_PROPERTY;


// ����ṹ
typedef struct TMonseterObj {
	//+5f4 ����Ѫ��
	DWORD ndHp;
	// �������
	DWORD ndMonType;
	// ����ID
	DWORD ndID;
	//+ 5f8 ����ȼ�
	DWORD ndLevel;
	//+ 360 ��������
	char* szMName;
	//+ 1060 ����λ��X
	float flX;
	//+ 1068 ����λ��Y
	float flY;
	//+ 3C0 ��������״̬ 0�� / 1��
	BOOL IsDead;
	// ������ҵľ���
	DWORD ndDistance;

}_TMonseterObj;

// �����б�
#define MONSETERNUM 10
typedef struct TMonseterList {

	_TMonseterObj tMonList[MONSETERNUM];

	// ��ʼ��
	TMonseterList* getData();

	// ��ӡ��Ϣ
	BOOL dbgPrintMsg();

	// ��ȡ·����С�Ĺֵ��±�
	DWORD getMinDistanceMonIndex();
}_TMonseterList;


// ��������Ľṹ
typedef struct TActionObj {

	// ��������
	char* szpName;
	// ����CALL �Ĳ���
	DWORD ndActionID;

}_TActionObj;

// ������������
#define ActionNum 18
typedef struct TCActionList {

	// ���嶯������
	_TActionObj tList[ActionNum];

	// ��ʼ��
	TCActionList* getData();

	// ��ӡ��Ϣ
	BOOL TestActionMsg();

	// ʹ�ö���ͨ���±�
	BOOL UseActionByIndex(DWORD ndIndex);

	// ʹ�ö���ͨ������
	BOOL UseActionByName(char* szpName);

}_TCActionList;

typedef struct TRoleObj {
	//+8 //��ɫ��ʶ 0x31
	//+ c //�������±�
	//+ 18 //�������
	//+ 1A64 //ѡ�й��� 0xFFFF��ʶδѡ��
	DWORD ndSelIndex;

	// ��ǰX����
	float flXCur;

	// ��ǰY����
	float flYCur;

	//+ 3428 //ѡ��״̬ 1ѡ���Լ�/0δѡ���Լ�

	// ��ʼ��
	TRoleObj* GetData();

	// ѡ�ж���
	BOOL SelObj(DWORD ndIndexForAllObj);

}_TRoleObj;

#endif