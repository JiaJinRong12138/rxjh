// CMainDialogWnd.cpp: 实现文件
//

#include "pch.h"
#include "MFC_DLL.h"
#include "CMainDialogWnd.h"
#include "afxdialogex.h"
#include "BaseGame.h"
#include "StructGame.h"
#include "HookGameMainThread.h"


// CMainDialogWnd 对话框

IMPLEMENT_DYNAMIC(CMainDialogWnd, CDialogEx)

CMainDialogWnd::CMainDialogWnd(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CMainDialogWnd::~CMainDialogWnd()
{
}

void CMainDialogWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMainDialogWnd, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CMainDialogWnd::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMainDialogWnd::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMainDialogWnd::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CMainDialogWnd::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CMainDialogWnd::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CMainDialogWnd::OnBnClickedButton6)
END_MESSAGE_MAP()


// CMainDialogWnd 消息处理程序


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

	if (g->UseGoodsForName("回城符(泫勃派)")) {
		TRACE("GameDebug: 使用 回城符(泫勃派) 成功");
	}


	// 进行数据修改
}


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


void CMainDialogWnd::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	msgTest(NULL);
}


void CMainDialogWnd::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	testActionMsg(NULL);
}
