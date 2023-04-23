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
		_preX = 0;
		_preY = 0;
		_humanX = 0;
		_humanY = 0;
		_step = 0;
		_moveTime = 0;
		_offsetX = 0;
		_offsetY = 0;
		_resetX = 0;
		_resetY = 0;
		_isFixedPos = false;
		_press = false;
		_collide = false;
		_isCrossMap = false;
		_notShow = false;
		_1stCross = false;
		_type = no;
		_nowmove = none;
		_pressing = none;
	}
	ObjMove::~ObjMove() {
	}
	void ObjMove::SetParam(ObjType tp, int step, int moveTime, 
		int offsetX, int offsetY, int resetX, int resetY) {
		_type = tp;
		_step = step;
		_moveTime = moveTime;
		_offsetX = offsetX;
		_offsetY = offsetY;
		_resetX = resetX;
		_resetY = resetY;
		string bitmapName;
		if (_type == house1_2F_TR_chair) {
			bitmapName = "darkBrown_chair0";
		}
		else if (_type == house1_2F_TL_chair) {
			bitmapName = "darkBrown_chair1";
		}
		else if (_type == house1_basement2_chair) {
			bitmapName = "black_chair0";
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
		if (_type == house1_2F_TL_chair ||
			_type == house1_basement2_chair) return _pos_y + _offsetY;
		return _pos_y;
	}
	int ObjMove::GetPosL() {
		return _pos_x - TILE;
	}
	int ObjMove::GetPosU() {
		if (_type == house1_2F_TL_chair || 
			_type == house1_basement2_chair) return _pos_y + _offsetY - TILE;
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
			TRACE("\n\n playerX %d playerY %d objX %d objY %d\n\n", _humanX, _humanY, _pos_x, _pos_y);
			if (isCollide()) {
				TRACE("\n\n\n obj isCollide \n");
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
			TimerUpdate(clock());
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
		if (_type == house1_2F_TL_chair) {
			if (!(_pos_x == 12 * TILE && _pos_y == 17 * TILE - TILE / 2)) {
				bitmap.ShowBitmap();
			}
			else {
				_notShow = true;
			}
		}
		else if (_type == house1_basement2_chair) {
			if (!(_pos_x == 15 * TILE && _pos_y == 12 * TILE - TILE / 2) && !_isCrossMap) {
				bitmap.ShowBitmap();
			}
			else if (_1stCross) {
				Reset();
				bitmap.ShowBitmap();
				_1stCross = false;
			}
			else if (_isCrossMap && !(_pos_x == 18 * TILE && _pos_y == 8 * TILE - TILE / 2) && !(_pos_x == 5 * TILE && _pos_y == 8 * TILE - TILE / 2)) {
				bitmap.ShowBitmap();
			}
			else {
				_notShow = true;
			}
		}
		else {
			bitmap.ShowBitmap();
		}
	}
	void ObjMove::Reset() {
		SetXY(_resetX - _offsetX, _resetY - _offsetY);
		bitmap.SetTopLeft(_pos_x, _pos_y);
	}
	void ObjMove::Fixed() {
		if (_type == house1_2F_TR_chair) {
			if (GetPosX() == 16 * TILE && GetPosY() == 9 * TILE) {
				_isFixedPos = true;
				_resetX = 16 * TILE;
				_resetY = 9 * TILE;
			}
			else if (GetPosX() == 14 * TILE && GetPosY() == 9 * TILE) {
				_isFixedPos = true;
				_resetX = 14 * TILE;
				_resetY = 9 * TILE;
			}
		}
		else if (_type == house1_2F_TL_chair) {
			if (GetPosX() == 13 * TILE && GetPosY() == 9 * TILE) {
				_isFixedPos = true;
				_resetX = 13 * TILE;
				_resetY = 9 * TILE;
			}
		}
		else if (_type == house1_basement2_chair) {
			if (_isCrossMap) {
				if (!_isFixedPos) {
					_resetX = 7 * TILE;
					_resetY = 8 * TILE;
				} 
				if (GetPosX() == 13 * TILE && GetPosY() == 18 * TILE) {
					_isFixedPos = true;
					_resetX = 13 * TILE;
					_resetY = 18 * TILE;
				}
			}
			else { // 9 11
				if (GetPosX() == 9 * TILE && GetPosY() == 11 * TILE) {
					_resetX = 9 * TILE;
					_resetY = 11 * TILE;
				}
			}
		}
	}
	bool ObjMove::isCollide() {
		int x = _pos_x + _offsetX;
		int y = _pos_y + _offsetY;
		if ((_humanX + TILE == GetPosX() && (_humanY >= GetPosY() && _humanY <= y) && _pressing == isright)
			|| (_humanX - TILE == x && (_humanY >= GetPosY() && _humanY <= y) && _pressing == isleft)
			|| ((_humanX >= GetPosX() && _humanX <= x) && _humanY + TILE == GetPosY() && _pressing == isdown)
			|| ((_humanX >= GetPosX() && _humanX <= x) && _humanY - TILE == y) && _pressing == isup)
			_collide = true;
		else
			_collide = false;

		return _collide;
	}
	bool ObjMove::IsFixed() {
		return _isFixedPos;
	}
	void ObjMove::ChangeMap() {
		// house1_2F_TR_chair not change map
		if (_type == house1_2F_TL_chair){
			Reset();
			_notShow = false;
		}
		else if (_type == house1_basement2_chair) { // 9 11
			if (_isCrossMap) {
				Fixed();
				Reset();
				_notShow = false;
			}
			if (_resetX == 9 * TILE && _resetY == 11 * TILE && _notShow) {
				_isCrossMap = true;
				_notShow = false;
				_1stCross = true;
			}
			else {
				Reset();
				_notShow = false;
			}
		}
		
	}
	bool ObjMove::isChangeMap() {
		return _isCrossMap;
	}
}