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

CGameStateRun::CGameStateRun(CGame *g) : CGameState(g){}
CGameStateRun::~CGameStateRun(){}

void CGameStateRun::OnBeginState()
{
	// CAudio::Instance()->Play(AUDIO_LAKE, false);			// 撥放 WAVE
		// CAudio::Instance()->Play(AUDIO_DING, false);		// 撥放 WAVE
		// CAudio::Instance()->Play(AUDIO_NTUT, false);			// 撥放 MIDI
}

void CGameStateRun::OnMove()							// 移動遊戲元素
{
	mycharacter.OnMove();
}

void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
{
		ShowInitProgress(10, "hi22222222222222");	// 接個前一個狀態的進度，此處進度視為33%
		Sleep(300); // 放慢，以便看清楚進度，實際遊戲請刪除此
		ShowInitProgress(50, "mid");
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	
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
}
