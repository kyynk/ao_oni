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
// 這個class為遊戲的結束狀態(Game Over)
/////////////////////////////////////////////////////////////////////////////

CGameStateOver::CGameStateOver(CGame *g): CGameState(g)
{
}

void CGameStateOver::OnMove()
{
}

void CGameStateOver::OnBeginState()
{
	GameOver.SetTopLeft((SIZE_X - 16 - GameOver.GetWidth()) / 2, (SIZE_Y - 20 - GameOver.GetHeight()) / 2);
}

void CGameStateOver::OnInit()
{
	//// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
	////     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
	//ShowInitProgress(66, "Initialize...");	// 接個前一個狀態的進度，此處進度視為66%
	//ShowInitProgress(100, "OK!");
	GameOver.LoadBitmapByString({ "img/game_over/gameover.bmp" }, RGB(204, 255, 0));
}

void game_framework::CGameStateOver::OnKeyDown(UINT nChar, UINT, UINT)
{
	if (nChar == VK_SPACE) {
		GotoGameState(GAME_STATE_INIT);
	}
}

void CGameStateOver::OnShow()
{
	GameOver.ShowBitmap();
}
