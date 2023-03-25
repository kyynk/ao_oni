#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "config.h"
#include <bitset>
#include "mygame.h"

namespace game_framework {
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
		flag = 0;
		startmenu.Load({ "img/cursor/tri1_1.bmp","img/cursor/tri1_2.bmp","img/cursor/tri1_3.bmp","img/cursor/tri1_2.bmp" }, { "img/animation/big_face.bmp" }, RGB(0, 0, 0), RGB(204, 255, 0));
		startmenu.SetParam(SIZE_X / 2 - 75, SIZE_Y / 2 - 75, 0, 0, SIZE_X / 2 - 75 - 5, SIZE_Y / 2 - 75, 50, { "Start","Load","Close" });
		ShowInitProgress(0, "Start Initialize...");	// 一開始的loading進度為0%
		vector<string> tmp;
		for (int i = 0; i < 59; i++) {
			tmp.push_back("img/Start_animation/" + to_string(i) + ".bmp");
			ShowInitProgress(i / 10, "img/Start_animation/" + to_string(i));
		}
		start_animation.LoadBitmapByString(tmp);
		start_animation.SetTopLeft(SIZE_X / 2 - start_animation.GetWidth() / 2, SIZE_Y / 2 - start_animation.GetHeight() / 2);
		start_animation.ToggleAnimation();
		//Sleep(200);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep


		// 此OnInit動作會接到CGameStaterRun::OnInit()，所以進度還沒到100%
		//
	}

	void CGameStateInit::OnBeginState()
	{
	}

	void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		// asdw 0x41 0x53 0x44 0x57
		if (nChar == VK_RETURN) {
		}
	}
	void CGameStateInit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {

		startmenu.OnMovingCursor(nChar);
		TRACE("%x\n", nChar);
		if (nChar == VK_RETURN) {
			switch (startmenu.GetSelection()) {
			case 0:
				flag = 1;

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
		if (flag) {
			start_animation.SetAnimation(50, true);
			start_animation.ShowBitmap();
			if (start_animation.IsAnimationDone()) {
				GotoGameState(GAME_STATE_RUN);
			}
		}
		else {
			//startmenu.ShowBitmap();
			startmenu.ShowCursor();
			CDC *pDC = CDDraw::GetBackCDC();
			CTextDraw::ChangeFontLog(pDC, 20, "Noto Sans TC", RGB(255, 255, 255));
			startmenu.ShowText(pDC);
			CDDraw::ReleaseBackCDC();
		}
	}
}