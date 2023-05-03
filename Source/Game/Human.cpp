#include "stdafx.h"
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "Entity.h"
#include "Human.h"
namespace game_framework {
	Human::Human() :Entity() {
	
	}
	void Human::SetPos(int x, int y) {
		SetXY(x,y - _offsetY);
	}
	void Human::init(int step, int offset, Direction dir) {
		_step = step;
		_offsetY = offset;
		_direction = dir;
		_machine_count = 0;
		_walkiter = true;
		_bstate = s1;
		_machine_done = false;
	}
	void Human::OnMove() {

		if (_direction == up) {
			bitmap.SetFrameIndexOfBitmap(BITMAP_UP);
		}
		else if (_direction == down) {
			bitmap.SetFrameIndexOfBitmap(BITMAP_DOWN);
		}
		else if (_direction == left) {
			bitmap.SetFrameIndexOfBitmap(BITMAP_LEFT);
		}
		else if (_direction == right) {
			bitmap.SetFrameIndexOfBitmap(BITMAP_RIGHT);
		}
		bitmap.SetTopLeft(_pos_x, _pos_y);

	}
	void Human::Load(vector<string> &filenames, COLORREF color) {
		bitmap.LoadBitmapByString(filenames, color);
	}
	void Human::OnShow() {
		if (_nowmove == up) {
			if (_bstate == s1) {
				_walkiter ? bitmap.SetFrameIndexOfBitmap(BITMAP_UP_1) :
					bitmap.SetFrameIndexOfBitmap(BITMAP_UP_2);
			}
			else {
				bitmap.SetFrameIndexOfBitmap(BITMAP_UP);
			}
		}
		else if (_nowmove == down) {
			if (_bstate == s1) {
				_walkiter ? bitmap.SetFrameIndexOfBitmap(BITMAP_DOWN_1) :
					bitmap.SetFrameIndexOfBitmap(BITMAP_DOWN_2);
			}
			else {
				bitmap.SetFrameIndexOfBitmap(BITMAP_DOWN);
			}
		}

		else if (_nowmove == left) {
			if (_bstate == s1) {
				_walkiter ? bitmap.SetFrameIndexOfBitmap(BITMAP_LEFT_1) :
					bitmap.SetFrameIndexOfBitmap(BITMAP_LEFT_2);
			}
			else {
				bitmap.SetFrameIndexOfBitmap(BITMAP_LEFT);
			}
		}

		else if (_nowmove == right) {
			if (_bstate == s1) {
				_walkiter ? bitmap.SetFrameIndexOfBitmap(BITMAP_RIGHT_1) :
					bitmap.SetFrameIndexOfBitmap(BITMAP_RIGHT_2);
			}
			else {
				bitmap.SetFrameIndexOfBitmap(BITMAP_RIGHT);
			}
		}
		bitmap.ShowBitmap();
	}
}