#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include <bitset>
#include <fstream>
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
	player.OnMove();

}

void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
{
	vector<string> tmp;
	for (int i = 0; i < 4;i++) {
		for (int j = 0; j < 3; j++) {
			tmp.push_back("img/hiroshi_move/Hiroshi_"+to_string(i)+to_string(j)+".bmp");
		}
	}
	player.Load(tmp,RGB(204,255,0));
	test.Load("map_bmp/house1_hallway1.txt", RGB(0, 0, 0));
	
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	player.OnKeyDown(nChar);
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	player.OnKeyUp(nChar);
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
	player.OnShow();
}
void CGameStateRun::LoadTiles() {

	std::ifstream in("map_bmp/mapsize.txt");
	string name;
	int count;
	vector<string> tmp;
	int total = 0;
	for (int i = 0; i < 12; i++) {
		in >> name >> count;
		CMovingBitmap tm;
		for (auto f : tmp) {
			//tm.LoadBitmapByString({ f });
			TRACE("%s\n", f.c_str());
			_tiles.push_back(tm);
		}
	}
}