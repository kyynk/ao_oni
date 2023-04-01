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
#include "GameMap.h"
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
		_humanX, _humanY, _step, 
			_offsetX, _offsetY= 0;
		_isDisappear = false;
		_walkiter = true;
		_bstate = s1;
		_blocked = false;
		_isup = false;
		_isdown = false;
		_isleft = false;
		_isright = false;
		_wait = false;
		TimerReset();
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
			_offsetX = 32;
			_offsetY = 80;
		}
		else if (_type == mika) {
			name = "mika_";
			_offsetX = 32;
			_offsetY = 80;
		}
		Load(name, RGB(204, 255, 0));
	}
	void Oni::SetPos(int x, int y) {
		SetXY(x, y - _offsetY);
	}
	int Oni::GetPosX() {
		return _pos_x;
	}
	int Oni::GetPosY() {
		return _pos_y + _offsetY;
	}
	int Oni::GetPosL() {
		return _pos_x - TILE;
	}
	int Oni::GetPosU() {
		return _pos_y + _offsetY - TILE;
	}
	int Oni::GetPosR() {
		return _pos_x + _offsetX + TILE;
	}
	int Oni::GetPosD() {
		return _pos_y + _offsetY - TILE;
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
	void Oni::Track(GameMap &map) {
		bool upmovable = false;
		bool downmovable = false;
		bool leftmovable = false;
		bool rightmovable = false;

		if ((map.GetMapData(0, (this->GetPosX() - map.GetX()) / TILE, (this->GetPosU() - map.GetY()) / TILE) == 0 ||
			map.GetMapData(0, (this->GetPosX() - map.GetX()) / TILE, (this->GetPosU() - map.GetY()) / TILE) == -87) &&
			(this->GetPosX() - map.GetX()) % TILE == 0 &&
			(this->GetPosU() - map.GetY()) % TILE == 0) {
			upmovable = false;
		}
		else {
			upmovable = true;
		}
		if ((map.GetMapData(0, (this->GetPosX() - map.GetX()) / TILE, (this->GetPosD() - map.GetY()) / TILE) == 0 ||
			map.GetMapData(0, (this->GetPosX() - map.GetX()) / TILE, (this->GetPosD() - map.GetY()) / TILE) == -87) &&
			(this->GetPosX() - map.GetX()) % TILE == 0 &&
			(this->GetPosD() - map.GetY()) % TILE == 0) {
			downmovable = false;
		}
		else {
			downmovable = true;
		}
		if ((map.GetMapData(0, (this->GetPosL() - map.GetX()) / TILE, (this->GetPosY() - map.GetY()) / TILE) == 0 ||
			map.GetMapData(0, (this->GetPosL() - map.GetX()) / TILE, (this->GetPosY() - map.GetY()) / TILE) == -87) &&
			(this->GetPosL() - map.GetX()) % TILE == 0 &&
			(this->GetPosY() - map.GetY()) % TILE == 0) {
			leftmovable = false;
		}
		else {
			leftmovable = true;
		}
		if ((map.GetMapData(0, (this->GetPosR() - map.GetX()) / TILE, (this->GetPosY() - map.GetY()) / TILE) == 0 ||
			map.GetMapData(0, (this->GetPosR() - map.GetX()) / TILE, (this->GetPosY() - map.GetY()) / TILE) == -87) &&
			(this->GetPosR() - map.GetX()) % TILE == 0 &&
			(this->GetPosY() - map.GetY()) % TILE == 0) {
			rightmovable = false;
		}
		else {
			rightmovable = true;
		}
		int xLen = _pos_x - _humanX;
		int yLen = _pos_y + _offsetY - _humanY;
		if (abs(xLen) < abs(yLen) && yLen < 0 && downmovable) _tracking = isdown;
		else if (abs(xLen) < abs(yLen) && yLen > 0 && upmovable) _tracking = isup;
		else if (abs(xLen) > abs(yLen) && xLen < 0 && rightmovable) _tracking = isright;
		else if (abs(xLen) > abs(yLen) && xLen > 0 && leftmovable) _tracking = isleft;
		// if block the longest LENGTH, then we need to choose other chance
		else if (abs(xLen) < abs(yLen) && xLen > 0 && rightmovable) _tracking = isright;
		else if (abs(xLen) < abs(yLen) && xLen < 0 && leftmovable) _tracking = isleft;
		else if (abs(xLen) > abs(yLen) && yLen > 0 && downmovable) _tracking = isdown;
		else if (abs(xLen) > abs(yLen) && yLen < 0 && upmovable) _tracking = isup;
		else _tracking = none;
	}
	void Oni::OnMove(GameMap &map) {
		//according humanXY
		//we find the abs((human - pos)X) abs((human - pos)Y)
		//which is bigger than the other
		//we will choose it
		//and move it first
		if (isCatch()) {
			TimerStop();
		}
		else {
			Track(map);
			TimerStart();
			if (TimerGetCount() == _moveTime) {
				TimerStop();
				_walkiter = !_walkiter;
			}
			if (IsTimerStart()) {
				if (TimerGetCount() % _moveTime == 0) {
					_nowmove = _tracking;
				}
				if (TimerGetCount() == _moveTime) {
					TimerReset();
					_walkiter = !_walkiter;
				}
				if (TimerGetCount() < (_moveTime / 2)) {
					_bstate = s1;
				}
				else {
					_bstate = s2;
				}
				if (_nowmove == isup) {
					_pos_y -= _step;
				}
				else if (_nowmove == isdown) {
					_pos_y += _step;
				}
				else if (_nowmove == isleft) {
					_pos_x -= _step;
				}
				else if (_nowmove == isright) {
					_pos_x += _step;
				}
				TimerUpdate();
				Countdown();
			}
			bitmap.SetTopLeft(_pos_x, _pos_y);
		}
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
		_overTime = 10*30;
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
	void Oni::SetWait() {
		_wait = true;
	}
	void Oni::Waiting(Oni::changeMap a) {
		if (_wait == true) {
			TimerStart();
		}
		else {
			TimerStop();
		}
		if (IsTimerStart()) {
			if (a == room)
				if (TimerGetCount() == 30)
					_wait = false;
			if (a == hallway)
				if (TimerGetCount() == 15)
					_wait = false;
			TimerUpdate();
		}
	}
	bool Oni::WaitOver() {
		return _wait;
	}
	bool Oni::isCatch() {
		return (_pos_x == _humanX || _pos_x + _offsetX == _humanX) 
			&& _pos_y + _offsetY == _humanY;
	}
	void Oni::SetVanish() {
		if (isCatch()) {
			_isDisappear = true;
			TimerStop();
			ResetOT();
			SetPos(0,0);
		}
	}
}