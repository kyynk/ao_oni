#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include <bitset>
#include <fstream>
#include "vector3d.h"
#include "GameMap.h"
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
	MapRes::GetInstance()->Cleanup();
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
	ShowInitProgress(33, "loading game mid");
	vector<string> tmp;
	for (int i = 0; i < 4;i++) {
		for (int j = 0; j < 3; j++) {
			tmp.push_back("img/hiroshi_move/Hiroshi_"+to_string(i)+to_string(j)+".bmp");
		}
	}
	player.Load(tmp,RGB(204,255,0));
	std::ifstream mapres_in("map_bmp/mapsize.txt");
	string name;
	int count;
	for (int i = 0; i < 12; i++) {
		mapres_in >> name >> count;
		ShowInitProgress(33 + i, name);
		MapRes::GetInstance()->Load(name, count);
	}
	test.Load("house1_hallway1.txt", RGB(0, 0, 0));
	
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
	test.ShowMap(1,MapRes::GetInstance()->GetData());
	//MapRes::GetInstance()->checkres();
}
