# rxjh
热血江湖外挂
# 外挂编写（一）

## 学习案例：热血江湖

## 使用环境：[Visual Studio 2019](https://visualstudio.microsoft.com/zh-hans/vs/)

# 学习使用的工具：[逆向工具集](https://github.com/JiaJinRong12138/reverseTools)

## 注：更新日志：

- 1.0：最新版人物信息基址：0x02C166D8；最新版背包存放基址：0x02E3B3E4
- 2.0：最新版物品使用call 为0x00838480
- 3.0：
  - 人物基址：0x02C176D8‬；
  - 背包存放基址：0x02E3C3E4；
  - 游戏主窗口基址：0x01195F88；
  - 使用物品的CALL：0x008384F0；
  - 人物动作基址：0x02E3CD58；
  - 动作使用的CALL：0x007139E0；
  - 所有对象数组：0x2E64A28；
  - 怪物基址：0x427FBA0
  - 角色对象基址：0x427FBA0

## 人物属性数据分析

### 注：

- 一般游戏开发的时候，相关的数据都是放到一个结构或者是一个类中，那么这些数据的内存地址相距的比较近；
- 一般内存地址使用CE工具逆向出来后表示为Client.exe+278A75C，表示软件地址加上偏移量为其基址

### HP值和MP值

- 生命值PH: Client.exe+278A75C =  02B8A758
- 内功值MH: Client.exe+278A75C = 02B8A75C

### 金币值

- 进行商品的买卖实现金币值的变动
- 基址：Client.exe+278A7BC = 02B8A7BC

### 其他人物属性分析

- OD软件的使用
  - 使用dd 02B8A758 查找到人物属性基址块
- 人物属性值以及对应的内存地址
- 基址 02B8A6D8
  - +0：人物名字
  - +80：生命值(红/HP)
  - +84：内功值(蓝/MP)
  - +88：愤怒值 
  - +8C：最大生命值
  - +90：最大内功值
  - +94：最大值愤怒值
  - +98：当前经验值
  - +A0：升级到下一级要的经验值
  - 势力 
  - +36：名声
  - +34：一字节空间表示等级
  - +35：一字节空间表示 几转
  - +AC：历练                             
  - 制造  
  - 熟练度  
  - 灵兽持有  
  - 精力
  - +C8：攻击                             
  - 武器命中                      
  - +CC：防御                              
  - 武器防御                      
  - +D0：命中                                    
  - 对人战斗                             
  - +D4：回避                              
  - 对怪攻击                      
  - 武功回避                      
  - 对怪防御                       
  - +B0：心                                  
  - +B4：气                                  
  - +B8：体                                  
  - +BC：魂         
  - +E4：金币值
- 气功值分析
  - 气功点数：基址 + F0 = 02b8a7c8
  - 第num个气功的点数：（一个字节）02B8A6D8+0f0+4*num
  - 可能是第num个气功的ID(没有就为0)：02B8A6D8+0f0+4*num+2

## 通过注入游戏进程读取游戏内数据

