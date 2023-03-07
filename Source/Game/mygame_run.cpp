#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"

using namespace game_framework;

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
/////////////////////////////////////////////////////////////////////////////

CGameStateRun::CGameStateRun(CGame *g) : CGameState(g)
{
}

CGameStateRun::~CGameStateRun()
{
}

void CGameStateRun::OnBeginState()
{
}

void CGameStateRun::OnMove()							// 移動遊戲元素
{
	
}

void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
{
	try0.LoadBitmapByString({"img/hiroshi_move/Hiroshi_00.bmp", "img/hiroshi_move/Hiroshi_01.bmp", "img/hiroshi_move/Hiroshi_02.bmp", 
		"img/hiroshi_move/Hiroshi_10.bmp", "img/hiroshi_move/Hiroshi_11.bmp", "img/hiroshi_move/Hiroshi_12.bmp", 
		"img/hiroshi_move/Hiroshi_20.bmp", "img/hiroshi_move/Hiroshi_21.bmp", "img/hiroshi_move/Hiroshi_22.bmp", 
		"img/hiroshi_move/Hiroshi_30.bmp", "img/hiroshi_move/Hiroshi_31.bmp", "img/hiroshi_move/Hiroshi_32.bmp", 
		}, RGB(255, 255, 255));
	try0.SetTopLeft(150, 265);
	
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// position
	int hiroshi_x = try0.GetLeft();
	int hiroshi_y = try0.GetTop();
	int hiroshi_h = try0.GetHeight();
	int hiroshi_w = try0.GetWidth();
	
	if (nChar == VK_LEFT) {
		hiroshi_x -= hiroshi_w;
	}
	else if (nChar == VK_RIGHT) {
		hiroshi_x += hiroshi_w;
	}
	else if (nChar == VK_UP) {
		hiroshi_y -= hiroshi_w;
	}
	else if (nChar == VK_DOWN) {
		hiroshi_y += hiroshi_w;
	}
	try0.SetTopLeft(hiroshi_x, hiroshi_y);
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	
}

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
}

void CGameStateRun::OnShow()
{
	show_image_by_phase();
}

void CGameStateRun::show_image_by_phase()
{
	try0.ShowBitmap();
}
