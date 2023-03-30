#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include <bitset>
#include <vector>
#include "config.h"
#include "Entity.h"
#include "Oni.h"


#define ONI_DOWN 0
#define ONI_DOWN_1 1
#define ONI_DOWN_2 2
#define ONI_UP 9
#define ONI_UP_1 10
#define ONI_UP_2 11
#define ONI_LEFT 3
#define ONI_LEFT_1 4
#define ONI_LEFT_2 5
#define ONI_RIGHT 6
#define ONI_RIGHT_1 7
#define ONI_RIGHT_2 8

namespace game_framework {
	Oni::Oni() {
		SetXY(0, 0);
		_type = normal;
		ResetOT();
		_humanX, _humanY, _step = 0;
		_isDisappear = false;
		_walkiter = true;
		_bstate = s1;
		_blocked = false;
		_isup = false;
		_isdown = false;
		_isleft = false;
		_isright = false;
		TimerReset();
		_premove = none;
		_nowmove = none;
		_tracking = none;
	}
	Oni::~Oni() {
	}
	void Oni::SetParam(Oni::OniType tp, int step, int moveTime) {
		_type = tp;
		_step = step;
		_moveTime = moveTime;
		string name;
		if (_type == normal) {
			name = "oni_";
		}
		else if (_type == mika) {
			name = "mika_";
		}
		Load(name, RGB(204, 255, 0));
	}
	void Oni::Load(string filename, COLORREF color) {
		vector<string> oniVec;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 3; j++) {
				oniVec.push_back("img/ao oni_action/" + filename +
					to_string(i) + to_string(j) + ".bmp");
			}
		}
		bitmap.LoadBitmapByString(oniVec, color);
	}
	void Oni::GetPlayerPos(int playerX, int playerY) {
		_humanX = playerX;
		_humanY = playerY;
	}
	void Oni::Track() {
		int xLen = _pos_x - _humanX;
		int yLen = _pos_y - _humanY;
		if (abs(xLen) < abs(yLen)) {
			if (yLen < 0) _tracking = isdown;
			else _tracking = isup;
		}
		else {
			if (xLen < 0) _tracking = isright;
			else _tracking = isleft;
		}
	}
	void Oni::OnMove() {
		//according humanXY
		//we find the abs((human - pos)X) abs((human - pos)Y)
		//which is bigger than the other
		//we will choose it
		//and move it first
		Track();
	}
	void Oni::OnShow() {
		//if (!_isDisappear)
		//show...
		if (_nowmove == isup) {
			if (_bstate == s1) {
				_walkiter ? bitmap.SetFrameIndexOfBitmap(ONI_UP_1) : bitmap.SetFrameIndexOfBitmap(ONI_UP_2);
			}
			else {
				bitmap.SetFrameIndexOfBitmap(ONI_UP);
			}
		}
		else if (_nowmove == isdown) {
			if (_bstate == s1) {
				_walkiter ? bitmap.SetFrameIndexOfBitmap(ONI_DOWN_1) : bitmap.SetFrameIndexOfBitmap(ONI_DOWN_2);
			}
			else {
				bitmap.SetFrameIndexOfBitmap(ONI_DOWN);
			}
		}

		else if (_nowmove == isleft) {
			if (_bstate == s1) {
				_walkiter ? bitmap.SetFrameIndexOfBitmap(ONI_LEFT_1) : bitmap.SetFrameIndexOfBitmap(ONI_LEFT_2);
			}
			else {
				bitmap.SetFrameIndexOfBitmap(ONI_LEFT);
			}
		}

		else if (_nowmove == isright) {
			if (_bstate == s1) {
				_walkiter ? bitmap.SetFrameIndexOfBitmap(ONI_RIGHT_1) : bitmap.SetFrameIndexOfBitmap(ONI_RIGHT_2);
			}
			else {
				bitmap.SetFrameIndexOfBitmap(ONI_RIGHT);
			}
		}
		bitmap.ShowBitmap();
	}
	void Oni::ResetOT() {
		_overTime = 10;
	}
	void Oni::Countdown() {
		//_time--
		//if _time == 0
		//_isDisappear = true
		if (_overTime == 0) _isDisappear = true;
		else _overTime--;
	}
	void Oni::ChangeMap() {
		//SetPos
		//_time ++
		_overTime++;
	}
	bool Oni::isCatch(int playerX, int playerY) {
		return _pos_x == playerX && _pos_y == playerY;
	}
}