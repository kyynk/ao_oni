#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include <bitset>
#include "config.h"
#include "Entity.h"
#include "Human.h"

#define HUMAN_DOWN 0
#define HUMAN_DOWN_1 1
#define HUMAN_DOWN_2 2
#define HUMAN_UP 9
#define HUMAN_UP_1 10
#define HUMAN_UP_2 11
#define HUMAN_LEFT 3
#define HUMAN_LEFT_1 4
#define HUMAN_LEFT_2 5
#define HUMAN_RIGHT 6
#define HUMAN_RIGHT_1 7
#define HUMAN_RIGHT_2 8

namespace game_framework{

	Human::Human() :Entity() {
		SetXY(12 * TILE, 11 * TILE + TILE / 2);
		_step = 0;
		_walkiter = true;
		_bstate = s1;
		_blocked = false;
		_isup = false ;
		_isdown = false;
		_isleft = false;
		_isright = false;
		TimerReset();
		_premove = none;
		_nowmove = none;
		_pressing = none;
	}
	void Human::init(int step) {
		_step = step;
	}

	void Human::OnMove() {

		if (_isup || _isdown || _isleft || _isright) {
			SetTimer(true);
		}
		else {
			if (TimerGetCount() == 8) {
				SetTimer(false);
				_walkiter = !_walkiter;
			}
		}

		if (IsTimerStart()) {
			if (TimerGetCount() % 8 == 0) {
				_nowmove = _pressing;
			}
			if (TimerGetCount() == 8) {
				TimerReset();
				_walkiter = !_walkiter;
			}
			if (TimerGetCount() < 4) {
				_bstate = s1;
			}
			else {
				_bstate = s2;
			}
			if (_nowmove == isup) {
				pos_y -= _step;
			}
			else if (_nowmove == isdown) {
				pos_y += _step;
			}
			else if (_nowmove == isleft) {
				pos_x -= _step;
			}
			else if (_nowmove == isright) {
				pos_x += _step;
			}
			TimerUpdate();

		}
		bitmap.SetTopLeft(pos_x, pos_y);
		TRACE("%d\n", TimerGetCount());
	}
	void Human::OnKeyDown(UINT nChar) {
		if (nChar == VK_LEFT) {
			_premove = _pressing;
			_pressing = isleft;
			_isleft = true;
		}
		else if (nChar == VK_UP) {
			_premove = _pressing;
			_pressing = isup;
			_isup = true;
		}
		else if (nChar == VK_RIGHT) {
			_premove = _pressing;
			_pressing = isright;
			_isright = true;
		}
		else if (nChar == VK_DOWN) {
			_premove = _pressing;
			_pressing = isdown;
			_isdown = true;
		}
	}

	void Human::OnKeyUp(UINT nChar){
		if (nChar == VK_LEFT) {
			_premove = isleft;
			_isleft = false;
		}
		else if (nChar == VK_UP) {
			_premove = isup;
			_isup = false;
		}
		else if (nChar == VK_RIGHT) {
			_premove = isright;
			_isright = false;
		}
		else if (nChar == VK_DOWN) {
			_premove = isdown;
			_isdown = false;
		}
	}

	void Human::OnShow() {
		

		if (_nowmove == isup) {
			if (_bstate == s1) {
				_walkiter ? bitmap.SetFrameIndexOfBitmap(HUMAN_UP_1) : bitmap.SetFrameIndexOfBitmap(HUMAN_UP_2);
			}
			else {
				bitmap.SetFrameIndexOfBitmap(HUMAN_UP);
			}
		}
		else if (_nowmove == isdown) {
			if (_bstate == s1) {
				_walkiter ? bitmap.SetFrameIndexOfBitmap(HUMAN_DOWN_1) : bitmap.SetFrameIndexOfBitmap(HUMAN_DOWN_2);
			}
			else {
				bitmap.SetFrameIndexOfBitmap(HUMAN_DOWN);
			}
		}

		else if (_nowmove == isleft) {
			if (_bstate == s1) {
				_walkiter ? bitmap.SetFrameIndexOfBitmap(HUMAN_LEFT_1) : bitmap.SetFrameIndexOfBitmap(HUMAN_LEFT_2);
			}
			else {
				bitmap.SetFrameIndexOfBitmap(HUMAN_LEFT);
			}
		}

		else if (_nowmove == isright) {
			if (_bstate == s1) {
				_walkiter ? bitmap.SetFrameIndexOfBitmap(HUMAN_RIGHT_1) : bitmap.SetFrameIndexOfBitmap(HUMAN_RIGHT_2);
			}
			else {
				bitmap.SetFrameIndexOfBitmap(HUMAN_RIGHT);
			}
		}


		bitmap.ShowBitmap();

	}
	void Human::Load(vector<string> filenames, COLORREF color) {
		bitmap.LoadBitmapByString(filenames,color );

	}

}