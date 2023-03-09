#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "config.h"
#include "mygame.h"

using namespace game_framework;
/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲開頭畫面物件
/////////////////////////////////////////////////////////////////////////////

CGameStateInit::CGameStateInit(CGame *g) : CGameState(g)
{
}

void CGameStateInit::OnInit()
{
	// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
	//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
	//
	startmenu.Load({ "img/tri.bmp" }, {"img/ao oni_action/oni_00.bmp"
}, RGB(0, 0, 0), RGB(204, 255, 0));
	startmenu.SetParam(SIZE_X / 2-75, SIZE_Y / 2-75, 0, 0, SIZE_X / 2-75-5, SIZE_Y / 2-75, 50 , { "Start","Load","Close" });
	ShowInitProgress(0, "Start Initialize...");	// 一開始的loading進度為0%
	
	Sleep(1000);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
	//
	// 此OnInit動作會接到CGameStaterRun::OnInit()，所以進度還沒到100%
	//
}

void CGameStateInit::OnBeginState()
{
}

void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_RETURN) {
	}
}
void CGameStateInit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	startmenu.OnMovingCursor(nChar);
	if (nChar == VK_RETURN) {
		switch (startmenu.GetSelection()) {
		case 0:
			break;
		case 1:
			break;
		case 2:
			break;
		}
	}
}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
{

}
void CGameStateInit::OnShow()
{	
	CDC *pDC = CDDraw::GetBackCDC();
	CFont *fp;
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(255, 255, 255));
	CTextDraw::ChangeFontLog(pDC, 20, "Noto Sans TC",RGB(255,255,255));

	startmenu.ShowText(pDC, fp);
	CDDraw::ReleaseBackCDC();
	startmenu.ShowBitmap();
	startmenu.ShowCursor();
}
