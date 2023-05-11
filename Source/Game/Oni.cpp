#include "stdafx.h"
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include <string>
#include "config.h"
#include "GameMap.h"
#include "Entity.h"
#include "Oni.h"

namespace game_framework {
	void Oni::init(OniName tp, int step, int moveTime) {
		_isShow = false;
		_walkiter = true;
		_bstate = s1;
		_wait = false;
		_nowmove = none;
		_tracking = none;
		_overTime = 900;
		_type = tp;
		_step = step;
		_moveTime = moveTime;
		GonnaGiveUpSoSadUntilTheNextMap = false;
		string name;
		if (_type == normal) {
			name = "oni_";
			_offsetX = TILE / 2;
			_offsetY = 80;
		}
		else if (_type == mika) {
			name = "mika_";
			_offsetX = TILE / 2;
			_offsetY = 80;
		}
		else if (_type == takesi) {
			name = "takesi_";
			_offsetX = TILE / 2;
			_offsetY = 80;
		}
		else if (_type == takurou) {
			name = "takurou_";
			_offsetX = TILE / 2;
			_offsetY = 80;
		}
		else if (_type == flat) {
			name = "flat_";
			_offsetX = TILE / 2;
			_offsetY = 48;
		}
		Load(name, RGB(204, 255, 0));
	}
	void Oni::SetPos(int x, int y) {
		SetXY(x - _offsetX, y - _offsetY);
	}
	int Oni::GetPosX() const {
		return _pos_x + _offsetX;
	}
	int Oni::GetPosY() const {
		return _pos_y;
	}
	
