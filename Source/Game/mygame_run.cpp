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
// �o��class���C�����C�����檫��A�D�n���C���{�����b�o��
/////////////////////////////////////////////////////////////////////////////

CGameStateRun::CGameStateRun(CGame *g) : CGameState(g){}
CGameStateRun::~CGameStateRun(){}

void CGameStateRun::OnBeginState()
{
	// CAudio::Instance()->Play(AUDIO_LAKE, false);			// ���� WAVE
		// CAudio::Instance()->Play(AUDIO_DING, false);		// ���� WAVE
		// CAudio::Instance()->Play(AUDIO_NTUT, false);			// ���� MIDI
}

void CGameStateRun::OnMove()							// ���ʹC������
{
	mycharacter.OnMove();
}

void CGameStateRun::OnInit()  								// �C������Ȥιϧγ]�w
{
		ShowInitProgress(10, "hi22222222222222");	// ���ӫe�@�Ӫ��A���i�סA���B�i�׵���33%
		Sleep(300); // ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����
		ShowInitProgress(50, "mid");
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	
}

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // �B�z�ƹ����ʧ@
{
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
{
}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
{
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // �B�z�ƹ����ʧ@
{
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
{
}

void CGameStateRun::OnShow()
{
}
