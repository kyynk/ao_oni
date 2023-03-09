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

	Human::Human(int x, int y) :Entity() {
		pos_x = -59 + 64 * 7;
		pos_y = -33 + 64 * 4;
		_step = 2;
		_walkiter = true;
		_state = still;
		_bstate = s1;
		_blocked = false;
		TimerReset();
	}
	void Human::OnMove() {
		if (_blocked) {
			if (TimerGetCount() < 16) {
				_bstate = s1;
			}
			else {
				_bstate = s2;
			}
			if (TimerGetCount() == 32) {
				TimerReset();
				_walkiter = !_walkiter;
				_state = still;
			}
		}
		
		else if (_state > 0 && _state < 5) {
			if (TimerGetCount() < 8) {
				_bstate = s1;
			}
			else {
				_bstate = s2;
			}
			if (TimerGetCount() == 16) {
				TimerReset();
				_walkiter = !_walkiter;
				_state = still;
			}
			else if (_state == movingup) {
				pos_y -= _step;
			}
			else if (_state == movingdown) {
				pos_y += _step;
			}
			else if (_state == movingleft) {
				pos_x -= _step;
			}
			else if (_state == movingright) {
				pos_x += _step;
			}
		}
		if (_state != 0)
			TimerUpdate();

	}
	void Human::SelectState(UINT nChar) {
		if (nChar == VK_LEFT) {
			_state = movingleft;
		}
		else if (nChar == VK_UP) {
			_state = movingup;
		}
		else if (nChar == VK_RIGHT) {
			_state = movingright;
		}
		else if (nChar == VK_DOWN) {
			_state = movingdown;
		}
	}
	void Human::OnShow() {
		bitmap.SetTopLeft(pos_x, pos_y);
		switch (_state) {
		case movingup:
			if (_bstate == s1) {
				_walkiter ? bitmap.SetFrameIndexOfBitmap(HUMAN_UP_1) : bitmap.SetFrameIndexOfBitmap(HUMAN_UP_2);
			}
			else {
				bitmap.SetFrameIndexOfBitmap(HUMAN_UP);
			}
			break;
		case movingdown:
			if (_bstate == s1) {
				_walkiter ? bitmap.SetFrameIndexOfBitmap(HUMAN_DOWN_1) : bitmap.SetFrameIndexOfBitmap(HUMAN_DOWN_2);
			}
			else {
				bitmap.SetFrameIndexOfBitmap(HUMAN_DOWN);
			}
			break;

		case movingleft:
			if (_bstate == s1) {
				_walkiter ? bitmap.SetFrameIndexOfBitmap(HUMAN_LEFT_1) : bitmap.SetFrameIndexOfBitmap(HUMAN_LEFT_2);
			}
			else {
				bitmap.SetFrameIndexOfBitmap(HUMAN_LEFT);
			}
			break;

		case movingright:
			if (_bstate == s1) {
				_walkiter ? bitmap.SetFrameIndexOfBitmap(HUMAN_RIGHT_1) : bitmap.SetFrameIndexOfBitmap(HUMAN_RIGHT_2);
			}
			else {
				bitmap.SetFrameIndexOfBitmap(HUMAN_RIGHT);
			}
			break;
		default:
			__asm {nop}
		}

		bitmap.ShowBitmap();

	}
	void Human::Load(vector<string> filenames, COLORREF color) {
		bitmap.LoadBitmapByString(filenames,color );

	}

}