- 注入DLL

  - 创建MFC DLL

    ![创建MFC_DLL](https://i.loli.net/2019/07/11/5d26ffb5af90c21949.png)

    ![](https://i.loli.net/2019/07/11/5d270041a363831087.png)

    ![](https://i.loli.net/2019/07/11/5d27010a5c79033592.png)

- 在添加窗口后，需要进行配置，才能在动态链接库注入后显示窗口

  - 为窗口添加Class

    ![](https://i.loli.net/2019/07/11/5d27076e05b7953340.png)

  - 修改MFC_DLL.cpp的代码

    - ```C++
      // MFC_DLL.cpp: 定义 DLL 的初始化例程。
      //
      
      #include "pch.h"
      #include "framework.h"
      #include "MFC_DLL.h"
      
      // 包含含有主窗口的class1的头文件
      #include "CMainDialogWnd.h"
      
      
      #ifdef _DEBUG
      #define new DEBUG_NEW
      #endif
      
      
      BEGIN_MESSAGE_MAP(CMFCDLLApp, CWinApp)
      END_MESSAGE_MAP()
      
      
      // CMFCDLLApp 构造
      
      CMFCDLLApp::CMFCDLLApp()
      {
      	// TODO:  在此处添加构造代码，
      	// 将所有重要的初始化放置在 InitInstance 中
      }
      
      
      // 唯一的 CMFCDLLApp 对象
      
      CMFCDLLApp theApp;
      
      
      // CMFCDLLApp 初始化
      
      // 定义全局的窗口变量
      CMainDialogWnd* PMainDialog;
      
      BOOL CMFCDLLApp::InitInstance()
      {
      	CWinApp::InitInstance();
      
      	// 添加显示窗口的代码
      	// 创建对象，划分空间
      	PMainDialog = new CMainDialogWnd;
      	//DoModal 是以阻塞的方式来运行
      	PMainDialog->DoModal();
      	// 释放空间
      	delete PMainDialog;
      	return TRUE;
      }
      
      ```

  - 使用[注入工具](<https://github.com/JiaJinRong12138/reverseTools/blob/master/README.md>)讲编译生成的DLL 注入到游戏进程中

    ![](https://i.loli.net/2019/07/11/5d2707df083dc26429.png)

    - 注：DoModal() 函数是以阻塞的方式去执行的，所以会造成线程阻塞

    - 解决方式：将DoModal() 放到新的线程去执行

      ![](https://i.loli.net/2019/07/11/5d270c58f2d0b26607.png)

    - 实现窗口关闭后自动释放DLL

      ![](https://i.loli.net/2019/07/11/5d2710855d47840496.png)

  #### 使用代码实现动态连接库的注入

  - 使用到的windows API
    - HWND FindWindow(lpClassName, lpWindowNAme) 
      - 通过类名指针或窗口名指针获取窗口句柄
    - DWORD GetWindowThreadProcessId(hwnd(窗口句柄), lpdwProcessId) 
      - 获取窗口线程句柄的ID(lpdwProcessId)
    - HANDLE WINAPI OpenProcess(dwDesiredAccess(访问权限), bInheritHandle, dwProcessId)
      - 开启并创建一个本地进程
    - LPVOID WINAPI VirtualAllocEx(hProcess, lpAddress, dwSize, flAllocationType, flProtect)
      - 分配内存空间
    - BOOL WINAPI WriteProcessMemory(hProcess, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesWritten)
      - 向内存中写入数据
    - HANDLE WINAPI CreateRemoteThread(hProcess, lpThreadAttributes, dwStackSize, lpStartAddress, lpParameter, dwCreationFlags, lpThreadId)
      - 为进程创建一个线程
    - DWORD WINAPI WaitForSingleObject(hHandle, dwMilliseconds)
      - 等待单个对象执行后再进行操作
    - BOOL WINAPI CloseHandle(hObject)
      - 关闭句柄
    - BOOL WINAPI VirtualFreeEx(hProcess, lpAddress, dwSize, dwFreeType)
      - 释放内存空间
    - BOOL WINAPI CloseHandle(hObject)
      - 关闭句柄



```
#### 代码
```

```c++
#include <iostream>
#include <windows.h>
#define GameClassName "D3D Window"
#define DllPath "D:\\c_work\\MFC_DLL\\Debug\\MFC_DLL.dll"

using namespace std;


void InjectDll() {

	DWORD pid = 0;
	HANDLE hProcess = NULL;
	LPDWORD lpAddr = NULL; // 获取远程分配成功的地址
	DWORD size = NULL;
	HANDLE threadHandle = NULL;

	// 获取游戏窗口句柄
	HWND GameH = FindWindow((LPCTSTR)GameClassName, NULL);
	if (GameH != 0) {
		//句柄获取成功
		// 获取进程PID

		GetWindowThreadProcessId(GameH, &pid);
		if (pid != 0) {
			// PID 获取成功
			// 获取进程句柄
			// 开启所以权限打开进程

			hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE ,pid);
			if (hProcess != NULL) {
				// 打开进程成功
				// 分配内存空间，写入动态链接库的全路径名
				//D:\\c_work\\MFC_DLL\\Debug\\MFC_DLL.dll

				lpAddr = (LPDWORD)VirtualAllocEx(hProcess, NULL, 256, MEM_COMMIT, PAGE_READWRITE);
				if (lpAddr != NULL) {
					// 地址分配成功, 写入DLL 的全路径

					WriteProcessMemory(hProcess, lpAddr, DllPath, strlen(DllPath) + 1, &size);
					if (size >= strlen(DllPath)) {
						threadHandle = CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryA, lpAddr, NULL, NULL);
						// 等待注入DLL 的线程执行完再执行下一步(等待的进程句柄， 等到多少毫秒)
						WaitForSingleObject(threadHandle, 0xFFFFFFFF);
						// 关闭线程
						CloseHandle(threadHandle);
						// 释放进程
						VirtualFreeEx(hProcess, lpAddr, 256, MEM_DECOMMIT);
						// 关闭句柄
						CloseHandle(hProcess);
						
						
						// 清除内存数据

					}
					else {
						cout << "写入DLL 失败" << endl;
					}
				}
				else {
					cout << "地址分配失败" << endl;
				}

			}
			else {
				cout << "打开进程失败" << endl;
			}
		}
		else {
			cout << "获取PID 失败" << endl;
		}
	}
	else {
		cout << "获取窗口句柄失败" << endl;
	}

}

int main()
{
    
	// 添加注入DLL 代码
	cout << "开始注入DLL" << endl;
	InjectDll();
	cout << "注入DLL结束" << endl;

}


```

- 步骤
  1. 获取窗口对应的进程的PID
  2. 根据PID 获取进程
  3. 获取游戏进程的PID
  4. 根据PID 获取进程
  5. 在目标进程分配内存空间，方便写入DLL 全路径
  6. 将DLL 全路径写入到目标进程
  7. 远程注入DLL
  8. 等待目标进程执行完成
  9. 释放进程空间
  10. 关闭线程句柄

### 整合游戏内数据

- 整数
  - QWORD 类型变量      nq前缀 //8字节 无符号整数 不能表示负数
  - DWORD 类型变量      nd前缀 //4字节 无符号整数 不能表示负数
  - WORD  类型变量      nw前缀 //2字节 无符号整数 不能表示负数
  - BYTE  类型变量      nb前缀 //1字节 无符号整数 不能表示负数
  - int   带符号类型    ni前缀 //4字节 带符号整数 可表示正负数
  - __int64  带符号整型 ni64_  //8字节 带符号整数 不能表示负数
  - UINT 类型变量       ui前缀 // 无符号整数 一般是4字节
  - //浮点数
  - float  单精度浮点数  fl前缀
  - double 双精度浮点数  fd前缀
- 字符串
  - char*和char []  sz前缀 //PCHAR szp
- CString         str前缀
- 结构名          T开头全大写
- 类名            C开头单词首字大写

#### 整合游戏数据步骤

1. 新建解决方案GameData

2. 创建头文件BaseGame.h和 StructGame.h

   ------

   - BaseGame.h

   ```c++
   #pragma once
   
   // 游戏人物的基址
   #define BaseRole 0x02B8A6D8
   
   ```

   ------

   - StructGame.h

   ```c++
   #pragma once
   #include <windows.h>
   typedef unsigned __int64 QWORD;
   // 游戏结构以及偏移量的管理
   
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
   
   	TROLE_PROPERTY* GetData();
   
   	char* GetRoleName();
   
   }_TROLE_PROPERTY;
   
   ```

   

3. 创建源文件StructGame.cpp

   ------

   - StructGame.cpp

   ```c++
   #include "StructGame.h"
   #include "BaseGame.h"
   TROLE_PROPERTY* TROLE_PROPERTY::GetData()
   {
   
   	// 添加异常处理
   	try {
   		// +0：人物名字
   		szRoleName = (char*)BaseRole;
   		// + 80：生命值(红 / HP)
   		ndRoleHP = (DWORD)(BaseRole + 0x80);
   		// + 84：内功值(蓝 / MP)
   		ndRoleMP = (DWORD)(BaseRole + 0x84);
   		// + 88：愤怒值
   		ndRoleAnger = (DWORD)(BaseRole + 0x88);
   		// + 8C：最大生命值
   		ndRoleMaxHP = (DWORD)(BaseRole + 0x8c);
   		// + 90：最大内功值
   		ndRoleMaxMP = (DWORD)(BaseRole + 0x90);
   		// + 94：最大值愤怒值
   		nqRoleMaxAnger = (QWORD)(BaseRole + 0x94);
   		// + 98：当前经验值
   		nqRoleExprienceNow = (QWORD)(BaseRole + 0x98);
   		// + A0：升级到下一级要的经验值
   		ndRoleExperienceNext = (DWORD)(BaseRole + 0xA0);
   		// + 36：名声
   		szReputation = (char*)(BaseRole + 0x36);
   		// + 34：一字节空间表示等级
   		nbClassValue = *(BYTE*)(BaseRole + 0x34);
   		// + 35：一字节空间表示 几转
   		nbJZ = *(BYTE*)(BaseRole + 0x35);
   		// + AC：历练
   		ndExprience = (DWORD)(BaseRole + 0xac);
   		// + C8：攻击
   		ndAttack = (DWORD)(BaseRole + 0xc8);
   		// + CC：防御
   		ndDefense = (DWORD)(BaseRole + 0xcc);
   		// + D4：回避
   		ndAvoid = (DWORD)(BaseRole + 0xd4);
   		// + B0：心
   		ndHeart = (DWORD)(BaseRole + 0xb0);
   		// + B4：气
   		ndGas = (DWORD)(BaseRole + 0xb4);
   		// + B8：体
   		ndBody = (DWORD)(BaseRole + 0x8c);
   		// + BC：魂
   		ndSoul = (DWORD)(BaseRole + 0xbc);
   		// + E4：金币值
   		nqMoney = (QWORD)(BaseRole + 0xe4);
   
   		for (int i = 0; i < 32; i++) {
   			ndQg[i] = *(BYTE*)(BaseRole + 0xf0 + 4 * (i + 1));
   		}
   	}
   	catch (...) {
   		// 处理所有的异常
   		OutputDebugStringA("读取人物数据异常\r\n");
   	}
   	
   
   
   	return this;
   }
   
   // 获取角色的名称、
   char* TROLE_PROPERTY::GetRoleName() {
   	return GetData()->szRoleName;
   }
   
   ```

   

### VS2019 导入静态链接库（lib）、配置输出路径的方式

- 配置链接库路径

  - 配置 附加包含目录

    ![](https://i.loli.net/2019/07/12/5d282a710f95f93418.png)

  - 配置 添加库目录

    ![](https://i.loli.net/2019/07/12/5d282aa5ca58875840.png)

- 配置编译输出路径

  - 修改输出目录

    ![](https://i.loli.net/2019/07/12/5d282b31d492535906.png)



### 游戏中的物品使用

- 在游戏中，对应的物品都会有一个结构/类，包含了物品的一些信息
- 使用物品实际上调用了应该CALL

##### 以金疮药为例

- 寻CALL 的过程

  - 使用CE工具找到对象地址指针

  - 去查看访问改指针的地址

  - 使用OD 工具对这些地址进行动态调试

  - 远程注入代码（使用金疮药）

  - ```汇编
    push 1
    push 1
    push 0
    mov ecx, 21DF06D0
    call 00838470
    ```

##### 背包数据的分析

- 背包在游戏中一般会写成应该结构体/类来存放物品对象
- 物品对象在背包中使用数组的形式存在
- 汇编中数组的访问方式一般是 **数组基址 + 4 * i**（' i '为数组下标）
- 查找背包数组基址：
  - 找到背包的物品格
  - 反复讲里面的物品拿出/放入
  - 使用CE工具进程分析
- **结果**
  - 存放背包基址的内存空间：0x02DAF3E4
  - 第num 个格子的数据获取
    - ***背包基址+num\*4+0x43C**
    - 注：0x43 是偏移量
    - 物品对象指针 + 0x64 = 物品名字
    - 物品对象指针 + 0xf9 = 对物品的描述
    - 物品对象指针 + 0xC4C = 物品剩余数量



### 封装背包数据

#### 封装背包结构体

```c++
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
	_TBACKPACK_GOODS mtGoodsList[nGoodsNum];

	// 对数据的初始化
	TGOODSLIST_PROPERTY* getData();
}_TGOODSLIST_PROPERTY;
```

#### 实现初始化方法（getDate()）

```c++
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
		OutputDebugStringA("读取背包数据异常\r\n");
		MessageBox(NULL, "读取背包数据异常(StructGame)", "Error", MB_OK);
	}

	return this;
}


```

#### 调试调用

```c++
void CMainDialogWnd::OnBnClickedButton1()
{
	TROLE_PROPERTY role;
	TROLE_PROPERTY* r = role.GetData();
	TGOODSLIST_PROPERTY goods;

	TRACE("GameDebug:我的调试信息\r\n");
	TRACE("GameDebug: 人物名=%s\r\n", r->GetRoleName());
	TRACE("GameDebug: 人物等级=%d\r\n", r->nbClassValue);
	TRACE("GameDebug: 人物名声=%s\r\n", r->szReputation);
	TRACE("GameDebug: 人物血量HP=%d//%d\r\n", r->ndRoleHP, r->ndRoleMaxHP);
	TRACE("GameDebug: 人物内功MP=%d//%d\r\n", r->ndRoleMP, r->ndRoleMaxMP);
	TRACE("GameDebug: 人物愤怒值=%d\r\n", r->ndRoleAnger);
	TRACE("GameDebug: 人物金币=%d\r\n", r->nqMoney);
	TGOODSLIST_PROPERTY* g = goods.getData();
	try {
		for (int i = 0; i < nGoodsNum; i++) {
			if (g->mtGoodsList[i].ndGoodsNum == 0) {
				continue;
			}
			TRACE("GameDebug: 人物第%d格数据：%s\r%s\r%d\r\n", i,
				g->mtGoodsList[i].szGoodsName,
				g->mtGoodsList[i].szGoodsIntro,
				g->mtGoodsList[i].ndGoodsNum
			);
		}
	}
	catch (...) {
		MessageBox(TEXT("读取背包数据异常(Dialog)"), TEXT("Error"), MB_OK);
	}


	// 进行数据修改
}


```

#### 文件结构

![](https://i.loli.net/2019/07/23/5d3707690948b56906.png)

## 背包物品的使用

### 之前Call 的分析

```
push 背包物品下标
push 1
push 0
mov ecx, 背包基址
call 00838470

```

#### 封装函数

```
UseGoods(char* szGoodsName)
{
	// 若存在则使用它
	return 1；
}

```

### 背包物品使用代码

- 定义基址

```c++
// 添加背包物品使用CALL 的地址 ===> 通过背包物品下标进行物品的使用
#define BaseCall_UseGoodsForIndex 0x00838470

```

- 定义结构

```c++
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

```

- 实现方法

```c++
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
		OutputDebugStringA("物品使用异常");
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
		// 比较字符串，判断该物品是否存在
		if (strcmp(szGoodsName, g->mtGoodsList[i].szGoodsName) == 0) {
			return i;
		}
	}

	return -1;
}


```

- 调用方法，实现物品的使用

```c++
if (g->UseGoodsForName("回城符(泫勃派)")) {
		TRACE("GameDebug: 使用 回城符(泫勃派) 成功");
	}


```

- 文件结构

  ![](https://i.loli.net/2019/07/24/5d38542dcc65255168.png)

## 编写自定义的DbgPrintMine方法用于打印格式化调式信息

```c++
// DbgPrintMine.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <windows.h>
using namespace std;

// 定义变参函数
void DbgPrintMine(char* pszFormat, ...) {
#ifdef _DEBUG
	// 如果在DEBUG 版本下才执行以下代码
	// 定义list
	va_list argList;
	// 初始化list
	va_start(argList, pszFormat);
	// 定义字符串缓冲区
	char szBufFormat[0x1000];

	// 定义调试前缀
	char szBufFormat_Game[0x1008] = "Game:";


	// 获取参数 va_arg(list, paramType)
	/*int i = va_arg(argList, int);
	int j = va_arg(argList, int);
	char* szK = va_arg(argList, char*);*/


	vsprintf_s(szBufFormat, pszFormat, argList);
	strcat_s(szBufFormat_Game, szBufFormat);
	OutputDebugStringA(szBufFormat_Game);


	// 清除list
	va_end(argList);
#endif

}


int main()
{
	DbgPrintMine((char*)"%d, %d, %s\n", 1, 2, "贾谨荣");
	system("pause");
}




```

## 注：多线程访问数据造成异常的原因以及解决方式

#### 造成异常的原因：

- 游戏主线程与外挂线程同时访问共享数据区域，造成程序异常
- 让两个线程依次使用共享数据或者将注入线程到主线程

#### 模拟游戏主线程和辅助线程同时执行

![](https://i.loli.net/2019/07/25/5d396c0068fd480536.png)

- 代码

```c++
// 定义变参函数
void DbgPrintMine(char* pszFormat, ...) {
#ifdef _DEBUG
	// 如果在DEBUG 版本下才执行以下代码
	// 定义list
	va_list argList;
	// 初始化list
	va_start(argList, pszFormat);
	// 定义字符串缓冲区
	char szBufFormat[0x1000];

	// 定义调试前缀
	char szBufFormat_Game[0x1008] = "Game:";


	// 获取参数 va_arg(list, paramType)
	/*int i = va_arg(argList, int);
	int j = va_arg(argList, int);
	char* szK = va_arg(argList, char*);*/


	vsprintf_s(szBufFormat, pszFormat, argList);
	strcat_s(szBufFormat_Game, szBufFormat);
	OutputDebugStringA(szBufFormat_Game);


	// 清除list
	va_end(argList);
#endif

}

DWORD g_ndGameData[10] = { 111, 222, 333, 444, 555, 666, 777, 888, 999, 000 };
DWORD* g_pndGameData[10];

void UseGoods(char* szGoodsName) {

	for (int i = 0; i < 10; i++) {
		DbgPrintMine("%s, %d\r\n", szGoodsName, *g_pndGameData[i]);
		Sleep(1 * 1000);
	}
	return;
}

DWORD WINAPI GameMainThreadProc(LPVOID lpData) {
	while (TRUE) {
		// 初始化内存
		memset(g_pndGameData, NULL, sizeof(g_pndGameData));
		for (int i = 0; i < 10; i++) {
			g_pndGameData[i] = g_ndGameData + i; // &g_ndGameData[i]
			Sleep(1000);
		}
		// 物品使用的CALL
		UseGoods("游戏主线程");
	}
}

DWORD WINAPI GameMyThreadProc(LPVOID lpData) {
	while (TRUE) {
		// 初始化内存
		
		UseGoods("外挂线程:222");
		Sleep(1 * 1000);
	}
}

// 游戏主线程
void CDataExceptionTestDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CreateThread(NULL, NULL, GameMainThreadProc, NULL, 0, NULL);
}

// 外挂线程
void CDataExceptionTestDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CreateThread(NULL, NULL, GameMyThreadProc, NULL, 0, NULL);
}


```

- 异常

![](https://i.loli.net/2019/07/25/5d396c8ded2eb52094.png)

#### 解决方式

- 将程序注入到主线程

- 使用临界区

  - 1

    ![](https://i.loli.net/2019/07/25/5d3970363d1b650747.png)

  - 2、代码

    ```c++
    // 定义变参函数
    void DbgPrintMine(char* pszFormat, ...) {
    #ifdef _DEBUG
    	// 如果在DEBUG 版本下才执行以下代码
    	// 定义list
    	va_list argList;
    	// 初始化list
    	va_start(argList, pszFormat);
    	// 定义字符串缓冲区
    	char szBufFormat[0x1000];
    
    	// 定义调试前缀
    	char szBufFormat_Game[0x1008] = "Game:";
    
    
    	// 获取参数 va_arg(list, paramType)
    	/*int i = va_arg(argList, int);
    	int j = va_arg(argList, int);
    	char* szK = va_arg(argList, char*);*/
    
    
    	vsprintf_s(szBufFormat, pszFormat, argList);
    	strcat_s(szBufFormat_Game, szBufFormat);
    	OutputDebugStringA(szBufFormat_Game);
    
    
    	// 清除list
    	va_end(argList);
    #endif
    
    }
    
    // 定义临界区;
    CRITICAL_SECTION lpCriticalSection;
    
    DWORD g_ndGameData[10] = { 111, 222, 333, 444, 555, 666, 777, 888, 999, 000 };
    DWORD* g_pndGameData[10];
    
    void UseGoods(char* szGoodsName) {
    	// 进入临界区
    	EnterCriticalSection(&lpCriticalSection);
    
    	for (int i = 0; i < 10; i++) {
    		DbgPrintMine("%s, %d\r\n", szGoodsName, *g_pndGameData[i]);
    		Sleep(1 * 100);
    	}
    	// 离开临界区
    	LeaveCriticalSection(&lpCriticalSection);
    
    	return;
    }
    
    DWORD WINAPI GameMainThreadProc(LPVOID lpData) {
    	while (TRUE) {
    		// 进入临界区
    		EnterCriticalSection(&lpCriticalSection);
    
    		// 初始化内存
    		memset(g_pndGameData, NULL, sizeof(g_pndGameData));
    		for (int i = 0; i < 10; i++) {
    			g_pndGameData[i] = g_ndGameData + i; // &g_ndGameData[i]
    			Sleep(1000);
    		}
    		// 离开临界区
    		LeaveCriticalSection(&lpCriticalSection);
    
    		// 腾出有点时间片给外挂线程使用
    		Sleep(1 * 1000);
    		// 物品使用的CALL
    		UseGoods("游戏主线程");
    	}
    }
    
    DWORD WINAPI GameMyThreadProc(LPVOID lpData) {
    	while (TRUE) {
    		// 初始化内存
    		
    		UseGoods("外挂线程:222");
    		Sleep(1 * 1000);
    	}
    }
    
    // 游戏主线程
    void CDataExceptionTestDlg::OnBnClickedButton1()
    {
    	// TODO: 在此添加控件通知处理程序代码
    	CreateThread(NULL, NULL, GameMainThreadProc, NULL, 0, NULL);
    }
    
    // 外挂线程
    void CDataExceptionTestDlg::OnBnClickedButton2()
    {
    	// TODO: 在此添加控件通知处理程序代码
    	CreateThread(NULL, NULL, GameMyThreadProc, NULL, 0, NULL);
    }
    
    
    void CDataExceptionTestDlg::OnBnClickedOk()
    {
    	// TODO: 在此添加控件通知处理程序代码
    	CDialogEx::OnOK();
    }
    
    
    void CDataExceptionTestDlg::OnBnClickedButton3()
    {
    	// TODO: 在此添加控件通知处理程序代码
    	// 初始化临界区
    	InitializeCriticalSection(&lpCriticalSection);
    }
    
    
    ```

## 将代码注入游戏的主线程

- 关键词

  ```c++
  SetWindowsHooksExa UnhookWindowsHookEx CWPSTRUCT
  
  ```

### 定义方法

```c++
#pragma once

// HookGameMainThread.h
#define MSG_USEGOODSFORNAME 1 //使用物品的消息种类

// 挂载主线程
DWORD HookMainThread();

// 卸载主线程
DWORD UnHookMainThread();

DWORD msgUseGoodsForName(char* szpName);

```

### 实现方法

```c++
// HookGameMainThread.cpp
#include "StructGame.h"
#include "HookGameMainThread.h"

HHOOK g_hhkGame;
const DWORD MyMsgCode = RegisterWindowMessageA("MyMsgCode");
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
				TGOODSLIST_PROPERTY goods;
				TGOODSLIST_PROPERTY* g = goods.getData();
				if (g->UseGoodsForName((char*)lpArg->lParam)) {
					DbgPrintMine((char*)("使用 %s 成功"), lpArg->lParam);
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

```

### 调用方法

```c++
// 连接主线程
void CMainDialogWnd::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	HookMainThread();
}


void CMainDialogWnd::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	UnHookMainThread();
}


void CMainDialogWnd::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	msgUseGoodsForName("金创药(小)");
}

```

### 文件结构

![](https://i.loli.net/2019/07/27/5d3c0b3353ba674453.png)

### 运行效果

![](https://i.loli.net/2019/07/27/5d3c0b5f3718e26663.png)

## 分析怪物列表

- 分析思路：从怪物明显的属性入手：名字、**血量**等

  1*4+427EBA0 //怪物列表基址（1-5）

  +8 种类/2E:怪物

  +354 显示血条

  +C 怪物选中参数

  +5f4 怪物血量

  +5f8 怪物等级

  +360 怪物名字

  +1060 怪物位置X

  +1068 怪物位置Y

  +3C0 怪物生命状态 0活/1死

  [0427EBA0] //角色对象指针

  +8 //角色分类31人物/2E

  +18 //角色名字

## 封装怪物对象属性

- 定义基址

  ```c++
  // 怪物列表基址
  #define BaseMonseterList 0x427EBA4
  
  ```

- 定义结构

  ```c++
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
  #define MONSETERNUM 20
  typedef struct TMonseterList {
  
  	_TMonseterObj tMonList[MONSETERNUM];
  
  	// 初始化
  	TMonseterList* getData();
  
  	// 打印信息
  	BOOL dbgPrintMsg();
  }_TMonseterList;
  
  ```

- 实现结构方法

  ```c++
  TMonseterList* TMonseterList::getData()
  {
  	DWORD ndObj = NULL;
  	//memset(this, 0, sizeof(TMonseterList));
  	try
  	{
  		for (int i = 0; i < MONSETERNUM; i++) {
  			ndObj = *(DWORD*)(BaseMonseterList + 4 * i);
  			if (ndObj == 0) {
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
  
  ```

- 在HOOK 内定义测试方法

  ```c++
  #define MSG_TEST 2 // 测试使用消息
  
  // 测试怪物
  DWORD msgTest(LPVOID lpData);
  
  ```

- 实现

  ```c++
  // HookGameMainThread.cpp
  #include "StructGame.h"
  #include "HookGameMainThread.h"
  
  HHOOK g_hhkGame;
  const DWORD MyMsgCode = RegisterWindowMessageA("MyMsgCode");
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
  				TGOODSLIST_PROPERTY goods;
  				TGOODSLIST_PROPERTY* g = goods.getData();
  				if (g->UseGoodsForName((char*)lpArg->lParam)) {
  					DbgPrintMine((char*)("使用 %s 成功"), lpArg->lParam);
  				}
  			}; break;
  			
  ////////////////////////////////////////////////////////////////////////
  			case MSG_TEST: {
  				TMonseterList tMonList;
  				TMonseterList* ptMonList = tMonList.getData();
  				ptMonList->dbgPrintMsg();
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
  
  ////////////////////////////////////////////////////////////////////////
  DWORD msgTest(LPVOID lpData)
  {
  	SendMessageA(GetGameWndHandle(), MyMsgCode, MSG_TEST, (LPARAM)lpData);
  	return 0;
  }
  
  
  ```

- 控件调用

  ```c++
  void CMainDialogWnd::OnBnClickedButton5()
  {
  	// TODO: 在此添加控件通知处理程序代码
  	msgTest(NULL);
  }
  
  
  ```

- 目录结构

  ![img](https://img-blog.csdnimg.cn/20190731224437537.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTM1MDk3,size_16,color_FFFFFF,t_70)



## 分析动作数组（攻击和打坐）

- 思路：
  - 通过选中的对象逆向回溯出动作的数组
  - 通过动作对象访问逆向回溯到攻击CALL 附近
  - 封包断点bp WSASend

1. 通过选中动作，利用CE 查找基址

2. 使用OD 分析访问内存信息，得到基址

   ```汇编
   0082D8F2    8D8CB7 3C040000 LEA ECX,DWORD PTR DS:[EDI+ESI*4+43C]
   
   ```

   动作公式：[02e3bd58]+43c+4*0

3. **找动作的CALL**

   - 使用CE 分析动作对象的调用访问

   - 得到一下信息

     ```汇编
     // 攻击
     008530CE - 6A 01 - push 01
     008530D0 - E8 0B4AFBFF - call Client.exe+407AE0
     008530D5 - 8B 8C B7 3C040000  - mov ecx,[edi+esi*4+0000043C] <<
     008530DC - 85 C9  - test ecx,ecx
     008530DE - 74 62 - je Client.exe+453142
     
     008544B9 - 83 BF 34160000 35 - cmp dword ptr [edi+00001634],35
     008544C0 - 75 20 - jne Client.exe+4544E2
     008544C2 - 8B 84 B7 3C040000  - mov eax,[edi+esi*4+0000043C] <<
     008544C9 - 85 C0  - test eax,eax
     008544CB - 74 15 - je Client.exe+4544E2
     
     
     ```

   - 使用OD分析得动作CALL为

     ```汇编
     mov edi, [02E3CD58]
     mov esi, 下标
     MOV EAX,DWORD PTR DS:[EDI+ESI*4+43C]
     mov ecx, [eax+0x54]
     push ecx
     CALL 007139E0
     
     
     ```

## 封装动作数组功能

- 封装动作对象
- 封装动作对象列表
- 封装使用对象功能函数

#### 封装

1. 封装基址

   ```C++
   // 人物动作使用的CALL 的基址
   #define BaseActionCall 0x00713970
   
   ```

2. 封装结构

   ```c++
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
   
   ```

3. 实现结构方法

   ```c++
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
   
   ```

4. 添加消息类型

   ```c++
   #define MSG_ACTIONTEST 3 //测试动作
   
   ```

   

5. 在主线程内调用结构体方法

   ```c++
   case MSG_ACTIONTEST: {
   				TCActionList* ptLIst = tList.getData();
   				ptLIst->TestActionMsg();
   				//ptLIst->UseActionByIndex(1);
   				ptLIst->UseActionByName((char*)("攻击"));
   			}; break;
   
   ```

6. 发送消息到主线程

   ```c++
   DWORD testActionMsg(LPVOID lpData) {
   	SendMessageA(GetGameWndHandle(), MyMsgCode, MSG_ACTIONTEST, (LPARAM)lpData);
   	return 0;
   }
   
   ```

   

7. 绑定控件，执行方法

   ```c++
   void CMainDialogWnd::OnBnClickedButton6()
   {
   	// TODO: 在此添加控件通知处理程序代码
   	testActionMsg(NULL);
   }
   
   ```



- 文件结构

  ![img](https://img-blog.csdnimg.cn/20190808170008912.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTM1MDk3,size_16,color_FFFFFF,t_70)

## 选怪功能实现

- 实现怪物选中
- 可能情况：
  - 选怪变量被赋值
  - 怪物是否被选中的属性

#### 选怪功能相关地址

- 玩家：

  [2E63A24] //存放的玩家对象的地址

  +3428 玩家是否被选中

- 怪物：

  [2E63A24]+1A64

  选中怪物时，传入怪物的选中ID

  没选中怪物时，值为0xFFFF

## 选怪功能的封装



## 计算怪物与玩家的距离





# 更新ING

