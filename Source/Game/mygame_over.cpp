#include "stdafx.h"
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"

using namespace game_framework;

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
