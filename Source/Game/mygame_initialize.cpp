#include "stdafx.h"
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"

namespace game_framework {
	CGameStateInit::CGameStateInit(CGame *g) : CGameState(g)
	{
	}

	void CGameStateInit::OnInit()
	{
		_substate = startmenustate;
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
		story.SetFigure("none");
		story.SetParam({ "We heard rumors about the mansion",
			"they say on the outskirts of town...",
			"there is a monster living here...!" }, false);

		CAudio::Instance()->Load(AUDIO_GAME_INIT, "Audio/USE/game_init.wav");
	}

	void CGameStateInit::OnBeginState()
	{
		_substate = 0;
		start_animation.ToggleAnimation();
	}

	void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		
	}
	void CGameStateInit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
		switch (_substate)
		{
		case startmenustate:
			startmenu.OnMovingCursor(nChar);		
			if (nChar == VK_SPACE) {
				switch (startmenu.GetSelection()) {
				case 0:
					_substate = animationstate;
					CAudio::Instance()->Play(AUDIO_GAME_INIT, false);
					break;
				case 1:

					break;
				case 2:

					break;
				}
			}
			break;
		case showstorydialogstate:
			if (nChar == VK_SPACE) {
				if (story.isShow()) {
					story.SetShow(false);
					_substate = jumpstate;
				}
				CAudio::Instance()->Stop(AUDIO_GAME_INIT);
			}
			
			break;
		default:
			break;
		}
	}
	void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
	{

	}
	void CGameStateInit::OnShow()
	{
		if (_substate == animationstate) {
			start_animation.SetAnimation(1, true);
			start_animation.ShowBitmap();
			if (start_animation.IsAnimationDone()) {
				_substate = showstorydialogstate;
			}
		}
		else if (startmenustate == _substate) {
			startmenu.ShowCursor();
			CDC* pDC = CDDraw::GetBackCDC();
			CTextDraw::ChangeFontLog(pDC, 20, "Consolas", RGB(255, 255, 255));
			startmenu.ShowText(pDC);
			CDDraw::ReleaseBackCDC();
		}
		else if (_substate == showstorydialogstate) {
			story.SetShow(true);
			story.ShowTotal();

		}
		else if (jumpstate) {
			GotoGameState(GAME_STATE_RUN);
		}
	}
}