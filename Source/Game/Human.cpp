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
#include "human.h"
namespace game_framework{

	Human::Human(int x, int y) :Entity(x, y) {
		pos_x = -59 + 64 * 7;
		pos_y = -33 + 64 * 4;
		_walkiter = true;
		_direction = down;
		_state = still;
		_bstate = s1;
		_blocked = false;
		TimerReset();
	}
	void Human::OnMove(bool pressed) {
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
				//_MovingUp = _MovingDown = _MovingLeft = _MovingRight = false;
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
				pos_y -= step;
			}
			else if (_state == movingdown) {
				pos_y += step;
			}
			else if (_state == movingleft) {
				pos_x -= step;
			}
			else if (_state == movingright) {
				pos_x += step;
			}
		}
		if (_state != 0)
			TimerUpdate();

	}
	void Human::OnShow() {
		bitmap.SetTopLeft(pos_x, pos_y);
		switch (_state) {
		case movingup:
			if (_bstate == s1) {
				_walkiter ? bitmap.SetFrameIndexOfBitmap(HERO_UP_WALK_1) : bitmap.SetFrameIndexOfBitmap(HERO_UP_WALK_2);
			}
			else {
				bitmap.SetFrameIndexOfBitmap(HERO_UP);
			}
			break;


		case movingdown:
			if (_bstate == s1) {
				_walkiter ? bitmap.SetFrameIndexOfBitmap(HERO_DOWN_WALK_1) : bitmap.SetFrameIndexOfBitmap(HERO_DOWN_WALK_2);
			}
			else {
				bitmap.SetFrameIndexOfBitmap(HERO_DOWN);
			}
			break;

		case movingleft:
			if (_bstate == s1) {
				_walkiter ? bitmap.SetFrameIndexOfBitmap(HERO_LEFT_WALK_1) : bitmap.SetFrameIndexOfBitmap(HERO_LEFT_WALK_2);
			}
			else {
				bitmap.SetFrameIndexOfBitmap(HERO_LEFT);
			}
			break;

		case movingright:
			if (_bstate == s1) {
				_walkiter ? bitmap.SetFrameIndexOfBitmap(HERO_RIGHT_WALK_1) : bitmap.SetFrameIndexOfBitmap(HERO_RIGHT_WALK_2);
			}
			else {
				bitmap.SetFrameIndexOfBitmap(HERO_RIGHT);
			}
			break;
		default:
			__asm {nop}
		}

	}
	void Human::Load(vector<string> filenames, COLORREF color) {
		bitmap.LoadBitmapByString(filenames,color );

	}

}