#include "stdafx.h"
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "config.h"
#include "GameMap.h"
#include "Entity.h"
#include "ObjMove.h"

namespace game_framework {
	ObjMove::ObjMove() {
		_humanX, _humanY,
			_step, _moveTime, _overTime,
			_offsetX, _offsetY,
			_resetX, _resetY,
			_fixedX, _fixedY = 0;
		_isFixedPos = false;
		_press = false;
		_collide = false;
		_type = no;
		_nowmove = none;
		_pressing = none;
	}
	ObjMove::~ObjMove() {
	}
	void ObjMove::SetParam(ObjType tp, int step, int moveTime, 
		int offsetX, int offsetY, int resetX, int resetY, 
		int fixedX, int fixedY) {
		_type = tp;
		_step = step;
		_moveTime = moveTime;
		_offsetX = offsetX;
		_offsetY = offsetY;
		_resetX = resetX;
		_resetY = resetY;
		_fixedX = fixedX;
		_fixedY = fixedY;
		string bitmapName;
		if (_type == red_chair) {
			bitmapName = "darkBrown_chair";
		}
		Load(bitmapName, RGB(204, 255, 0));
	}
	void ObjMove::Load(string filename, COLORREF color) {
		vector<string> objVec;
		objVec.push_back("img/item_animation/chair/" + filename + ".bmp");
		bitmap.LoadBitmapByString(objVec, color);
	}
	int ObjMove::GetPosX() {
		return _pos_x;
	}
	int ObjMove::GetPosY() {
		return _pos_y;
	}
	int ObjMove::GetPosL() {
		return _pos_x - TILE;
	}
	int ObjMove::GetPosU() {
		return _pos_y - TILE;
	}
	int ObjMove::GetPosR() {
		return _pos_x + _offsetX + TILE;
	}
	int ObjMove::GetPosD() {
		return _pos_y + _offsetY + TILE;
	}
	void ObjMove::GetPlayerPos(int playerX, int playerY) {
		_humanX = playerX;
		_humanY = playerY;
	}
	void ObjMove::Track(GameMap &map) {
		bool upmovable = false;
		bool downmovable = false;
		bool leftmovable = false;
		bool rightmovable = false;

		if ((map.GetMapData(0, (this->GetPosX() - map.GetX()) / TILE, (this->GetPosU() - map.GetY()) / TILE) == 0 ||
			map.GetMapData(0, (this->GetPosX() - map.GetX()) / TILE, (this->GetPosU() - map.GetY()) / TILE) == -87) &&
			(this->GetPosX() - map.GetX()) % TILE == 0 &&
			(this->GetPosU() - map.GetY()) % TILE == 0 ||  
			_pressing != isup) {
			upmovable = false;
		}
		else {
			upmovable = true;
		}
		if ((map.GetMapData(0, (this->GetPosX() - map.GetX()) / TILE, (this->GetPosD() - map.GetY()) / TILE) == 0 ||
			map.GetMapData(0, (this->GetPosX() - map.GetX()) / TILE, (this->GetPosD() - map.GetY()) / TILE) == -87) &&
			(this->GetPosX() - map.GetX()) % TILE == 0 &&
			(this->GetPosD() - map.GetY()) % TILE == 0 || 
			_pressing != isdown) {
			downmovable = false;
		}
		else {
			downmovable = true;
		}
		if ((map.GetMapData(0, (this->GetPosL() - map.GetX()) / TILE, (this->GetPosY() - map.GetY()) / TILE) == 0 ||
			map.GetMapData(0, (this->GetPosL() - map.GetX()) / TILE, (this->GetPosY() - map.GetY()) / TILE) == -87) &&
			(this->GetPosL() - map.GetX()) % TILE == 0 &&
			(this->GetPosY() - map.GetY()) % TILE == 0 || 
			_pressing != isleft) {
			leftmovable = false;
		}
		else {
			leftmovable = true;
		}
		if ((map.GetMapData(0, (this->GetPosR() - map.GetX()) / TILE, (this->GetPosY() - map.GetY()) / TILE) == 0 ||
			map.GetMapData(0, (this->GetPosR() - map.GetX()) / TILE, (this->GetPosY() - map.GetY()) / TILE) == -87) &&
			(this->GetPosR() - map.GetX()) % TILE == 0 &&
			(this->GetPosY() - map.GetY()) % TILE == 0 || 
			_pressing != isright) {
			rightmovable = false;
		}
		else {
			rightmovable = true;
		}
		if (leftmovable) _nowmove = isleft;
		else if (upmovable) _nowmove = isup;
		else if (rightmovable) _nowmove = isright;
		else if (downmovable) _nowmove = isdown;
		else _nowmove = none;

		/*if (_pressing == isleft) { TRACE("\n\n lll \n\n"); }
		if (_pressing == isup) { TRACE("\n\n uuu \n\n"); }
		if (_pressing == isright) { TRACE("\n\n rrr \n\n"); }
		if (_pressing == isdown) { TRACE("\n\n ddd \n\n"); }*/

		_pressing = none;
	}
	void ObjMove::OnMove(GameMap &map) {	// every time obj move, will track first
		Fixed(); // to check isFixed or not
		if (_press && !_isFixedPos) {
			if (isCollide()) {
				//TRACE("\n\n\n obj isCollide \n");
				Track(map);
			}
			_press = false;
		}
		if (_nowmove != none) {
			TimerStart();
		}
		if (TimerGetCount() == _moveTime) {
			TimerStop();
			_nowmove = none;
		}
		if (IsTimerStart()) {
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
		}
		bitmap.SetTopLeft(_pos_x, _pos_y);
	}
	void ObjMove::OnKeyDown(UINT nChar) {
		if (nChar == VK_SPACE) {
			_press = true;
		}
		if (nChar == VK_LEFT) {			
			_pressing = isleft;
			//TRACE("\n\n press L \n\n");
		}
		else if (nChar == VK_UP) {			
			_pressing = isup;
			//TRACE("\n\n press U \n\n");
		}
		else if (nChar == VK_RIGHT) {			
			_pressing = isright;
			//TRACE("\n\n press R \n\n");
		}
		else if (nChar == VK_DOWN) {			
			_pressing = isdown;
			//TRACE("\n\n press D \n\n");
		}
	}
	void ObjMove::OnKeyUp(UINT nChar) {
		if (nChar == VK_SPACE) {
			_press = false;
		}
	}
	void ObjMove::OnShow() {
		bitmap.ShowBitmap();
	}
	void ObjMove::Reset() {
		SetXY(_resetX, _resetY);
	}
	void ObjMove::Fixed() {
		if (_pos_x == _fixedX && _pos_y == _fixedY)
			_isFixedPos = true;
		if (_isFixedPos) {
			_resetX = _fixedX;
			_resetY = _fixedY;
		}
	}
	bool ObjMove::isCollide() {
		int x = _pos_x + _offsetX;
		int y = _pos_y + _offsetY;
		if ((_humanX + 32 == _pos_x && (_humanY >= _pos_y || _humanY <= y) && _pressing == isright)
			|| (_humanX - 32 == x && (_humanY >= _pos_y || _humanY <= y) && _pressing == isleft)
			|| ((_humanX >= _pos_x || _humanX <= x) && _humanY + 32 == _pos_y && _pressing == isdown)
			|| ((_humanX >= _pos_x || _humanX <= x) && _humanY - 32 == y) && _pressing == isup)
			_collide = true;
		else
			_collide = false;

		return _collide;
	}
	bool ObjMove::IsFixed() {
		return _isFixedPos;
	}
}