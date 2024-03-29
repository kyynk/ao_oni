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
		_objclass = "ObjMove";
	}
	ObjMove::~ObjMove() = default;
	void ObjMove::SetParam(ObjName tp, int step, int moveTime, 
		int offsetX, int offsetY, int resetX, int resetY) {
		_isFixedPos = false;
		_isFixedPos2 = false;
		_press = false;
		_isCrossMap = false;
		_notShow = false;
		_1stCross = false;
		_nowmove = none;
		_pressing = none;

		_type = tp;
		_step = step;
		_moveTime = moveTime;
		_offsetX = offsetX;
		_offsetY = offsetY;
		_resetX = resetX;
		_resetY = resetY;
		if (_type == house1_2F_TR_chair) {
			Load("darkBrown_chair0", RGB(204, 255, 0));
		}
		else if (_type == house1_2F_TL_chair) {
			Load("darkBrown_chair1", RGB(204, 255, 0));
		}
		else if (_type == house1_basement2_chair) {
			Load("black_chair0", RGB(204, 255, 0));
		}
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
		if (_type == house1_2F_TL_chair||_type == house1_basement2_chair) 
			return _pos_y + _offsetY;
		return _pos_y;
	}
	int ObjMove::GetPosL() {
		return _pos_x - TILE;
	}
	int ObjMove::GetPosU() {
		if (_type == house1_2F_TL_chair || _type == house1_basement2_chair)
			return _pos_y + _offsetY - TILE;
		return _pos_y - TILE;
	}
	int ObjMove::GetPosR() {
		return _pos_x + _offsetX + TILE;
	}
	int ObjMove::GetPosD() {
		return _pos_y + _offsetY + TILE;
	}
	int ObjMove::GetPosY_offset() {
		return _pos_y + _offsetY;
	}
	void ObjMove::StorePlayerPos(int playerX, int playerY) {
		_playerX = playerX;
		_playerY = playerY;
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
			_pressing != up) {
			upmovable = false;
		}
		else {
			upmovable = true;
		}
		if ((map.GetMapData(0, (this->GetPosX() - map.GetX()) / TILE, (this->GetPosD() - map.GetY()) / TILE) == 0 ||
			map.GetMapData(0, (this->GetPosX() - map.GetX()) / TILE, (this->GetPosD() - map.GetY()) / TILE) == -87) &&
			(this->GetPosX() - map.GetX()) % TILE == 0 &&
			(this->GetPosD() - map.GetY()) % TILE == 0 || 
			_pressing != down) {
			downmovable = false;
		}
		else {
			downmovable = true;
		}
		if ((map.GetMapData(0, (this->GetPosL() - map.GetX()) / TILE, (this->GetPosY() - map.GetY()) / TILE) == 0 ||
			map.GetMapData(0, (this->GetPosL() - map.GetX()) / TILE, (this->GetPosY() - map.GetY()) / TILE) == -87) &&
			(this->GetPosL() - map.GetX()) % TILE == 0 &&
			(this->GetPosY() - map.GetY()) % TILE == 0 || 
			_pressing != left) {
			leftmovable = false;
		}
		else {
			leftmovable = true;
		}
		if ((map.GetMapData(0, (this->GetPosR() - map.GetX()) / TILE, (this->GetPosY() - map.GetY()) / TILE) == 0 ||
			map.GetMapData(0, (this->GetPosR() - map.GetX()) / TILE, (this->GetPosY() - map.GetY()) / TILE) == -87) &&
			(this->GetPosR() - map.GetX()) % TILE == 0 &&
			(this->GetPosY() - map.GetY()) % TILE == 0 || 
			_pressing != right) {
			rightmovable = false;
		}
		else {
			rightmovable = true;
		}
		if (leftmovable) _nowmove = left;
		else if (upmovable) _nowmove = up;
		else if (rightmovable) _nowmove = right;
		else if (downmovable) _nowmove = down;
		else _nowmove = none;
		_pressing = none;
	}
	void ObjMove::OnMove(GameMap &map) {
		Fixed(); 
		if (_press && !_isFixedPos) {
			if (isCollide()) {
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
		bitmap.SetTopLeft(_pos_x, _pos_y);
	}
	void ObjMove::OnKeyDown(UINT nChar) {
		if (nChar == VK_SPACE) {
			_press = true;
		}
		if (nChar == VK_LEFT) {			
			_pressing = left;
		}
		else if (nChar == VK_UP) {			
			_pressing = up;
		}
		else if (nChar == VK_RIGHT) {			
			_pressing = right;
		}
		else if (nChar == VK_DOWN) {			
			_pressing = down;
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
	void ObjMove::OnShowConditional(bool flip)
	{
		bool changemap = (flip ? isChangeMap() : !isChangeMap());
		if (changemap) {
			OnShow();
		}
		else {
			ChangeMap();
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
					_isFixedPos2 = true;
					
					_resetX = 9 * TILE;
					_resetY = 11 * TILE;
				}
			}
		}
	}
	bool ObjMove::isCollide() {
		int x = _pos_x + _offsetX;
		int y = _pos_y + _offsetY;
		if ((_playerX + TILE == GetPosX() && (_playerY >= GetPosY() && _playerY <= y) && _pressing == right)
			|| (_playerX - TILE == x && (_playerY >= GetPosY() && _playerY <= y) && _pressing == left)
			|| ((_playerX >= GetPosX() && _playerX <= x) && _playerY + TILE == GetPosY() && _pressing == down)
			|| ((_playerX >= GetPosX() && _playerX <= x) && _playerY - TILE == y) && _pressing == up)
			return true;
		return false;

	}
	bool ObjMove::IsFixed() {
		return _isFixedPos;
	}
	bool ObjMove::IsFixed2() {
		return _isFixedPos2;
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
	void ObjMove::SetPrePos() {
		_preX = GetPosX();
		_preY = GetPosY();
	}
	void ObjMove::SetPreX(int x) {
		_preX = x;
	}
	void ObjMove::SetPreY(int y) {
		_preY = y;
	}
	int ObjMove::GetPreX() {
		return _preX;
	}
	int ObjMove::GetPreY() {
		return _preY;
	}
}