	int Oni::GetPosL() const {
		return _pos_x + _offsetX - TILE;
	}
	int Oni::GetPosU() const {
		return _pos_y + _offsetY - TILE;
	}
	int Oni::GetPosR() const {
		return _pos_x + _offsetX + TILE;
	}
	int Oni::GetPosD() const {
		return _pos_y + _offsetY + TILE;
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
	void Oni::SetPlayerPos(int playerX, int playerY) {
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
		if ((map.GetMapData(0, (this->GetPosL() - map.GetX()) / TILE, (this->GetPosY() + _offsetY - map.GetY()) / TILE) == 0 ||
			map.GetMapData(0, (this->GetPosL() - map.GetX()) / TILE, (this->GetPosY() + _offsetY - map.GetY()) / TILE) == -87) &&
			(this->GetPosL() - map.GetX()) % TILE == 0 &&
			(this->GetPosY() + _offsetY - map.GetY()) % TILE == 0) {
			leftmovable = false;
		}
		else {
			leftmovable = true;
		}
		if ((map.GetMapData(0, (this->GetPosR() - map.GetX()) / TILE, (this->GetPosY() + _offsetY - map.GetY()) / TILE) == 0 ||
			map.GetMapData(0, (this->GetPosR() - map.GetX()) / TILE, (this->GetPosY() + _offsetY - map.GetY()) / TILE) == -87) &&
			(this->GetPosR() - map.GetX()) % TILE == 0 &&
			(this->GetPosY() + _offsetY - map.GetY()) % TILE == 0) {
			rightmovable = false;
		}
		else {
			rightmovable = true;
		}
		int xLen = _pos_x + _offsetX - _humanX;
		int yLen = _pos_y + _offsetY - _humanY;
		if (_pos_x + TILE / 2 <= _humanX && _humanX <= _pos_x + TILE / 2) xLen = 0;
		
		//TRACE("\n\n   xLen %d     yLen %d \n\n", xLen, yLen);
		if (abs(xLen) < abs(yLen) && yLen < 0 && downmovable) _tracking = down;
		else if (abs(xLen) < abs(yLen) && yLen > 0 && upmovable) _tracking = up;
		else if (abs(xLen) > abs(yLen) && xLen < 0 && rightmovable) _tracking = right;
		else if (abs(xLen) > abs(yLen) && xLen > 0 && leftmovable) _tracking = left;
		// if block the longest LENGTH, then we need to choose other chance
		else if (abs(xLen) < abs(yLen) && xLen < 0 && rightmovable) _tracking = right;
		else if (abs(xLen) < abs(yLen) && xLen > 0 && leftmovable) _tracking = left;
		else if (abs(xLen) > abs(yLen) && yLen < 0 && downmovable) _tracking = down;
		else if (abs(xLen) > abs(yLen) && yLen > 0 && upmovable) _tracking = up;
		// if abs xLen == abs yLen, then choose xLen first
		else if (abs(xLen) == abs(yLen) && xLen < 0 && rightmovable) _tracking = right;
		else if (abs(xLen) == abs(yLen) && xLen > 0 && leftmovable) _tracking = left;
		else _tracking = none;
	}
	void Oni::OnMove(GameMap &map) {
		if (_wait) {
			if (IsTimerStart() && TimerGetCount() == 30) {
				_wait = false;
				SetPos(_nextx + map.GetX(), _nexty + map.GetY());
				bitmap.SetTopLeft(_pos_x, _pos_y);
				TimerStop();
			}
			else {
				TimerUpdate();
			}
		}
		else {
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
					if (_nowmove == up) {
						_pos_y -= _step;
					}
					else if (_nowmove == down) {
						_pos_y += _step;
					}
					else if (_nowmove == left) {
						_pos_x -= _step;
					}
					else if (_nowmove == right) {
						_pos_x += _step;
					}
					TimerUpdate();
				}
				if (!_changemaponceprocess) {
					Countdown();
				}
				bitmap.SetTopLeft(_pos_x, _pos_y);

			}
		}
	}
	void Oni::OnShow() {
		if ((_isShow && !_wait) || GonnaGiveUpSoSadUntilTheNextMap) {
			if (_nowmove == up) {
				if (_bstate == s1) {
					_walkiter ? bitmap.SetFrameIndexOfBitmap(BITMAP_UP_1) : bitmap.SetFrameIndexOfBitmap(BITMAP_UP_2);
				}
				else {
					bitmap.SetFrameIndexOfBitmap(BITMAP_UP);
				}
			}
			else if (_nowmove == down) {
				if (_bstate == s1) {
					_walkiter ? bitmap.SetFrameIndexOfBitmap(BITMAP_DOWN_1) : bitmap.SetFrameIndexOfBitmap(BITMAP_DOWN_2);
				}
				else {
					bitmap.SetFrameIndexOfBitmap(BITMAP_DOWN);
				}
			}

			else if (_nowmove == left) {
				if (_bstate == s1) {
					_walkiter ? bitmap.SetFrameIndexOfBitmap(BITMAP_LEFT_1) : bitmap.SetFrameIndexOfBitmap(BITMAP_LEFT_2);
				}
				else {
					bitmap.SetFrameIndexOfBitmap(BITMAP_LEFT);
				}
			}

			else if (_nowmove == right) {
				if (_bstate == s1) {
					_walkiter ? bitmap.SetFrameIndexOfBitmap(BITMAP_RIGHT_1) : bitmap.SetFrameIndexOfBitmap(BITMAP_RIGHT_2);
				}
				else {
					bitmap.SetFrameIndexOfBitmap(BITMAP_RIGHT);
				}
			}
			bitmap.ShowBitmap();
		}
		if (_wait) {
			if (GonnaGiveUpSoSadUntilTheNextMap) {
				ResetOni();
				GonnaGiveUpSoSadUntilTheNextMap = false;
			}
			TimerStart();
			
		}
		
		
	}
	void Oni::Countdown() {
		
		if (_overTime == 0) {
			GonnaGiveUpSoSadUntilTheNextMap = true;
		}
		else _overTime--;
	}
	void Oni::AddTime() {
		_overTime+=10;
	}

	void Oni::SetChangeMap(int x,int y,int id)
	{
		if (_changemaponceprocess) {
			_overTime += 100;
			_wait = true;
			_nextx = x;
			_nexty = y;
			_mapID = id;
			_changemaponceprocess = false;
		}

	}

	bool Oni::isCatch() {
		return _pos_x + _offsetX == _humanX
			&& _pos_y + _offsetY == _humanY;
	}
	
	void Oni::ResetOni() {
		_isShow = false;
		_overTime = 900;
		TimerStop();
	}
	
}