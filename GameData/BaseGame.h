#pragma once

// 游戏人物的基址
#define BaseRole 0x02C166D8

// 游戏背包的基址存放地址
#define BaseBackpack 0x02E3B3E4

// 添加背包物品使用CALL 的地址 ===> 通过背包物品下标进行物品的使用
#define BaseCall_UseGoodsForIndex 0x00838480

// 游戏主窗口基址
#define BaseGameWndHandle 0x01195318

// 怪物列表基址
#define BaseMonseterList 0x427EBA4

// 人物动作基址
#define BaseActionList 0x02e3bd58

// 人物动作使用的CALL 的基址
#define BaseActionCall 0x00713970
