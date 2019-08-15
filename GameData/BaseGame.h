#pragma once

// 游戏人物的基址
#define BaseRole 0x02C176D8

// 游戏背包的基址存放地址
#define BaseBackpack 0x02E3C3E4

// 添加背包物品使用CALL 的地址 ===> 通过背包物品下标进行物品的使用
#define BaseCall_UseGoodsForIndex 0x008384F0

// 游戏主窗口基址
#define BaseGameWndHandle 0x01195F88

// 怪物列表基址
#define BaseMonseterList 0x427FBA0  

// 人物动作基址
#define BaseActionList 0x02E3CD58

// 人物动作使用的CALL 的基址
#define BaseActionCall 0x007139E0

// 角色对象的基址
#define BaseRoleObj 0x427FBA0

// 所有对象的数组 [0*4+2E63A28]
#define BaseAllObjList 0x2E64A28
